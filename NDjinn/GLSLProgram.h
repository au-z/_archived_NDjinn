#pragma once
#include <string>
#include <GL/glew.h>

class GLSLProgram
{
private:
	int _numAttributes; 
	GLuint _programId;
	GLuint _vertexShaderId;
	GLuint _fragmentShaderId;

	void compileShader(const std::string filePath, GLuint& id);

public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void linkShaders();
	void addAttribute(const std::string attribName);
	void use();
	void unuse();

	GLint getUniformLocation(const std::string& uniformName);
};

