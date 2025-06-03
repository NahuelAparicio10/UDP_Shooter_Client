#include "HealthComponent.h"

void HealthComponent::GetDamage(int dmg)
{
    _health -= dmg;
    if (_health <= 0)
    {
        _health = 0;
        std::cout << "Player death" << std::endl;
        onPlayerDie.Invoke();
    }
}

const std::type_index HealthComponent::GetType()
{
    return typeid(HealthComponent);
}
