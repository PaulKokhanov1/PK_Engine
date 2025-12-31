#ifndef VERTEX_H
#define VERTEX_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

struct VERTEX
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

};
#endif // !VERTEX_H

