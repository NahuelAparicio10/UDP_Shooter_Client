#include "ClientUDP.h"

ClientUDP::ClientUDP() { Init(); }

ClientUDP::~ClientUDP() { _socket.unbind(); }

bool ClientUDP::Init()
{
    if (_socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[CLIENT_UDP] Failed to bind UDP socket\n";
        return false;
    }
    return true;
}

void ClientUDP::Send(PacketHeader header, PacketType type, const std::string& content, sf::IpAddress ip, unsigned short port)
{
    SendDatagram(_socket, header, type, content, ip, port);
}

void ClientUDP::StartMatchSearchWithRetry(std::string matchType)
{
    const int maxRetries = 5;
    int retries = 0;
    bool ackReceived = false;

    while (retries < maxRetries && !ackReceived)
    {
        Send(PacketHeader::CRITICAL, PacketType::FIND_MATCH, matchType, Constants::ServiceServerIP.value(), Constants::MatchMakingServerPort);
        std::cout << "[CLIENT_UDP] Sent FIND_MATCH: " << matchType << " (retry " << retries << ")\n";

        sf::Clock timer;
        while (timer.getElapsedTime().asSeconds() < 1.0f)
        {
            char buffer[1024];
            std::size_t received = 0;
            std::optional<sf::IpAddress> sender = std::nullopt;
            unsigned short senderPort;

            if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
            {
                RawPacketJob job;
                if (ParseRawDatagram(buffer, received, job, sender.value(), senderPort) && job.type == PacketType::SEARCH_ACK)
                {
                    std::cout << "[CLIENT_UDP] Received SEARCH_ACK from server.\n";
                    ackReceived = true;
                    break;
                }
            }

            sf::sleep(sf::milliseconds(100));
        }

        if (!ackReceived)
            retries++;
    }

    if (ackReceived)
    {
        StartListeningForMatch();
    }
    else
    {
        std::cout << "[CLIENT_UDP] No ACK received after retries. Match search failed.\n";
    }
}

void ClientUDP::StartReceivingGameplayPackets()
{
    _receivingGameplay = true;

    _dispatcher.RegisterHandler(PacketType::PLAYER_MOVEMENT, [this](const RawPacketJob& job) {
            MovementPacket packet = MovementPacket::Deserialize(job.content);
            onMovementPacketRecived.Invoke(packet);
        });

    _dispatcher.RegisterHandler(PacketType::RECONCILE, [this](const RawPacketJob& job) {
            MovementPacket packet = MovementPacket::Deserialize(job.content);
            onReconcilePacketRecived.Invoke(packet);
        });

    std::thread gameplayThread([this]()
        {
            _socket.setBlocking(false);

            while (_receivingGameplay)
            {
                char buffer[1024];
                std::size_t received = 0;
                std::optional<sf::IpAddress> sender = std::nullopt;
                unsigned short senderPort;

                if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
                {
                    RawPacketJob job;
                    if (ParseRawDatagram(buffer, received, job, sender.value(), senderPort))
                    {
                        _dispatcher.EnqueuePacket(job);
                    }
                }

                sf::sleep(sf::milliseconds(10));
            }

            std::cout << "[CLIENT_UDP] Gameplay reception thread ended.\n";
        });

    gameplayThread.detach();
}

void ClientUDP::StopReceivingGameplayPackets()
{
    _receivingGameplay = false;
}

void ClientUDP::StartListeningForMatch()
{
    if (_listening) return;
    _listening = true;
    _dispatcher.Stop();
    _dispatcher.RegisterHandler(PacketType::MATCH_FOUND, [this](const RawPacketJob& job)
        {
            Send(PacketHeader::NORMAL, PacketType::ACK_MATCH_FOUND, "", job.sender.value(), job.port);
            _listening = false;

            std::istringstream ss(job.content);
            std::string ipStr, portStr, matchIDStr, playerIDStr;
            std::getline(ss, ipStr, ':');
            std::getline(ss, portStr, ':');
            std::getline(ss, matchIDStr, ':');
            std::getline(ss, playerIDStr, ':');

            std::optional<sf::IpAddress> ip = sf::IpAddress::resolve(ipStr);
            unsigned short port = static_cast<unsigned short>(std::stoi(portStr));
            unsigned int matchID = std::stoi(matchIDStr);
            unsigned int playerID = std::stoi(playerIDStr);

            _gameServerIp = ip;
            _gameServerPort = port;
            currentMatchID = matchID;      
            currentPlayerID = playerID;

            std::cout << "[CLIENT_UDP] Match encontrado en GameServer: " << ip.value() << ":" << port << "\n";

            _gameServerIp = ip;
            _gameServerPort = port;

            JoinGameServer();
            onMatchFound.Invoke(job.content);
        });

    _dispatcher.RegisterHandler(PacketType::ACK_JOINED, [this](const RawPacketJob& job) {
        std::cout << "[CLIENT] Received ACK_JOINED from server.\n";
        _joinedConfirmed = true;
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

            if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
            {
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

    while (timer.getElapsedTime().asSeconds() < 0.5f)
    {
        char buffer[1024];
        std::size_t received = 0;
        std::optional<sf::IpAddress> sender = std::nullopt;
        unsigned short senderPort;

        if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
        {
            RawPacketJob job;
            if (ParseRawDatagram(buffer, received, job, sender.value(), senderPort) && job.type == PacketType::OK)
            {
                std::cout << "[CLIENT_UDP] Cancel confirmed by server.\n";
                onCancelConfirmed.Invoke();
                return;
            }
        }

        sf::sleep(sf::milliseconds(50));
    }

    std::cout << "[CLIENT_UDP] Cancel timeout - no response from server.\n";
}

void ClientUDP::JoinGameServer()
{
    if (!_gameServerIp.has_value()) return;

    _joinedConfirmed = false;

    std::string content = std::to_string(currentMatchID) + ":" + std::to_string(currentPlayerID);

    std::thread joinThread([this, content]() {
        int attempts = 0;
        while (!_joinedConfirmed && attempts < 10) {
            Send(PacketHeader::CRITICAL, PacketType::JOIN_GAME, content,
                GetGameServerIP().value(), GetCurrentGameServerPort());
            sf::sleep(sf::milliseconds(200));
            attempts++;
        }

        if (!_joinedConfirmed) {
            std::cout << "[CLIENT] Failed to join game server.\n";
        }
        });

    joinThread.detach();
}
