#prgma once

#include "Static.h"

#include "glm/vec3.hpp"

#include <vector>

class Transform {
private:
public:
	glm::vec3 m_Position = { 0, 0, 0 };
	glm::vec3 m_Rotation = { 0, 0, 0 };
	glm::vec3 m_Scale = { 1, 1, 1 };

	std::vector<std::string> m_Tags;
};