#pragma once
#include "Constants.h"
#include <SFML/Network.hpp>
#include <fstream>
#include <iostream>
#include <optional>

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

