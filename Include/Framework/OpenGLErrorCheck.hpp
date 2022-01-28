#ifndef _FRAMEWORK_OPENGL_ERROR_CHECK_HPP_
#define _FRAMEWORK_OPENGL_ERROR_CHECK_HPP_

#include "GL/gl3w.h"
#include <string>
#include <iostream>


/**
	@brief Проверка на ошибки OpenGL.
*/
inline const GLenum __gl_get_error(const char* file, const char* func, int line)
{
	GLenum _error_enum = 0u;

	while ((_error_enum = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (_error_enum)
		{
		case GL_INVALID_ENUM:
			error = "invalid enumeration";
			break;

		case GL_INVALID_VALUE:
			error = "invalid value";
			break;

		case GL_INVALID_OPERATION:
			error = "invalid operation";
			break;

		case GL_STACK_OVERFLOW:
			error = "stack overflow";
			break;

		case GL_STACK_UNDERFLOW:
			error = "stack underflow";
			break;

		case GL_OUT_OF_MEMORY:
			error = "out of memory";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "invalid framebuffer operation";
			break;
		}


		std::cout << "ERROR: OpenGL " << error << "! File: " << file << ", line: " << line << ", in function: " << func << std::endl;
	}

	return _error_enum;
}

/**
	@brief Проверка на ошибки OpenGL.
*/
#define glCheckErrors() __gl_get_error(__FILE__, __FUNCTION__, __LINE__)


#endif