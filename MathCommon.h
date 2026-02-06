#pragma once

#include <cmath>
#include <glm/glm.hpp>

inline bool IsFiniteVec3(const glm::vec3& v)
{
	return std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z);
}

namespace constants {

	// Any variable declared constexpre is implicitly inline
	constexpr glm::vec3 yAxis(1.0f, 0.0f, 0.0f);
	constexpr glm::vec3 xAxis(0.0f, 1.0f, 0.0f);
	constexpr glm::vec3 zAxis(0.0f, 0.0f, 1.0f);
}
