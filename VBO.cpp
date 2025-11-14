#include "VBO.h"

VBO::VBO()
{
	ID = 0;
}

VBO::~VBO()
{
	Delete();
}

void VBO::Construct(std::vector<VERTEX>& vertices)
{

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VERTEX), vertices.data(), GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[OpenGL] Error creating VBO (code " << error << ")\n";
	}

	// No need to unbind as VAO will handle usage within main rendering

}
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
