#include "ClientTCP.h"

ClientTCP::ClientTCP()
{
}

ClientTCP::~ClientTCP()
{
    DisconnectFromBootstrapServer();
}

bool ClientTCP::ConnectToBootstrapServer()
{

    if (_bootstrapSocket.connect(Constants::SERVICE_SERVER_IP.value(), Constants::LOGIN_REGISTER_SERVER_PORT) != sf::Socket::Status::Done)
    {
        std::cout << "[CLIENT_TCP] Failed to connect to bootstrap server." << std::endl;
        return false;
    }

    _bootstrapSocket.setBlocking(false);
    _selector.add(_bootstrapSocket);

    std::cout << "[CLIENT_TCP] Connected to bootstrap server ip " + Constants::SERVICE_SERVER_IP.value().toString() << std::endl;
    return true;
}

void ClientTCP::DisconnectFromBootstrapServer() { _bootstrapSocket.disconnect(); }

// - Last SendPackage
bool ClientTCP::SendPackage(const std::string& username, const std::string& password, const std::string& command)
{
    std::cout << "[CLIENT_TCP] "<< command << " sended with user " << username << " and password " << password << std::endl;
    sf::Packet packet;
    packet << command << username << password;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}


// -- Rework sendpackage to handle all
void ClientTCP::LoginRegister(const std::string& command, const std::string& username, const std::string& password)
{
    if (!SendPackage(username, password, command))
    {
        std::cout << "[CLIENT_TCP] Error sending " + command << std::endl;
        return;
    }

    auto responseOpt = WaitForServerMessage(2.f);

    if (!responseOpt.has_value())
    {
        std::cout << "[CLIENT_TCP] Timeout waiting for server response." << std::endl;
        return;
    }

    std::string reply;
    responseOpt->operator>>(reply);

    if (reply == command + "_OK")
    {
        if (command == "LOGIN")
        {
            onLoginSuccess.Invoke();
        }
        else 
        {
            std::cout << "[CLIENT_TCP] Registered succesfully." << std::endl;
        }
    }
    else
    {
        std::cout <<  "[" + command + "] Server rejected: " + reply << std::endl;
    }
}

std::optional<sf::Packet> ClientTCP::WaitForServerMessage(float timeoutSeconds)
{
    if (_selector.wait(sf::seconds(timeoutSeconds)) && _selector.isReady(_bootstrapSocket))
    {
        sf::Packet packet;
        if (_bootstrapSocket.receive(packet) == sf::Socket::Status::Done)
        {
            return packet;
        }
    }
    return std::nullopt;
}


