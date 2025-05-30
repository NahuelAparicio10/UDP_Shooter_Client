#include "PhysicsManager.h"

// -- Registers a new Collider to colliders physic manager



void PhysicsManager::Register(GameObject* go)
{
	if (go->GetComponent<BoxCollider2D>())
	{
		_colliders.push_back(go);
	}
}

void PhysicsManager::UnRegister(GameObject* go)
{
    auto it = std::find(_colliders.begin(), _colliders.end(), go);

    if (it != _colliders.end())
    {
        _colliders.erase(it);
    }
}

void PhysicsManager::Update(float dt)
{
    for (size_t i = 0; i < _colliders.size(); ++i)
    {
        auto a = _colliders[i];
        auto colA = a->GetComponent<BoxCollider2D>();
        auto transA = a->GetComponent<Transform>();

        for (size_t j = i + 1; j < _colliders.size(); ++j)
        {
            auto b = _colliders[j];
            auto colB = b->GetComponent<BoxCollider2D>();
            auto transB = b->GetComponent<Transform>();

            if (colA && colB && transA && transB)
            {
                if (colA->GetBounds(transA).findIntersection(colB->GetBounds(transB)))
                {
                    // - Invokes collision event on collider 2D
                    colA->OnCollisionEnter.Invoke(b);
                    colB->OnCollisionEnter.Invoke(a);
                }
            }
        }
    }
}
