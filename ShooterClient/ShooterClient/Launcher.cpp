#include "Launcher.h"

Launcher::Launcher() { }

Launcher::~Launcher() { }

bool Launcher::CheckAndUpdate()
{   
    sf::UdpSocket socket;

    if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Status::Done)
    {
        std::cerr << "[LAUNCHER] Failed to bind UDP socket." << std::endl;
        return false;
    }

    // - Sends to Service Server his version to make the check control

    SendDatagram(socket, PacketHeader::URGENT, PacketType::VERSION, GetLocalVersion(), Constants::SERVICE_SERVER_IP.value(), Constants::VERSION_CHECKER_SERVER_PORT);

    char buffer[1024];
    std::size_t received = 0;
    std::optional<sf::IpAddress> sender = std::nullopt;
    unsigned short senderPort = 0;

    bool mapUpdated = false;

    while (true)
    {
        if (socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Status::Done && sender.has_value())
        {
            RawPacketJob job;
            
            // - If receives packet and can be parsed in Raw Datagram checks for packet type

            if (ParseRawDatagram(buffer, received, job, sender.value(), senderPort))
            {
                switch (job.type)
                {
                    case PacketType::OK: // - If its ok when continue to Login.
                        std::cout << "[LAUNCHER] Client is up to date.\n";
                        return true;

                    case PacketType::UPDATE: // - If needs update, saves the new version and wait for UPDATE_MAP
                        SaveLocalVersion(job.content);
                        std::cout << "[LAUNCHER] Update required to version " << job.content << ". Receiving map...\n";
                        break;
                    case PacketType::UPDATE_MAP: // - if recieves an update map we save the incoming map into the file
                    {
                        std::ofstream mapFile(Constants::MapFile, std::ios::trunc);
                        if (mapFile.is_open()) 
                        {
                            mapFile << job.content;
                            std::cout << "[LAUNCHER] Map updated successfully.\n";
                        }
                        else {
                            std::cerr << "[LAUNCHER] Failed to save updated map.\n";
                        }
                        mapUpdated = true;
                        mapFile.close();

                        break;
                    }
                    default:
                        break;
                }
            }
        }
        if (mapUpdated)
        {
            break;
        }

    }

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

