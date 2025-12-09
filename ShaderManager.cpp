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
			LogShaderManagerError("Invalid shader created.");
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
		LogShaderManagerError("Shader program '" + programName + "' not found.");
		return false;
	}

	shaderPrograms.erase(programName);
	return true;
}

bool ShaderManager::bind(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		LogShaderManagerError("Shader program '" + programName + "' not found.");
		return false;
	}
	shaderPrograms[programName]->Activate();
	return true;
}

Shader* ShaderManager::get(std::string programName)
{
	if (shaderPrograms.find(programName) == shaderPrograms.end()) {
		LogShaderManagerError("Shader program '" + programName + "' not found.");
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

			LogShaderManagerInfo("Success recompiling Shader: " + sh.first);


		}
		catch (const ShaderException& e) {
			std::cerr << e.what() << std::endl;
			continue;
		}
	}
}
