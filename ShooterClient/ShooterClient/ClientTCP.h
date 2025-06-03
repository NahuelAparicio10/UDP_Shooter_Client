#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include "Event.h"
#include "Constants.h"
class ClientTCP
{
public:
	ClientTCP();
	~ClientTCP();

	bool ConnectToBootstrapServer();
	void DisconnectFromBootstrapServer();
	bool SendPackage(const std::string& username, const std::string& password, const std::string& command);
	void LoginRegister(const std::string& command, const std::string& username, const std::string& password);
	std::optional<sf::Packet> WaitForServerMessage(float timeoutSeconds);
	Event<> onLoginSuccess;

private:
	sf::TcpSocket _bootstrapSocket;
	sf::SocketSelector _selector;
};

