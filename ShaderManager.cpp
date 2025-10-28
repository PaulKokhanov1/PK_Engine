#include"ShaderManager.h"

shaderManager::shaderManager()
{
}

shaderManager::~shaderManager()
{
}

void shaderManager::load(std::string programName, const char* vertexShader, const char* fragmentShader)
{
	Shader newShader(vertexShader, fragmentShader);
	shaderPrograms[programName] = newShader;
}

bool shaderManager::remove(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		std::cerr << "[ShaderManager] Error: Shader program '" << programName << "' not found.\n";
		return false;
	}

	shaderPrograms.erase(programName);
	return true;
}

bool shaderManager::bind(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		std::cerr << "[ShaderManager] Error: Shader program '" << programName << "' not found.\n";
		return false;
	}
	shaderPrograms[programName].Activate();
	return true;
}

Shader* shaderManager::get(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		std::cerr << "[ShaderManager] Error: Shader program '" << programName << "' not found.\n";
		return nullptr;
	}
	return &shaderPrograms[programName];
}
