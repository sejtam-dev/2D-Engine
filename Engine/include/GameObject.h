#pragma once

#include "Static.h"
#include "Scene.h"
#include "Component.h"
#include "Transform.h"
#include "MethodCalls.h"

#include "glm/vec3.hpp"

#include <vector>

class GameObject : public ObjectCalls {
private:
	std::vector<Component> m_Components;
	Scene* m_Scene;
public:
	Transform m_Transform;

	std::vector<std::string> m_Tags;
};