#ifndef _FRAMEWORK_SHADER_HPP_
#define _FRAMEWORK_SHADER_HPP_

#include <string>
#include "GL/gl3w.h"


/**
namespace framework
@brief Пространство имен вспомогательных функций.
*/
namespace framework
{

/**
	@brief Класс шейдера OpenGL.
*/
class Shader
{
public:

	/**
		@brief Перечислитель типов шейдера.
	*/
	enum Type
	{
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
	};

	/**
		@brief Конструктор.
		@param Тип шейдера.
	*/
	Shader(const Shader::Type type);
	
	~Shader();

	/**
		@brief Функция считывания кода шейдера из файла.
	*/
	void loadFromFile(const std::string& path);

	/**
		@brief Функция возвращает тип шейдера.
	*/
	const Type getType() const;
	
	/**
		@brief Функция возвращает уникальный ID-переменную OpenGL.
	*/
	const GLuint getShaderID() const;

private:
	GLuint _shader;
	Type _type;

};

}

#endif