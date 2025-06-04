#pragma once
#include "Component.h"
#include "Event.h"
class HealthComponent : public Component
{
public:
	void GetDamage(int dmg);	
	int GetCurrentHealth() { return _health; }
	virtual const std::type_index GetType() override;
	Event<> onPlayerDie;
private:
	int _health = 15;
};

