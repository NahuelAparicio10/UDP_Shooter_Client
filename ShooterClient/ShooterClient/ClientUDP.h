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
#include "PacketDispatcher.h"
#include <sstream>
class ClientUDP
{
public:
    ClientUDP();
    ~ClientUDP();

    bool Init();
    void Send(PacketHeader header, PacketType type, const std::string& content, sf::IpAddress ip, unsigned short port);

    void StartListeningForMatch();
    void CancelMatchSearch();
    void JoinGameServer();
    void StartMatchSearchWithRetry(std::string matchType);

    void StartReceivingGameplayPackets();
    void StartSendingPings(std::atomic<bool>* shouldRun);
    void StopReceivingGameplayPackets();
    
    Event<const int> onMatchFound;
    Event<> onCancelConfirmed;
    Event<const CreateBulletPacket&> onCreateBullet;
    Event<int> onDestroyBullet;
    Event<MovementPacket> onMovementPacketRecived;
    Event<const MovementPacket&> onReconcilePacketRecived;
    Event<> onMatchFinished;
    Event<int> onPlayerHit;
    Event<std::vector<CreatePlayerPacket>> onPlayerCreatedRecieved;
    //Event <const CreatePlayerPacket&> onPlayerCreated


    std::optional< sf::IpAddress> GetGameServerIP() { return _gameServerIp; }
    unsigned short GetCurrentGameServerPort() { return _gameServerPort; }
    unsigned int currentMatchID;
    unsigned int currentPlayerID;

private:
    sf::UdpSocket _socket;
    std::thread _matchmakingThread;
    std::atomic<bool> _joinedConfirmed = false;
    std::atomic<bool> _receivingGameplay = false;
    std::atomic<bool> _listening = false;
    PacketDispatcher _dispatcher;
    std::optional< sf::IpAddress> _gameServerIp;
    unsigned short _gameServerPort;

};

