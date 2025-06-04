#include "BulletHandler.h"

BulletHandler::BulletHandler(PhysicsManager* pManager) : _physicsManager(pManager) 
{ 
    
}

void BulletHandler::DestroyBulletByID(int ID)
{
    auto it = _bulletById.find(ID);
    if (it == _bulletById.end()) return;

    GameObject* bullet = it->second;

    _bullets.erase(std::remove(_bullets.begin(), _bullets.end(), bullet), _bullets.end());

    _bulletById.erase(it);

    delete bullet;
}

void BulletHandler::CreateBullet(int bulletID, sf::Vector2f position, sf::Vector2f direction)
{
    auto* bullet = new GameObject();
    bullet->transform->position = position;
    bullet->AddComponent<SpriteRenderer>("Assets/Sprites/bullet.png", sf::Color::White, true);

    sf::Vector2u size = bullet->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();

    auto* rb = bullet->AddComponent<Rigidbody2D>();
    rb->applyGravity = false;
    rb->velocity = direction * 400.f;
    bullet->id = bulletID;

    _bullets.push_back(bullet);
    _bulletById[bulletID] = bullet;
}

void BulletHandler::HandleCreateBulletPacket(const CreateBulletPacket& packet)
{
    // - Check for the closest bullet ghost
    int bestGhostID = -99999;
    float bestDist = 99999.f;
    sf::Vector2f bestPos = packet.position;

    for (const auto& b : _bullets)
    {
        if (b->id >= 0) continue; 

        float dist = UtilsMaths::Distance(b->transform->position, packet.position);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestGhostID = b->id;
            bestPos = b->transform->position;
        }
    }

    // - Deletes ghost bullet
    if (bestGhostID != -99999)
        DestroyBulletByID(bestGhostID);

    // - Create the oficial one (pos visual = last)
    CreateBullet(packet.bulletID, bestPos, packet.direction);

}

void BulletHandler::UpdateBullets(float dt)
{
    if (_bullets.empty()) return;

	for (auto* b : _bullets)
	{
        b->GetComponent<Rigidbody2D>()->Update(b->transform, dt);
	}
}

void BulletHandler::RenderBullets(sf::RenderWindow* window)
{
    if (_bullets.empty()) return;

	for (auto* b : _bullets)
	{
		b->GetComponent<SpriteRenderer>()->Draw(window, b->transform);
	}
}
