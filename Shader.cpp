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
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	this->vertexFile = vertexFile;
	this->fragmentFile = fragmentFile;

	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create vertex shader obj, get ref and compile
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	// Handle Compilation errors
	if (checkCompileErrors(vertexShader, VERTEXSHADER)) {

		glDeleteShader(vertexShader);
		throw(ShaderException("Compiling shader failed", VERTEXSHADER, vertexFile));
	}

	// Create fragment shader obj, get ref and compile
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	// Handle Compilation errors
	if (checkCompileErrors(fragmentShader, FRAGMENTSHADER)) {

		glDeleteShader(fragmentShader);
		throw(ShaderException("Compiling fragment failed", FRAGMENTSHADER, fragmentFile));
	}

	// Create program
	ID = glCreateProgram();

	// Attach Shaders
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	
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

}

// Creating destructor so that when shader manager erases shader from map, it will call destructor immeditaely and release memory
Shader::~Shader()
{
	Delete();
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

	if (type == VERTEXSHADER || type == FRAGMENTSHADER) {

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

unsigned int Shader::getUniformLocation(const std::string& name)
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

