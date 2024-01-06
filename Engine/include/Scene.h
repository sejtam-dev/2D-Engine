#pragma once

#include "Static.h"

#include "GameObject.h"
#include "MethodCalls.h"

#include <vector>

class Scene : public SceneCalls {

private:
	std::vector<GameObject> objects;

public:

};