#include"ShaderManager.h"

ShaderManager::ShaderManager()
{
}

void ShaderManager::load(const std::string& programName, const char* vertexShader, const char* fragmentShader, std::optional<const char*> geometryShader, std::optional<const char*> tcsFile, std::optional<const char*> tesFile)
{
	try {
		shaderPrograms[programName] = std::make_unique<Shader>(vertexShader, fragmentShader, geometryShader, tcsFile, tesFile);

		if (!shaderPrograms[programName]->isValid()) {
			LogShaderManagerError("Invalid shader created.");
		}
	}
	catch (const ShaderException& e) {
		std::cerr << e.what() << std::endl;
		return;
	}

}

bool ShaderManager::remove(const std::string& programName)
{
	auto it = shaderPrograms.find(programName);

	if (it == shaderPrograms.end())
	{
		LogShaderManagerError("Shader program '" + programName + "' not found.");
		return false;
	}

	shaderPrograms.erase(programName);
	return true;
}

bool ShaderManager::bind(const std::string& programName)
{
	auto it = shaderPrograms.find(programName);

	if (it == shaderPrograms.end())
	{
		LogShaderManagerError("Shader program '" + programName + "' not found.");
		return false;
	}
	it->second->Activate();
	return true;
}

Shader* ShaderManager::get(const std::string& programName)
{
	auto it = shaderPrograms.find(programName);

	if (it == shaderPrograms.end())
	{
		LogShaderManagerError("Shader program '" + programName + "' not found.");
		return nullptr;
	}
	return it->second.get();
}

void ShaderManager::reloadAll()
{

	// Iterate through all current shader's
	for (auto& [name, shader] : shaderPrograms) {

		// Create new shader, using same shader files as previous
		try {
			shaderPrograms[name] = std::make_unique<Shader>(shader->getVertexFile(), shader->getFragmentFile(), shader->getGeometryFile(), shader->getTCSFile(), shader->getTESFile());	// Keep same program name

			LogShaderManagerInfo("Success recompiling Shader: " + name);


		}
		catch (const ShaderException& e) {
			std::cerr << e.what() << std::endl;
			continue;
		}
	}
}
