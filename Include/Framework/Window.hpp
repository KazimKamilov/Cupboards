#ifndef _FRAMEWORK_WINDOW_HPP_
#define _FRAMEWORK_WINDOW_HPP_

#include <string>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "SDL.h"

/**
namespace framework
@brief Пространство имен вспомогательных функций.
*/
namespace framework
{

/**
	@brief Класс окна. Основан на SDL2.
*/
class Window
{
public:
	Window();
	~Window();

	/**
		@brief Функция загрузки курсора из bmp-файла.
		@param Строка с именем файла.
	*/
	void loadCursor(const std::string& cursor_bmp_file);
	
	/**
		@brief Устанавливает цвет бэкграунда.
	*/
	void setBackgroundColor(const glm::vec3& color);

	/**
		@brief Возвращает указатель на эвент SDL2.
	*/
	SDL_Event* getEvent();
	
	/**
		@brief Возвращает указатель на окно SDL2.
	*/
	SDL_Window* getSDLWindow();

	void PreUpdateMainWindow();
	void PreUpdateAdditiveWindow();

	/**
		@brief Функция обновления окна.
	*/
	void update();

	void updateAdditive();

	/**
		@brief Проверка, обновляется ли окно(работает ли приложение).
	*/
	const bool isWorking() const;

private:
	//Window pointer
	SDL_Window* _window;
	SDL_Window* _additive_window;
	SDL_Event _event;
	SDL_GLContext _context;

	glm::ivec2 _additive_pos;

	//Is still window updated
	bool _working;

	glm::vec3 _background;
};

}

#endif