#pragma once
#include <SFML/Network.hpp>
#include <iostream>

class ClientTCP
{
public:
	ClientTCP();
	~ClientTCP();

	bool ConnectToBootstrapServer(const std::string& ip, unsigned short port);
	void DisconnectFromBootstrapServer();
	bool SendPackage(const std::string& username, const std::string& password, const std::string& command);
	bool SendRegister(const std::string& username, const std::string& password);
	std::optional<sf::Packet> WaitForServerMessage(float timeoutSeconds);

private:
	sf::TcpSocket _bootstrapSocket;
	sf::SocketSelector _selector;
};

