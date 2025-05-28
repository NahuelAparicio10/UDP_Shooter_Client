#pragma once
#include <SFML/Network.hpp>
#include <optional>
#include <string>
#include <iostream>
#include "Constants.h"
#include <atomic>
#include <thread>
#include <functional>
#include "Event.h"

class ClientUDP
{
public:
    ClientUDP();
    ~ClientUDP();

    bool Init();
    void SendStringMessage(std::string command, std::optional<sf::IpAddress> ip, unsigned short port);

    void StartListeningForMatch();
    void CancelMatchSearch();

    Event<const std::string&> onMatchFound;
    Event<> onCancelConfirmed;

private:
    sf::UdpSocket _socket;
    std::thread _matchmakingThread;
    std::atomic<bool> _listening = false;

};

