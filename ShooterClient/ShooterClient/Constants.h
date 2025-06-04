#pragma once
#include <string>
#include "SFML/Network.hpp"

class Constants
{
public:
	#pragma region Configs
		inline static constexpr int WIDTH = 1080;
		inline static constexpr int HEIGHT = 720;
		inline static constexpr int NUM_PLAYERS = 2;
		inline static constexpr float FIXED_DT = 0.033f;
	#pragma endregion

	#pragma region Paths
		inline static const std::string VersionFile = "Config/version.txt";
		inline static const std::string MapFile = "Maps/map_v0_0.txt";
	#pragma endregion 

	#pragma region IP / Ports
		inline static const std::optional<sf::IpAddress> SERVICE_SERVER_IP = sf::IpAddress::resolve("127.0.0.1");
		inline static const unsigned short VERSION_CHECKER_SERVER_PORT = 9000;
		inline static const unsigned short MATCHMAKING_SERVER_PORT = 9100;

		inline static const unsigned short LOGIN_REGISTER_SERVER_PORT = 50500;
	#pragma endregion 

};