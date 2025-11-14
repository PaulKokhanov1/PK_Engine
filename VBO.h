#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<iostream>
#include<vector>
#include"Vertex.h"

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Default constructor to create VBO's to default values
	VBO();
	~VBO();

	// Generates a Vertex Buffer Object and links it to vertices
	void Construct(std::vector<VERTEX>& vertices);
	// Binds the VBO
	void Bind();
	// Unbinds the VBOz
	void Unbind();
	// Deletes the VBO
	void Delete();
}; 

#endif