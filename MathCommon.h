#pragma once

#include <cmath>
#include <glm/glm.hpp>

inline bool IsFiniteVec3(const glm::vec3& v)
{
	return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}
