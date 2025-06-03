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

//-- Sends every X time to the server FIND_MATCH until an ACK recieved

void ClientUDP::StartMatchSearchWithRetry(std::string matchType)
{
    const int maxRetries = 5;
    int retries = 0;
    bool ackReceived = false;

    // - Sends FIND_MATCH with X retries to the SS

    while (retries < maxRetries && !ackReceived)
    {
        Send(PacketHeader::URGENT, PacketType::FIND_MATCH, matchType, Constants::SERVICE_SERVER_IP.value(), Constants::MATCHMAKING_SERVER_PORT);
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
                // - If I recieve a SEARCH_ACK confirms and Starts Listening For Match
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

    _dispatcher.RegisterHandler(PacketType::MATCH_FINISHED, [this](const RawPacketJob& job) {
        onMatchFinished.Invoke();
        });

    // - Crear uno para CREATE_PLAYER, que invoque un evento como onReconcilePacketRecived.Invoke(packet);

    _dispatcher.RegisterHandler(PacketType::CREATE_PLAYER, [this](const RawPacketJob& job) {
        std::cout << "Create Player Arrived";
        std::vector<CreatePlayerPacket> playersToCreate = CreatePlayerPacket::Deserialize(job.content, playersToCreate);
        onPlayerCreatedRecieved.Invoke(playersToCreate);
        });

    _dispatcher.RegisterHandler(PacketType::CREATE_BULLET, [this](const RawPacketJob& job) {
        CreateBulletPacket packet = CreateBulletPacket::Deserialize(job.content);
        onCreateBullet.Invoke(packet);
        });

    _dispatcher.RegisterHandler(PacketType::DESTROY_BULLET, [this](const RawPacketJob& job) {
        unsigned int bulletID = std::stoul(job.content);
        onDestroyBullet.Invoke(bulletID);
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

void ClientUDP::StartSendingPings(std::atomic<bool>* shouldRun)
{
    std::thread pingThread([this, shouldRun]() {
        std::string msg = std::to_string(currentMatchID);
        while (*shouldRun)
        {
            Send(PacketHeader::NORMAL, PacketType::PING, msg + ":", GetGameServerIP().value(), GetCurrentGameServerPort());

            sf::sleep(sf::seconds(1)); // 1 segundo entre pings
        }
        });

    pingThread.detach();
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
            //_listening = false;

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

            JoinGameServer();

        });

    _dispatcher.RegisterHandler(PacketType::ACK_JOINED, [this](const RawPacketJob& job) {
        std::cout << "[CLIENT] Received ACK_JOINED from server.\n";
        _listening = false;
        _joinedConfirmed = true;
        onMatchFound.Invoke(Constants::NUM_PLAYERS);
        });

    _dispatcher.Start();

    _matchmakingThread = std::thread([this]() {
        while (_listening)
        {
            char buffer[1024];
            std::size_t received = 0;
            std::optional<sf::IpAddress> sender = std::nullopt;
            unsigned short senderPort;

            _socket.setBlocking(false);
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

    Send(PacketHeader::NORMAL, PacketType::CANCEL_SEARCH, "", Constants::SERVICE_SERVER_IP.value(), Constants::MATCHMAKING_SERVER_PORT);

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
    std::cout << "Entering JoinGameServer..." << std::endl;
    if (!_gameServerIp.has_value()) return;

    _joinedConfirmed = false;

    std::string content = std::to_string(currentMatchID) + ":" + std::to_string(currentPlayerID);

    std::thread joinThread([this, content]() {
        const int maxAttempts = 100;
        const sf::Time retryDelay = sf::milliseconds(750);
        int attempts = 0;

        while (!_joinedConfirmed && attempts < maxAttempts) 
        {
            std::cout << "[CLIENT] Sending JOIN_GAME attempt #" << (attempts + 1) << std::endl;

            Send(PacketHeader::URGENT, PacketType::JOIN_GAME, content, GetGameServerIP().value(), GetCurrentGameServerPort());

            sf::sleep(retryDelay);
            attempts++;
        }

        if (!_joinedConfirmed) 
        {
            std::cout << "[CLIENT] Failed to join game server after " << maxAttempts << " attempts.\n";
        }
    });

    joinThread.detach();
}


