#ifndef _GAME_CHECKER_HPP_
#define _GAME_CHECKER_HPP_

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>



/**
namespace app
@brief Пространство имен игры.
*/
namespace app
{

/**
	@brief Класс фишки.
*/
class Checker
{
public:
	Checker();
	~Checker();

	/**
		@brief Добавляет координаты в список анимации.
	*/
	void addMovePath(const glm::ivec2& path);
	
	/**
		@brief Очищает анимацию.
	*/
	void clearPath();

	/**
		@brief Проигрывание анимации.
	*/
	void moveProcess(const double& delta);
	
	/**
		@brief Движение к заданной точке(запуск анимации).
	*/
	void moveToPoint(const glm::ivec2& point);

	/**
		@brief Connects checker to the cell where it is located.
	*/
	void setCellPtr(void* const cell_ptr);

	/**
		@brief Устанавливает цвет фишки.
	*/
	void setColor(const glm::vec3& color);

	/**
		@brief Устанавливает позицию фишки(в пикселях).
	*/
	void setPosition(const glm::vec2& position);

	/**
		@brief Выделяет фишку(красный круг вокруг фишки).
	*/
	void setSelect(const bool select);
	
	/**
		@brief Выделяет фишку, когда она находится на финальной позиции(зеленый круг вокруг фишки).
	*/
	void setWin(const bool win);
	
	/**
		@brief Переключение режима выделения фишки(вкл/выкл).
	*/
	void toggleSelect();

	/**
		@brief Возвращает позицию фишки.
	*/
	const glm::ivec2 getPosition() const;

	/**
		@brief Возвращает цвет фишки.
	*/
	const glm::vec3& getColor() const;

	/**
		@brief Get connected cell pointer.
	*/
	const void* getCellPtr() const;

	/**
		@brief Проверка, выбрана ли фишка.
	*/
	const bool isSelect() const;
	
	/**

	*/
	const bool isMoving() const;

	/**
		@brief Проверка, находится ли фишка в финальной позиции.
	*/
	const bool isWin() const;

	/**
		@brief Отрисовка графона. XD
	*/
	void draw();

private:
	void* _cell_ptr;
	glm::vec2 _position;
	glm::vec3 _color;
	bool _win;
	unsigned int _VAO, _VBO;
	int _checkers_program;
	int _color_u;
	int _select_u, _win_u;
	bool _select;
	bool _move;
	std::vector<glm::ivec2> _move_path;
	glm::ivec2 _direction;
	glm::ivec2 _current_path;
	unsigned int _path_iter;
	float _anim_speed;
};

}

#endif