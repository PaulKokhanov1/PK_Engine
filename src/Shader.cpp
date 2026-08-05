#include"Shader.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader()
{
	ID = -1;
	vertexFile = "";
	fragmentFile = "";
	geometryFile = "";
}

Shader::Shader(const char* vertexFile, const char* fragmentFile, std::optional<const char*> geometryFile, std::optional<const char*> tcsFile, std::optional<const char*> tesFile)
{
	this->vertexFile = vertexFile;
	this->fragmentFile = fragmentFile;
	this->geometryFile = geometryFile;
	this->tcsFile = tcsFile;
	this->tesFile = tesFile;

	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexFile, VERTEXSHADER);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentFile, FRAGMENTSHADER);
	GLuint geometryShader = compileShader(GL_GEOMETRY_SHADER, geometryFile ? geometryFile.value() : "", GEOMETRYSHADER);
	GLuint tcsShader = compileShader(GL_TESS_CONTROL_SHADER, tcsFile ? tcsFile.value() : "", TESSELLATIONCONTROLSHADER);
	GLuint tesShader = compileShader(GL_TESS_EVALUATION_SHADER, tesFile ? tesFile.value() : "", TESSELLATIONEVALUTATIONSHADER);

	// Create program
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	if (geometryShader != 0)
	{
		glAttachShader(ID, geometryShader);
	}

	if (tcsShader != 0 && tesShader != 0)
	{
		glAttachShader(ID, tcsShader);
		glAttachShader(ID, tesShader);
	}


	// Link shaders into program
	glLinkProgram(ID);
	// Check Link status
	if (checkCompileErrors(ID, PROGRAM)) {
		// The program is useless now. So delete it.
		glDeleteShader(ID);
		throw(ShaderException("Linking program failed", PROGRAM));
	}

	// Delete unnecessary shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShader != 0) glDeleteShader(geometryShader);	// 0 is a sentinel value
	if (tcsShader != 0) glDeleteShader(tcsShader);
	if (tesShader != 0) glDeleteShader(tesShader);

}

// Creating destructor so that when shader manager erases shader from map, it will call destructor immeditaely and release memory
Shader::~Shader()
{
	Delete();
}

GLuint Shader::compileShader(GLenum type, const char* filename, errorType err)
{
	GLuint shader = 0;
	if (filename != "") {
		std::string code = get_file_contents(filename);
		const char* source = code.c_str();

		// Create geometry shader obj, get ref and compile

		shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		// Handle Compilation errors
		if (checkCompileErrors(shader, err)) {

			glDeleteShader(shader);
			throw(ShaderException("Compiling shader failed", err, filename));
		}

	}
	return shader;

}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

bool Shader::checkCompileErrors(GLuint id, errorType type)
{
	GLint success = 0;

	if (type == VERTEXSHADER || type == FRAGMENTSHADER || type == GEOMETRYSHADER || type == TESSELLATIONCONTROLSHADER || type == TESSELLATIONEVALUTATIONSHADER) {

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			std::cerr << "----------------------------------------\n";
			std::cerr << "Shader compilation failed\n";
			std::cerr << infoLog.data() << "\n";
			std::cerr << "----------------------------------------\n";

			return true;
		}
	} else {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			std::cerr << "----------------------------------------\n";
			std::cerr << "Program compilation failed\n";
			std::cerr << infoLog.data() << "\n";
			std::cerr << "----------------------------------------\n";

			return true;
		}
	}

	return false;

}

bool Shader::isValid() const {
	return glIsProgram(ID) == GL_TRUE;
}

const char* Shader::getVertexFile()
{
	return vertexFile;
}

const char* Shader::getFragmentFile()
{
	return fragmentFile;
}

std::optional<const char*> Shader::getGeometryFile()
{
	return geometryFile;
}

std::optional<const char*> Shader::getTCSFile()
{
	return tcsFile;
}

std::optional<const char*> Shader::getTESFile()
{
	return tesFile;
}

int Shader::getUniformLocation(const std::string& name) const
{
	if (shaderUniformLocations.find(name) != shaderUniformLocations.end()) {
		return shaderUniformLocations[name];
	}

	int location = glGetUniformLocation(ID, name.c_str());

	if (location == -1) {
		std::cerr << "Warning: uniform: " << name << " not found!" << std::endl;
	}

	shaderUniformLocations[name] = location;
	return location;
}

void Shader::setSampler(const std::string& name, GLuint slot)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniform1i(loc, slot);
}

void Shader::setUniform1i(const std::string& name, int val)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniform1i(loc, val);
}

void Shader::setUniform1f(const std::string& name, float val)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniform1f(loc, val);
}

void Shader::setUniform2fv(const std::string& name, const glm::vec2& val)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniform2fv(loc, 1, glm::value_ptr(val));
}

void Shader::setUniform3fv(const std::string& name, const glm::vec3& val)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniform3fv(loc, 1, glm::value_ptr(val));
}

void Shader::setUniform4fv(const std::string& name, const glm::vec4& val)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniform4fv(loc, 1, glm::value_ptr(val));
}

void Shader::setUniformMat3fv(const std::string& name, const glm::mat3& matrix)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformMat4fv(const std::string& name, const glm::mat4& matrix)
{
	GLint loc = getUniformLocation(name);
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

