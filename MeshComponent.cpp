#include"MeshComponent.h"


// ADD REMAINING PARTS OF POSSIBLE OBJ FILE COMPOENENTS, ex: textures
MeshComponent::MeshComponent(const char* filename)
{
	// Using Cem Yuksel's obj file parsing
	cyTriMesh mesh;

	// Redundant error check on file load
	if (!mesh.LoadFromFileObj(filename)) {
		LogMeshError("Failed to load OBJ: " + filename);
		return;
	}

	// create filepath
	filepath = removeLastWord(filename);
	cout << "FILEPATH: " << filepath << endl;

	std::cout << "Loaded OBJ: " << filename <<std::endl;
	std::cout << "Vertices: " << mesh.NV() << "\n";
	std::cout << "Normals:  " << mesh.NVN() << "\n";
	std::cout << "Tex Coords:  " << mesh.NVT() << "\n";
	std::cout << "Faces:    " << mesh.NF() << "\n";
	std::cout << "Materials:    " << mesh.NM() << "\n";
	std::cout << "Material Index:    " << mesh.GetMaterialIndex(3) << "\n";
	std::cout << "Faces for material 0:		" << mesh.GetMaterialFaceCount(0) << "\n";

	 //Compute Normals if no normals are specified
	if (mesh.NVN() == 0) {
		LogMeshInfo("No normals found - computing per-vertex normals.");
		mesh.ComputeNormals();
	}


	for (int i = 0; i < std::min(5, (int) mesh.NV()); ++i) {
		auto v = mesh.V(i);
		auto n = mesh.VN(i);
		auto t = mesh.VT(i);
		std::cout << "Vertex " << i << ": Pos("
			<< v.x << ", " << v.y << ", " << v.z << ")  "
			<< "Norm("
			<< n.x << ", " << n.y << ", " << n.z << ")  "
			<< "Tex("
			<< t.x << ", " << t.y << ", " << t.z << ")\n";
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
		auto ft = mesh.FT(i);
		std::cout << "Face " << i << " position indices:  "
			<< f.v[0] << "," << f.v[1] << "," << f.v[2]
			<< "  | normal indices: "
			<< fn.v[0] << "," << fn.v[1] << "," << fn.v[2] << 
			"  | texture indices: "
			<< ft.v[0] << "," << ft.v[1] << "," << ft.v[2] << "\n";
	}

	// Handle duplciation of vertices if resued attributes mixed with seperate attributes
	// Ex; vertex {pos1, norm1, tex1} then later vertex {pos1, norm2, tex1} need to duplicate so ebo doesn't reuse old vertex
	buildVertices(mesh);

	/*std::cout << "\n[MeshComponent] Dumping VertexKey - NewIndex map:\n";
	for (const auto& entry : newVertexIndex) {
		const VertexKey& key = entry.first;
		uint32_t newIndex = entry.second;

		std::cout << "  Key(pos=" << key.pos
			<< ", norm=" << key.normal
			<< ") - newVertexIndex = " << newIndex << "\n";
	}*/

	buildSubMeshes(mesh);

	std::cout << "\n[MeshComponent] Final mesh:\n";
	std::cout << "  Unique vertices created: " << vertices.size() << "\n";
	std::cout << "  Indices:                 " << indices.size() << "\n";

	// Center Mesh using Bounding Box
	centerMesh(mesh);

	// Initialize remaining member variables
	this->meshName = filename;
	CreateMeshObject();

}

MeshComponent::MeshComponent(std::string name, std::vector<VERTEX>& vertices, std::vector<GLuint>& indices) : meshName(name), vertices(vertices), indices(indices)
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

