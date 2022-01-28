#include "Framework/Shader.hpp"
#include "Framework/OpenGLErrorCheck.hpp"
#include <fstream>


namespace framework
{

Shader::Shader(const Shader::Type type) :
	_shader(0u),
	_type(type)
{
}


Shader::~Shader()
{
	if (_shader > 0)
	{
		glDeleteShader(_shader);
		_shader = 0u;
		glCheckErrors();
	}
}


void Shader::loadFromFile(const std::string& path)
{
	std::fstream file;
	std::string source, temp;

	file.open(path, std::ios::in);	
	if (file.good())
	{
		while (!file.eof())
		{
			std::getline(file, temp);
			source += temp + "\n";
		}
	}
	else
	{
		std::cout << "Cannot open shader file \"" << path << "\"!" << std::endl;
	}

	file.close();

	const char* src = source.c_str();

	_shader = glCreateShader(_type);

	glShaderSource(_shader, 1, &src, nullptr);
	glCompileShader(_shader);

	GLint _result = 0;

	glGetShaderiv(_shader, GL_COMPILE_STATUS, &_result);

	GLchar error_stack[2048];

	if (!_result)
	{
		glGetShaderInfoLog(_shader, 2048, nullptr, error_stack);
		const std::string& shader_type = (_type == VERTEX) ? "vertex" : ((_type == GEOMETRY) ? "geometry" : "fragment");
		std::cout << "OpenGL " << shader_type << " shader error! Shader compilation failed! Info:\n" << std::string(error_stack) << std::endl;
	}
}


const Shader::Type Shader::getType() const
{
	return _type;
}


const GLuint Shader::getShaderID() const
{
	return _shader;
}

}