#include "ClientUDP.h"

ClientUDP::ClientUDP()
{
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

// -- Listens for a match incoming message from Service Server on recieved sends a ACK to verify

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
                SendACK(sender.value(), senderPort);
                return msg;
            }
        }
    }
    return std::nullopt;
}

void ClientUDP::SendACK(const sf::IpAddress& ip, unsigned short port)
{
    std::string ack = "ACK_MATCH_FOUND";
    _socket.send(ack.c_str(), ack.size(), ip, port);
}
