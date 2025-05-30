#pragma once
#include <string>
#include "SFML/Network.hpp"

class Constants
{
public:
	inline static constexpr int WIDTH = 1080;
	inline static constexpr int HEIGHT = 720;
	
	#pragma region Paths
		inline static const std::string VersionFile = "Config/version.txt";
		inline static const std::string MapFile = "Maps/map_v0_0.txt";
	#pragma endregion 

	#pragma region IP / Ports
		inline static const std::optional<sf::IpAddress> ServiceServerIP = sf::IpAddress::resolve("127.0.0.1");
		inline static const unsigned short VersionCheckerServerPort = 9000;
		inline static const unsigned short MatchMakingServerPort = 9100;
	#pragma endregion 

};