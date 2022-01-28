#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "Game/Field.hpp"
#include "Game/MiniField.hpp"
#include "Framework/Window.hpp"


/**
namespace app
@brief Пространство имен игры.
*/
namespace app
{

/**
	@brief Класс игры.
*/
class Game
{
public:
	Game();
	~Game();

	/**
		@brief Запуск и инициализация данных игры.
	*/
	void run();

	/**
		@brief Обновление игры.
	*/
	void update();

	/**
		@brief Удаление всех данных.
	*/
	void destroy();

	/**
		@brief Возвращает 
	*/
	//const double& getDeltaTime() const;

protected:

	/**
		@brief Проверка, находится ли курсор мыши в заданном прямоугольнике.
	*/
	inline const bool isIntersect(const glm::vec2& position, const glm::vec2& rect) const;
	
	/**
		@brief Проверка, нажата ли левая кнопка мыши в заданном прямоугольнике.
	*/
	inline const bool isClicked(const glm::vec2& position, const glm::vec2& rect) const;

private:
	framework::Window* _window;

	ConfigFile _config;

	app::Field* _field;
	app::MiniField* _mini;

	std::vector<Checker*> _checkers;

	glm::vec3 _background;
	glm::ivec2 _mouse;
	const glm::vec2 _select_rect;

	//Timer
	unsigned long long _current_time, _last_time;
	double _delta_time;
};

}

#endif