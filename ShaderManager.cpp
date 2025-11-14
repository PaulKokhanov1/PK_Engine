#include"ShaderManager.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::load(std::string programName, const char* vertexShader, const char* fragmentShader)
{
	try {
		shaderPrograms[programName] = std::make_unique<Shader>(vertexShader, fragmentShader);

		if (!shaderPrograms[programName]->isValid()) {
			std::cerr << "[ShaderManager] Error: Invalid shader created.\n";
		}
	}
	catch (const ShaderException& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

}

bool ShaderManager::remove(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		std::cerr << "[ShaderManager] Error: Shader program '" << programName << "' not found.\n";
		return false;
	}

	shaderPrograms.erase(programName);
	return true;
}

bool ShaderManager::bind(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		std::cerr << "[ShaderManager] Error: Shader program '" << programName << "' not found.\n";
		return false;
	}
	shaderPrograms[programName]->Activate();
	return true;
}

Shader* ShaderManager::get(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		std::cerr << "[ShaderManager] Error: Shader program '" << programName << "' not found.\n";
		return nullptr;
	}
	return shaderPrograms[programName].get();
}

void ShaderManager::reloadAll()
{

	// Iterate through all current shader's
	for (auto& sh : shaderPrograms) {

		// Create new shader, using same shader files as previous
		try {
			shaderPrograms[sh.first] = std::make_unique<Shader>(sh.second->getVertexFile(), sh.second->getFragmentFile());	// Keep same program name

			std::cout << "Sucess recompiling Shader: " << sh.first << std::endl;

		}
		catch (const ShaderException& e) {
			std::cerr << e.what() << std::endl;
			continue;
		}
	}
}
