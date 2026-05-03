#pragma once

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

struct SubMesh {
	Material material;
	uint32_t indexStart;
	uint32_t indexCount;
};

// Used to create obj from .obj files or creating objects manually
class MeshComponent
{
public:
	// Create obj from .obj file
	MeshComponent(const char* filename, bool centerTheMesh = true);
	// Create obj manually
	MeshComponent(std::string name, std::vector<VERTEX>& vertices, std::vector<GLuint>& indices);
	~MeshComponent();

	// Draw the object
	void DrawSubMesh(const SubMesh& s);

	// Setters and Getters

	void setTransform(Transform transform);

	std::string getMeshName() const;
	std::vector<SubMesh>& getSubMeshes();	// By reference because we'd want to edit the material information
	Transform& getTransform();


	// Used to create Model Matrix
	Transform transform;
	glm::mat4 computeModelMatrix();


private:

	// Reading OBJ support methods
	void buildVertices(cyTriMesh& mesh);
	void buildSubMeshes(cyTriMesh& mesh);
	void centerMesh(cyTriMesh& mesh);

	// Instantiate VAO, VBO and EBO
	void CreateMeshObject();

	// Debug method
	void debugMeshInfo(cyTriMesh& mesh);

	std::string meshName;
	std::vector<VERTEX> vertices;	// All vertices, MeshGeometry then takes specific index range from it
	std::vector<GLuint> indices;
	VAO vao;
	VBO vbo; // currently only storing position
	EBO ebo;

	// Hold all submeshes that may have seperate textures/materials, store object for better locality rather than pointers
	std::vector<SubMesh> submeshes;

	// filepath for obj location
	std::string filepath = "";

	// Function used for finding folder path of related OBJ, MTL and PNG files related to mess
	std::string removeLastWord(std::string filename);

	const unsigned MISSING_INDEX = std::numeric_limits<unsigned>::max();

};

