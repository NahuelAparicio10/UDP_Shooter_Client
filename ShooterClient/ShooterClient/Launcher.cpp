#include "Launcher.h"

Launcher::Launcher() { }

Launcher::~Launcher() { }

bool Launcher::CheckAndUpdate()
{
    sf::UdpSocket socket;

    // Tries to bind UDP socket 

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[LAUNCHER] Failed to bind UDP socket." << std::endl;
	    return false;
    }

    // If socket binds sends message with version

    std::string message = "VERSION:" + GetLocalVersion();

    if (socket.send(message.c_str(), message.size(), Constants::ServiceServerIP.value(), Constants::VersionCheckerServerPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[LAUNCHER] Failed to send version check." << std::endl;
        return false;
    }

    char buffer[1024];
    std::size_t received = 0;
    std::optional<sf::IpAddress> sender = std::nullopt;
    unsigned short senderPort = 0;

    std::ofstream mapFile(Constants::MapFile, std::ios::trunc);

    if (!mapFile.is_open()) 
    {
        std::cerr << "[LAUNCHER] Couldn't open map file for writing.\n";
        return false;
    }

    bool updateStarted = false;

    while (true)
    {
        if (socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value()) {
            std::string data(buffer, received);

            if (data == "OK") 
            {
                std::cout << "[LAUNCHER] Client is up to date.\n";
                return true;
            }
            else if (data.rfind("UPDATE:", 0) == 0)
            {
                std::string version = data.substr(7);
                SaveLocalVersion(version);
                updateStarted = true;
                std::cout << "[LAUNCHER] Update required to version " << version << ". Receiving map...\n";
            }
            else if (data == "EOF") 
            {
                std::cout << "[LAUNCHER] Update completed successfully.\n";
                break;
            }
            else if (updateStarted) 
            {
                mapFile << data << "\n";
            }
        }
    }

    mapFile.close();

    return true;
}

// - Gets local version from file text

std::string Launcher::GetLocalVersion()
{
    std::ifstream file(Constants::VersionFile);

    if (!file.is_open()) 
    {
        std::cerr << "[LAUNCHER] Couldn't open version file, assuming 0.0" << std::endl;
        return "0.0";
    }

    std::string version;
    std::getline(file, version);
    return version;
}

// - Saves the new version to the file text

void Launcher::SaveLocalVersion(const std::string& version)
{
    std::ofstream file(Constants::VersionFile, std::ios::trunc);

    if (file.is_open()) 
    {
        file << version;
        file.close();
        std::cout << "[LAUNCHER] Saved new version: " << version << std::endl;
    }
    else 
    {
        std::cerr << "[LAUNCHER] Failed to save new version." << std::endl;
    }
}

