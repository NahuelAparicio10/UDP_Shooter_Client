#include "ClientUDP.h"

ClientUDP::ClientUDP() 
{
    Init();
}

ClientUDP::~ClientUDP()
{
	_socket.unbind();
}

bool ClientUDP::Init()
{
    if (_socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done) 
    {
        std::cerr << "[CLIENT_UDP] Failed to bind UDP socket\n";
        return false;
    }
    return true;
}

void ClientUDP::Send(PacketHeader header, PacketType type, const std::string& content, sf::IpAddress ip, unsigned short port) { SendDatagram(_socket, header, type, content, ip, port); }

// -- Listens for a match incoming message from Service Server on recieved sends a ACK to verify without timeout

void ClientUDP::StartListeningForMatch()
{
    if (_listening) return;
    _listening = true;

    _dispatcher.RegisterHandler(PacketType::MATCH_FOUND, [this](const RawPacketJob& job) 
        {
        Send(PacketHeader::NORMAL, PacketType::ACK_MATCH_FOUND, "", job.sender.value(), job.port);
        _listening = false;

        std::string ipPort = job.content;
        std::size_t sep = ipPort.find(':');
        if (sep == std::string::npos) {
            std::cerr << "[CLIENT_UDP] Formato de IP inválido en MATCH_FOUND\n";
            return;
        }

        std::string ipStr = ipPort.substr(0, sep);
        std::string portStr = ipPort.substr(sep + 1);
        std::optional<sf::IpAddress> ip = sf::IpAddress::resolve(ipStr);
        unsigned short port = static_cast<unsigned short>(std::stoi(portStr));

        std::cout << "[CLIENT_UDP] Match encontrado en GameServer: " << ip.value() << ":" << port << "\n";

        _gameServerIp = ip;
        _gameServerPort = port;

        onMatchFound.Invoke(job.content);
        });

    _dispatcher.Start();

    _matchmakingThread = std::thread([this]() {
        _socket.setBlocking(false);
        while (_listening)
        {
            char buffer[1024];
            std::size_t received = 0;
            std::optional<sf::IpAddress> sender = std::nullopt;
            unsigned short senderPort;

            if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value()) {
                RawPacketJob job;
                if (ParseRawDatagram(buffer, received, job, sender.value(), senderPort))
                    _dispatcher.EnqueuePacket(job);
            }

            sf::sleep(sf::milliseconds(100));
        }
        std::cout << "[CLIENT_UDP] Listening stopped.\n";
        });
    _matchmakingThread.detach();
}

void ClientUDP::CancelMatchSearch() 
{ 
    if (!_listening) return;
    _listening = false;

    Send(PacketHeader::NORMAL, PacketType::CANCEL_SEARCH, "", Constants::ServiceServerIP.value(), Constants::MatchMakingServerPort);

    _socket.setBlocking(false);
    sf::Clock timer;

    while (timer.getElapsedTime().asSeconds() < 0.5f) {
        char buffer[1024];
        std::size_t received = 0;
        std::optional<sf::IpAddress> sender = std::nullopt;
        unsigned short senderPort;

        if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value()) {
            RawPacketJob job;
            if (ParseRawDatagram(buffer, received, job, sender.value(), senderPort) && job.type == PacketType::OK) {
                std::cout << "[CLIENT_UDP] Cancel confirmed by server.\n";
                onCancelConfirmed.Invoke();
                return;
            }
        }
        sf::sleep(sf::milliseconds(50));
    }

    std::cout << "[CLIENT_UDP] Cancel timeout - no response from server.\n";
}

void ClientUDP::RequestMatch(PacketType type)
{
    _dispatcher.RegisterHandler(PacketType::SEARCH_ACK, [this](const RawPacketJob& job) {
        std::cout << "[CLIENT_UDP] Match search ACK received. Starting to listen...\n";
        StartListeningForMatch(); 
        });

    std::string content = (type == PacketType::FIND_MATCH ? "NORMAL" : "RANKED");

    Send(PacketHeader::CRITICAL, PacketType::FIND_MATCH, content,
        Constants::ServiceServerIP.value(), Constants::MatchMakingServerPort);
}


