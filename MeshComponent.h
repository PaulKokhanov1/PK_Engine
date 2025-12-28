#ifndef MESH_COMPONENT_CLASS_H
#define MESH_COMPONENT_CLASS_H
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string>
#include<vector>
#include <algorithm>
#include <unordered_map>

#include"LogMesh.h"
#include"Shader.h"
#include"VAO.h"
#include"EBO.h"
#include"Vertex.h"
#include"VertexKey.h"
#include"GLDebug.h"
#include"cyTriMesh.h"
#include"Material.h"
#include"Transform.h"


// Used to create obj from .obj files or creating objects manually
class MeshComponent
{
public:
	// Create obj from .obj file
	MeshComponent(const char* filename);
	// Create obj manually
	MeshComponent(std::string name, std::vector<VERTEX>& vertices, std::vector<GLuint>& indices);
	~MeshComponent();

	// Draw the object
	void Draw();

	// Getters and Setters

	std::string getMeshName() const;
	glm::mat4 getModelMatrix() const;
	Material& getMaterial();	// By reference because we'd want to edit the material information

	void setTransform(Transform transform);

private:

	// Instantiate VAO, VBO and EBO
	void CreateMeshObject();

	std::string meshName;
	std::vector<VERTEX> vertices;
	std::vector<GLuint> indices;
	VAO vao;
	VBO vbo; // currently only storing position
	EBO ebo;

	// Used to create Model Matrix
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Currently used for shading
	Material material;

};

#endif // !MESH_COMPONENT_CLASS_H


#pragma once
