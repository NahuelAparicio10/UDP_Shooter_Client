#pragma once
#include "ClientTCP.h"
#include "ClientUDP.h"
class NetworkManager
{
public:
	static NetworkManager& GetInstance();
	void Initialize();

	ClientTCP* GetTCPClient();
	ClientUDP* GetUDPClient();

private:
	NetworkManager();
	~NetworkManager();

	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;

	ClientTCP* _clientTCP;
	ClientUDP* _clientUDP;
};

