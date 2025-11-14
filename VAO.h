#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	/*
	Link attribute in VBO to VAO
	layout -> location where the this attribute is stored in the program specific to the activate shader
	numComponents -> number of data pieces per vertex
	type -> data type of each data piece in VBO
	stride -> num of bytes between ending of attrib data one vertex to next
	offset -> offset from the start of the incoming data to the specific attribute
	*/
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();
};
#endif
