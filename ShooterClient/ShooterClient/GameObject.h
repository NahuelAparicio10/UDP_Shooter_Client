#pragma once
#include <vector>
#include "Component.h"
#include "Transform.h"

// Making GO as ECS (simplified) like UNity :')

class GameObject
{
public:
	GameObject();
	~GameObject();

	template<typename T, typename ...Args>
	T* AddComponent(Args && ...args)
	{
		T* component = new T(std::forward<Args>(args)...);
		this->components.push_back(component);
		return component;
	}

	template <typename T> T* GetComponent()
	{
		std::type_index typeIndex = typeid(T);

		for (auto& component : components)
		{
			if (component && component->GetType() == typeIndex)
			{
				return static_cast<T*>(component);
			}
		}
		return nullptr;
	}

	std::vector<Component*> components;
};

