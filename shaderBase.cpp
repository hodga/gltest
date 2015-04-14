
#include "shaderBase.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

using namespace hwgl;
using namespace std;

bool shaderHasError(GLuint shaderHandle, GLuint statusType)
{
	GLint status;
	glGetShaderiv(shaderHandle, statusType,&status);
	if(status == GL_FALSE)
	{
		string error("An error has occured\n");

		GLint infoLogLength;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *strInfoLog = new char [infoLogLength +1];
		glGetShaderInfoLog(shaderHandle, infoLogLength, NULL, strInfoLog);
		error += strInfoLog;
		delete(strInfoLog);
		return true;
	}
	return false;
}

bool programHasError(GLuint programHandle, GLuint statusType)
{
	GLint status;
	glGetProgramiv(programHandle, statusType,&status);
	if(status == GL_FALSE)
	{
		string error("An error has occured\n");

		GLint infoLogLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &infoLogLength);
		char *strInfoLog = new char [infoLogLength +1];
		glGetProgramInfoLog(programHandle, infoLogLength, NULL, strInfoLog);
		error += strInfoLog;
		delete(strInfoLog);
		return true;
	}
	return false;
}
ShaderBase::ShaderBase()
{

}

ShaderBase::~ShaderBase()
{
	for(GLuint shaderHandle : shaderHandles)
	{
		glDeleteShader(shaderHandle);
	}
}

GLuint ShaderBase::addShaderFromSource(const string &source, GLenum shaderType)
{
	GLuint shaderHandle = glCreateShader(shaderType);

	if(shaderHandle == 0)
	{
		cout << "failed to create shader, returning 0";
		return 0;
	}
	
	const char *code = source.c_str();
	glShaderSource(shaderHandle, 1, (const GLchar **)&code, NULL);

	glCompileShader(shaderHandle);
	
	if(shaderHasError(shaderHandle, GL_COMPILE_STATUS))
	{
		cout << "failed compiling shader, returning 0";
		glDeleteShader(shaderHandle);
		return 0;
	}

	shaderHandles.push_back(shaderHandle);
	return shaderHandle;
	
}

GLuint ShaderBase::addShaderFromFile(const string &fileName, GLenum shaderType)
{
	ifstream f;
	f.open(fileName.c_str(), ios::in | ios::binary);
	if(!f.is_open()){
		throw runtime_error(std::string("Failed to open file: ") + fileName);
	}

	stringstream buffer;
	buffer << f.rdbuf();
	return addShaderFromSource(buffer.str(), shaderType);
}

GLuint ShaderBase::makeProgram()
{

	if(shaderHandles.size() <= 0)
	{
		cout << "No shaders were added, returning 0";
		return 0;
	}

	GLuint programHandle = glCreateProgram();
	if(programHandle == 0)
	{
		cout << "Failed creating program, returning 0";
		return 0;
	}

	for(GLuint shaderHandle : shaderHandles)
	{
		glAttachShader(programHandle, shaderHandle);
	}

	glLinkProgram(programHandle);
	
	for(GLuint shaderHandle : shaderHandles)
	{
		glDetachShader(programHandle, shaderHandle);
	}

	if(programHasError(programHandle, GL_LINK_STATUS))
	{
		cout <<"Failed linking program, returning 0";
		glDeleteProgram(programHandle);
		return 0;
	}
	ShaderBase::programHandle = programHandle;
	return programHandle;
}

GLint ShaderBase::attrib(const GLchar* attribName) const {
	if(!attribName)
		    throw std::runtime_error("attribName was NULL");
	        
	GLint attrib = glGetAttribLocation(programHandle, attribName);
	if(attrib == -1)
		throw std::runtime_error(std::string("Program attribute not found: ") + attribName);
		        
	return attrib;
}

GLint ShaderBase::uniform(const GLchar* uniformName) const {
	if(!uniformName)
		throw std::runtime_error("uniformName was NULL");
	        
	GLint uniform = glGetUniformLocation(programHandle, uniformName);
	if(uniform == -1)
	throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
	
	return uniform;
}