void MeshComponent::DrawSubMesh(SubMesh& s)
{
	vao.Bind();

	glDrawElements(GL_TRIANGLES, s.indexCount, GL_UNSIGNED_INT, (void*)(s.indexStart * sizeof(uint32_t)));

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

std::vector<SubMesh>& MeshComponent::getSubMeshes()
{
	return submeshes;
}

void MeshComponent::setTransform(Transform transform)
{
	trans = glm::translate(trans, transform.translation);
	rot = glm::mat4_cast(transform.rotation);
	sca = glm::scale(sca, transform.scale);
}

void MeshComponent::buildVertices(cyTriMesh& mesh)
{
	std::unordered_map<VertexKey, long> newVertexIndex;

	// Handling only position, normal and tex Coords
	// Also assuming each face will have a position, a normal AND a texCoord assigned, maybe handle this after you finish implementation
	for (int i = 0; i < mesh.NF(); ++i) {
		auto f = mesh.F(i);
		auto fn = mesh.FN(i);
		auto ft = mesh.FT(i);
		for (int j = 0; j < 3; ++j) {
			// Using the indices are the faces to make a vertex Key so no need to worry that tex coord is 2D
			VertexKey key = VertexKey{ f.v[j], fn.v[j], ft.v[j] };

			if (newVertexIndex.find(key) == newVertexIndex.end()) {
				// Create new Vertex
				newVertexIndex[key] = vertices.size();

				// Add it to vertices
				glm::vec3 pos = glm::vec3(mesh.V(f.v[j]).x, mesh.V(f.v[j]).y, mesh.V(f.v[j]).z);
				glm::vec3 norm = glm::vec3(mesh.VN(fn.v[j]).x, mesh.VN(fn.v[j]).y, mesh.VN(fn.v[j]).z);
				glm::vec2 tex = glm::vec2(mesh.VT(ft.v[j]).x, mesh.VT(ft.v[j]).y);

				vertices.push_back(VERTEX{ pos, norm, tex });
			}

			// Add either old or new vertex index to EBO, always pushing back because each face vertex needs to be represented in our EBO
			indices.push_back(newVertexIndex[key]);
		}

	}
}

void MeshComponent::buildSubMeshes(cyTriMesh& mesh)
{
	// Handle creating submeshes
	uint32_t idxOffset = 0;
	for (int i = 0; i < mesh.NM(); ++i) {

		// Create new submesh
		SubMesh subMesh;

		subMesh.indexStart = idxOffset;

		idxOffset += (mesh.GetMaterialFaceCount(i) * 3);
		subMesh.indexCount = mesh.GetMaterialFaceCount(i) * 3;

		// Set material for Submesh

		// Get Material properties, assuming 1 material per obj AT THE MOMENT and 1 material per each face
		glm::vec3 ka = glm::vec3{ mesh.M(i).Ka[0], mesh.M(i).Ka[1], mesh.M(i).Ka[2] };
		glm::vec3 kd = glm::vec3{ mesh.M(i).Kd[0], mesh.M(i).Kd[1], mesh.M(i).Kd[2] };
		glm::vec3 ks = glm::vec3{ mesh.M(i).Ks[0], mesh.M(i).Ks[1], mesh.M(i).Ks[2] };
		const char* map_ka = mesh.M(i).map_Ka;
		const char* map_kd = mesh.M(i).map_Kd;
		const char* map_ks = mesh.M(i).map_Ks;

		// Send to material
		subMesh.material.setAttributes(ka, kd, ks, 100.0f);

		// Set and Load texture(s) in material
		subMesh.material.setTextureNames(map_ka, map_kd, map_ks);
		subMesh.material.loadTextures(filepath.c_str());

		submeshes.push_back(subMesh);
	}
}

void MeshComponent::centerMesh(cyTriMesh& mesh)
{
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
}

void MeshComponent::CreateMeshObject()
{
	vao.Bind();
	GL_CHECK_ERROR();	// Macro to ensure binding of VAO is properly done

	vbo.Construct(vertices);
	ebo.Construct(indices);

	// Link attributes to VAO
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(VERTEX), (void*)0); // Eventually change THIS TO NOT MANUALLY
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(VERTEX), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(VERTEX), (void*)(6 * sizeof(float)));


	// Unbind all buffer's, MUST unbind EBO AFTER VAO
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

std::string MeshComponent::removeLastWord(std::string filename)
{
	size_t lastDelimiterPos = filename.find_last_of('/');
	if (lastDelimiterPos == std::string::npos) {
		return "";
	}

	filename.erase(lastDelimiterPos + 1);
	return filename;
}
