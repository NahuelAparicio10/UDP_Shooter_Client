#include "ClientTCP.h"

ClientTCP::ClientTCP()
{
}

ClientTCP::~ClientTCP()
{
    DisconnectFromBootstrapServer();
}

bool ClientTCP::ConnectToBootstrapServer(const std::string& ip, unsigned short port)
{
    auto resolved = sf::IpAddress::resolve(ip);

    if (_bootstrapSocket.connect(*resolved, port) != sf::Socket::Status::Done)
    {
        std::cout << "[CLIENT] Failed to connect to bootstrap server." << std::endl;
        return false;
    }

    _bootstrapSocket.setBlocking(false);
    _selector.add(_bootstrapSocket);

    std::cout << "[CLIENT] Connected to bootstrap server ip " + ip << std::endl;
    return true;
}

void ClientTCP::DisconnectFromBootstrapServer() { _bootstrapSocket.disconnect(); }

bool ClientTCP::SendPackage(const std::string& username, const std::string& password, const std::string& command)
{
    std::cout << "[CLIENT] "<< command << " sended with user " << username << " and password " << password << std::endl;
    sf::Packet packet;
    packet << command << username << password;
    return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
}

bool ClientTCP::SendRegister(const std::string& username, const std::string& password)
{
    //std::cout << "[CLIENT] Register sended with user " << username << " and password " << password << std::endl;

    //sf::Packet packet;
    //packet << "REGISTER" << username << password;
    //return _bootstrapSocket.send(packet) == sf::Socket::Status::Done;
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


