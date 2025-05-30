#include "MapManager.h"

MapManager::MapManager(PhysicsManager* physicisManager)
{
    _physicsManager = physicisManager;
    GenerateGameObjects();
}

std::vector<std::string> MapManager::LoadMapFromFile()
{
    std::ifstream file(Constants::MapFile);
    std::vector<std::string> map;
    std::string line;

    if (!file.is_open()) 
    {
        std::cerr << "[MAP_MANAGER] Couln't open map file" << std::endl;
        return map;
    }

    while (std::getline(file, line)) 
    {
        map.push_back(line);
    }

    file.close();
    return map;
}

void MapManager::GenerateGameObjects()
{
    std::vector<std::string> map = LoadMapFromFile();

    if (map.empty()) return;
    int rows = map.size();
    int cols = map[0].size();
    float tileWidth = Constants::WIDTH / cols;
    float tileHeight = Constants::HEIGHT / rows;


    for (int y = 0; y < rows; ++y)
    {
        int x = 0;
        while (x < cols)
        {
            char tile = map[y][x];

            if (tile == '-')
            {
                // Detecta secuencia de '-'
                int startX = x;
                while (x < cols && map[y][x] == '-') x++;
                int length = x - startX;

                auto* go = new GameObject();
                auto* transform = go->GetComponent<Transform>();
                transform->position = {
                    (startX + length / 2.f) * tileWidth,
                    y * tileHeight + tileHeight / 2.0f
                };

                auto* collider = go->AddComponent<BoxCollider2D>();
                collider->size = { length * tileWidth, tileHeight };

                go->AddComponent<SpriteRenderer>("", sf::Color::White, true, collider->size);
                _mapObjects.push_back(go);
                _physicsManager->Register(go);
            }
            else if (tile == '#')
            {
                // Pared normal
                auto* go = new GameObject();
                auto* transform = go->GetComponent<Transform>();
                transform->position = {
                    x * tileWidth + tileWidth / 2.0f,
                    y * tileHeight + tileHeight / 2.0f
                };

                auto* collider = go->AddComponent<BoxCollider2D>();
                collider->size = { tileWidth, tileHeight };

                go->AddComponent<SpriteRenderer>("", sf::Color::Blue, true, collider->size);
                _mapObjects.push_back(go);
                _physicsManager->Register(go);

                x++;
            }
            else
            {
                x++;
            }
        }
    }
}

void MapManager::Draw(sf::RenderWindow* window)
{
    for (auto* p : _mapObjects)
    {
        p->GetComponent<SpriteRenderer>()->Draw(window, p->GetComponent<Transform>());
    }
}
