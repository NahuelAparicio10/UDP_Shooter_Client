#pragma once
#include <SFML/Network.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "Constants.h"

class ClientUDP
{
public:
    ClientUDP();
    ~ClientUDP();

    bool Init();
    void SendFindMatch(std::string command);
    std::optional<std::string> ListenForMatch(float timeoutSeconds);
    void SendACK(const sf::IpAddress& ip, unsigned short port);

private:
    sf::UdpSocket _socket;
    //sf::IpAddress _serverIp = "127.0.0.1"; // o desde Constants
    //unsigned short _serverPort = 9100;     // puerto del MatchmakingServer
};

