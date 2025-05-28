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
    void SendFindMatch(std::string command);
    std::optional<std::string> ListenForMatch(float timeoutSeconds);

    void StartListeningForMatch();
    void CancelMatchSearch();

    Event<const std::string&> onMatchFound;
    Event<> onCancelConfirmed;

private:
    sf::UdpSocket _socket;
    std::thread _matchmakingThread;
    std::atomic<bool> _listening = false;

    void SendACK(const sf::IpAddress& ip, unsigned short port, const std::string& ack);
};

