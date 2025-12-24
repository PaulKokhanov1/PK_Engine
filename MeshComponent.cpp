#include"MeshComponent.h"


// ADD REMAINING PARTS OF POSSIBLE OBJ FILE COMPOENENTS, ex: textures
MeshComponent::MeshComponent(const char* filename, std::string shaderName) : shaderName(shaderName)
{
	// Using Cem Yuksel's obj file parsing
	cyTriMesh mesh;

	// Redundant error check on file load
	if (!mesh.LoadFromFileObj(filename)) {
		LogMeshError("Failed to load OBJ: " + filename);
		return;
	}


	std::cout << "Loaded OBJ: " << filename <<std::endl;
	std::cout << "Vertices: " << mesh.NV() << "\n";
	std::cout << "Normals:  " << mesh.NVN() << "\n";
	std::cout << "Faces:    " << mesh.NF() << "\n";

	 //Compute Normals if no normals are specified
	if (mesh.NVN() == 0) {
		LogMeshInfo("No normals found - computing per-vertex normals.");
		mesh.ComputeNormals();
	}


	for (int i = 0; i < std::min(5, (int) mesh.NV()); ++i) {
		auto v = mesh.V(i);
		auto n = mesh.VN(i);
		std::cout << "Vertex " << i << ": Pos("
			<< v.x << ", " << v.y << ", " << v.z << ")  "
			<< "Norm("
			<< n.x << ", " << n.y << ", " << n.z << ")\n";
	}

	for (int i = 0; i < std::min(3, (int)mesh.NF()); ++i) {
		auto f = mesh.F(i);
		std::cout << "Face " << i << ": "
			<< f.v[0] << ", "
			<< f.v[1] << ", "
			<< f.v[2] << "\n";
	}

	for (int i = 0; i < std::min(5, (int)mesh.NF()); ++i) {
		auto f = mesh.F(i);
		auto fn = mesh.FN(i);
		std::cout << "Face " << i << " position indices:  "
			<< f.v[0] << "," << f.v[1] << "," << f.v[2]
			<< "  | normal indices: "
			<< fn.v[0] << "," << fn.v[1] << "," << fn.v[2] << "\n";
	}

	// Handle duplciation of vertices if resued attributes mixed with seperate attributes
	// Ex; vertex {pos1, norm1} then later vertex {pos1, norm2} need to duplicate so ebo doesn't reuse old vertex
	std::unordered_map<VertexKey, long> newVertexIndex;

	// Handling only position and normal for NOW
	// Also assuming each face will have a position AND a normal assigned, maybe handle this after you finish implementation
	for (int i = 0; i < mesh.NF(); ++i) {
		auto f = mesh.F(i);
		auto fn = mesh.FN(i);
		for (int j = 0; j < 3; ++j) {
			VertexKey key = VertexKey{ f.v[j], fn.v[j] };

			if (newVertexIndex.find(key) == newVertexIndex.end()) {
				// Create new Vertex
				newVertexIndex[key] = vertices.size();

				// Add it to vertices
				glm::vec3 pos = glm::vec3(mesh.V(f.v[j]).x, mesh.V(f.v[j]).y, mesh.V(f.v[j]).z);
				glm::vec3 norm = glm::vec3(mesh.VN(fn.v[j]).x, mesh.VN(fn.v[j]).y, mesh.VN(fn.v[j]).z);
				vertices.push_back(VERTEX{ pos, norm });
			}

			// Add either old or new vertex index to EBO, always pushing back because each face vertex needs to be represented in our EBO
			indices.push_back(newVertexIndex[key]);
		}

	}

	/*std::cout << "\n[MeshComponent] Dumping VertexKey - NewIndex map:\n";
	for (const auto& entry : newVertexIndex) {
		const VertexKey& key = entry.first;
		uint32_t newIndex = entry.second;

		std::cout << "  Key(pos=" << key.pos
			<< ", norm=" << key.normal
			<< ") - newVertexIndex = " << newIndex << "\n";
	}*/


	std::cout << "\n[MeshComponent] Final mesh:\n";
	std::cout << "  Unique vertices created: " << vertices.size() << "\n";
	std::cout << "  Indices:                 " << indices.size() << "\n";


	// Center object using its bounding box, because not all obj files are centered around origin
	mesh.ComputeBoundingBox();
	cyVec3f minBoundcy = mesh.GetBoundMin();
	cyVec3f maxBoundcy = mesh.GetBoundMax();
	cyVec3f center = (minBoundcy + maxBoundcy) * 0.5f;	// Division is heavy so instead multiply
	cyVec3f size = maxBoundcy - minBoundcy;

	if (size.Length() == 0.0f) {
		LogMeshWarn("Warning: Zero-size bounding box — skipping normalization.");
		return;
	}

	float scaleFactor = 1.0f / size.Length();

	glm::vec3 centerGLM(center.x, center.y, center.z);
	for (auto& v : vertices) {
		v.position = (v.position - centerGLM) * scaleFactor;
	}

	// Initialize remaining member variables
	this->meshName = filename;
	CreateMeshObject();
	

}

MeshComponent::MeshComponent(std::string name, std::vector<VERTEX>& vertices, std::vector<GLuint>& indices, std::string shaderName) : meshName(name), vertices(vertices), indices(indices), shaderName(shaderName)
{
	if (vertices.empty() || indices.empty()) {
		LogMeshError("Empty vertices or indices for mesh: " + name);
	}

	CreateMeshObject();
}

MeshComponent::~MeshComponent()
{
	// Avoid memory leak 
	vbo.Delete();
	ebo.Delete();
	vao.Delete();

}

void MeshComponent::Draw()
{
	vao.Bind();

	// Draw mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

std::string MeshComponent::getShaderName() const
{
	return this->shaderName;
}

std::string MeshComponent::getMeshName() const
{
	return this->meshName;
}

// Manually creating model matrix at the moment
glm::mat4 MeshComponent::getModelMatrix() const
{
	// Must return in order: translation * rotation * scale
	return trans * rot * sca;
}

Material& MeshComponent::getMaterial()
{
	return material;
}

void MeshComponent::CreateMeshObject()
{
	vao.Bind();
	GL_CHECK_ERROR();	// Macro to ensure binding of VAO is properly done

	vbo.Construct(vertices);
	ebo.Construct(indices);

	// Link attributes to VAO
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(VERTEX), (void*)0); // Eventually change THIS TO NOT MANUALLY, currently only processign a position
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(VERTEX), (void*)(3 * sizeof(float)));

	// Unbind all buffer's, MUST unbind EBO AFTER VAO
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}
