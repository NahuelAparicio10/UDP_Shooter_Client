#include "NetworkManager.h"

NetworkManager& NetworkManager::GetInstance()
{
	static NetworkManager instance;
	return instance;
}

NetworkManager::NetworkManager()
{
	_clientTCP = new ClientTCP();
	_clientUDP = new ClientUDP();
}

NetworkManager::~NetworkManager()
{
	delete _clientTCP;
	delete _clientUDP;
}

void NetworkManager::Initialize() { }

ClientTCP* NetworkManager::GetTCPClient() {	return _clientTCP; }

ClientUDP* NetworkManager::GetUDPClient() {	return _clientUDP; }

