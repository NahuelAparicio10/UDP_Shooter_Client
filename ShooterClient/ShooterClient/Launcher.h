#pragma once
#include "Constants.h"
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <optional>
#include "NetworkDefs.h" 
#include "PacketDispatcher.h"

class Launcher
{
public:
	Launcher();
	~Launcher();
	bool CheckAndUpdate();

private:
	std::string GetLocalVersion();
	void SaveLocalVersion(const std::string& version);
};

