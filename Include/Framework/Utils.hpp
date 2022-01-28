#ifndef _FRAMEWORK_UTILS_HPP_
#define _FRAMEWORK_UTILS_HPP_

#include "glm/vec2.hpp"
#include <random>


/**
namespace framework
@brief Пространство имен вспомогательных функций.
*/
namespace util
{

/**
	@brief Функция конвертирования экранных координат OpenGL в координаты окна в пикселях.
*/
inline const glm::ivec2 OpenGLToScreen(const glm::vec2& gl_coords)
{
	//Get screen coords from OpenGL screen to screen in pixels
	const glm::vec2 crd(((gl_coords + 1.0f) * 0.5f) * glm::vec2(700.0f));
	//From OpenGL left-bottom screen to left-top
	return glm::ivec2(static_cast<int>(crd.x), 700 - static_cast<int>(crd.y));
}


/**
	@brief Функция конвертирования координат окна в пикселях в координаты OpenGL.
*/
inline const glm::vec2 ScreenToOpenGL(const glm::ivec2& screen_coords)
{
	//From screen pixels to OpenGL screen -1.0 - 1.0
	return (2.0f * (glm::vec2(static_cast<float>(screen_coords.x), 700.0f - static_cast<float>(screen_coords.y)) / glm::vec2(700.0f)) - 1.0f);
}


/**
	@brief Функция конвертирования координат окна в пикселях в координаты OpenGL(minimap).
*/
inline const glm::vec2 ScreenToMiniOpenGL(const glm::ivec2& screen_coords)
{
	//From screen pixels to OpenGL screen -1.0 - 1.0
	return (2.0f * ((glm::vec2(static_cast<float>(screen_coords.x), 700.0f - static_cast<float>(screen_coords.y)) / glm::vec2(700.0f)) / 3.5f) - 1.0f);
}


/**
	@brief 
*/
inline const float Random(float min, float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> uid(min, max);
	return uid(gen);
}


/**
	@brief
*/
inline const glm::vec3 RandomColor()
{
	return glm::vec3(Random(0.1f, 1.0f), Random(0.1f, 1.0f), Random(0.1f, 1.0f));
}

}

#endif