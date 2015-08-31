#include "GLSLProgram.h"
#include "Errors.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace NDjinn;

GLSLProgram::GLSLProgram() : _numAttributes(0), _programId(0), _vertexShaderId(0), _fragmentShaderId(0)
{
}

GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
	_programId = glCreateProgram();
	
	_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderId == 0) {
		fatalError("Vertex shader failed to create!");
	}

	_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderId == 0) {
		fatalError("Fragment shader failed to create!");
	}

	compileShader(vertexShaderPath, _vertexShaderId);
	compileShader(fragmentShaderPath, _fragmentShaderId);
}

//Full Documentation https://www.opengl.org/wiki/Shader_Compilation
void GLSLProgram::linkShaders() {
	//Attach our shaders to our program
	glAttachShader(_programId, _vertexShaderId);
	glAttachShader(_programId, _fragmentShaderId);

	//Link our program
	glLinkProgram(_programId);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programId, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(_programId, maxLength, &maxLength, &infoLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programId);
		//Don't leak shaders either.
		std::printf("%s\n", &(infoLog[0]));
		fatalError("Shaders failed to link!");
		
		glDeleteShader(_vertexShaderId);
		glDeleteShader(_fragmentShaderId);
		//In this simple program, we'll just leave
		return;
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programId, _vertexShaderId);
	glDetachShader(_programId, _fragmentShaderId);
	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);
}

void GLSLProgram::addAttribute(const std::string attribName) {
	glBindAttribLocation(_programId, _numAttributes++, attribName.c_str());
}

void GLSLProgram::compileShader(const std::string filePath, GLuint& id) {

	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		perror(filePath.c_str());
		fatalError("Failed to open " + filePath);
	}

	std::string shaderContents = "";
	std::string line;
	while (std::getline(vertexFile, line)) {
		shaderContents += line + "\n"; //must add new line
	}

	vertexFile.close();

	const char* contentsPtr = shaderContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// exit with failure
		glDeleteShader(id); // prevent bad shader leakage
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader failed to compile!");
		return;
	}
}

void GLSLProgram::use() {
	glUseProgram(_programId);
	for (int i = 0; i < _numAttributes; ++i) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; ++i) {
		glDisableVertexAttribArray(i);
	}
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(_programId, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform " + uniformName + " not found in shader!");
	}
	return location;
}
