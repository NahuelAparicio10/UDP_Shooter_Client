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

void ClientUDP::SendFindMatch(std::string command)
{
    std::string msg = "FIND_MATCH:" + command;
    _socket.send(msg.c_str(), msg.size(), Constants::ServiceServerIP.value(), Constants::MatchMakingServerPort);
}

// -- Listens for a match incoming message from Service Server on recieved sends a ACK to verify with timeout

std::optional<std::string> ClientUDP::ListenForMatch(float timeoutSeconds)
{
    _socket.setBlocking(false);
    sf::Clock timer;

    while (timer.getElapsedTime().asSeconds() < timeoutSeconds)
    {
        char buffer[1024];
        std::size_t received = 0;
        std::optional<sf::IpAddress> sender = std::nullopt;
        unsigned short senderPort;

        if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
        {
            std::string msg(buffer, received);

            if (msg.find("MATCH_FOUND:") == 0)
            {
                SendACK(sender.value(), senderPort, "ACK_MATCH_FOUND");
                return msg;
            }
        }
    }
    return std::nullopt;
}

// -- Listens for a match incoming message from Service Server on recieved sends a ACK to verify without timeout

void ClientUDP::StartListeningForMatch()
{
    if (_listening) return;

    _listening = true;
    // - Start thread for check constantly every X ms if the server did found us a match

    _matchmakingThread = std::thread([this]() {
        _socket.setBlocking(false);
        sf::Clock timer;

        while (_listening)
        {
            char buffer[1024];
            std::size_t received = 0;
            std::optional<sf::IpAddress> sender = std::nullopt;
            unsigned short senderPort;

            if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
            {
                std::string msg(buffer, received);

                if (msg.find("MATCH_FOUND:") == 0)
                {
                    // - If server founds a match we answer with ACK response and invoke the event for notify the scene
                    SendACK(sender.value(), senderPort, "ACK_MATCH_FOUND");
                    _listening = false;
                    onMatchFound.Invoke(msg);
                    return;
                }
            }

            sf::sleep(sf::milliseconds(100)); // Reduce carga CPU
        }
        std::cout << "[CLIENT_UDP] Listening stopped.\n";

        });

    _matchmakingThread.detach();
}

void ClientUDP::CancelMatchSearch() 
{ 
    if (!_listening) return;
    _listening = false;

    std::string cancelMsg = "CANCELED_SEARCHING";

    _socket.send(cancelMsg.c_str(), cancelMsg.size(), Constants::ServiceServerIP.value(), Constants::MatchMakingServerPort);

    // Espera confirmación del servidor
    _socket.setBlocking(false);
    sf::Clock timer;

    while (timer.getElapsedTime().asSeconds() < 0.5f) // timeout de espera
    {
        char buffer[1024];
        std::size_t received = 0;
        std::optional<sf::IpAddress> sender = std::nullopt;
        unsigned short senderPort;

        if (_socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done)
        {
            std::string msg(buffer, received);

            if (msg == "CANCEL_CONFIRMED")
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

void ClientUDP::SendACK(const sf::IpAddress& ip, unsigned short port, const std::string& ack)
{
    //std::string ack = "ACK_MATCH_FOUND";
    _socket.send(ack.c_str(), ack.size(), ip, port);
}
