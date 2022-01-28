#ifndef _GAME_MINIFIELD_HPP_
#define _GAME_MINIFIELD_HPP_

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <vector>
#include "Game/ConfigFile.hpp"
#include "Game/Checker.hpp"


/**
namespace app
@brief Пространство имен игры.
*/
namespace app
{

/**
	@brief Класс мини-поля.
*/
class MiniField
{
public:

	/**
		@brief Конструктор.
		@param Константный адрес на конфигурационный файл и константный адрес на массив с фишками.
	*/
	MiniField(const ConfigFile& cfg, const std::vector<glm::ivec2>& coords, const std::vector<glm::vec3>& colors);
	~MiniField();

	/**
		@brief Отрисовка мини-поля.
	*/
	void draw();

private:
	std::vector<float> _field_points;
	std::vector<glm::vec2> _field_lines;
	int _points_num;
	int _lines_num;
	unsigned int _VAO[2];
	unsigned int _VBO[2];
	int _point_program, _line_program;

};

}

#endif