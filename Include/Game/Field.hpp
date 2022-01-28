#ifndef _GAME_FIELD_HPP_
#define _GAME_FIELD_HPP_

#include "glm/vec2.hpp"
#include "Game/CellTree.hpp"
#include <stack>
#include <map>

/**
namespace app
@brief Пространство имен игры.
*/
namespace app
{

/**
	@brief Класс игрового поля.
*/
class Field
{
public:

	/**
		@brief Конструктор.
		@param Константный адрес на конфигурационный файл.
	*/
	Field(const ConfigFile& cfg, const std::vector<glm::vec3>& colors);
	~Field();

	/**
		@brief Устанавливает указатель на фишку по заданному индексу.
	*/
	void setCellChecker(const unsigned char cell_index, Checker* const checker);
	
	/**
		@brief Очищает указатель на фишку в заданной ячейке по индексу.
	*/
	void setCellFree(const unsigned char cell_index);

	/**
		@brief Функция выбора ячейки.
	*/
	void selectCell(const unsigned char cell_index);
	
	/**
		@brief Функция отмены выбора ячейки.
	*/
	void unselectCell(const unsigned char cell_index);

	/**
		@brief Функция передвижения выбранной фишки к заданной ячейке по индексу.
	*/
	void moveSelectedTo(const unsigned char cell_index);

	/**
		@brief Проверка, пуста ли ячейка.
	*/
	const bool isCellFree(const unsigned char cell_index) const;
	
	/**
		@brief Проверка, есть ли выбранные фишки.
	*/
	const bool haveSelected() const;

	/**
		@brief Возвращает координаты ячейки по заданному индексу.
	*/
	const glm::ivec2 getCellCoords(const unsigned char cell_index) const;

	/**

	*/
	const std::vector<glm::ivec2>& getCellCoordsArray() const;

	/**
		@brief Возвращает указатель на шашку по заданному индексу.
	*/
	Checker* getChecker(const unsigned char cell_index) const;

	/**

	*/
	CellTree::Cell* getTreeCell(const unsigned char cell_index);

	/**

	*/
	const std::size_t getTreeSize() const;

	/**
		@brief Отрисовка поля.
	*/
	void draw();

protected:

	/**
		@brief Рекурсивная функция поиска доступных путей для передвижения выбранной фишки.
	*/
	void searchAvailablePaths(const unsigned char cell_index);
	
	/**
		@brief Проверка, есть ли указанный индекс в доступных путях.
	*/
	const bool isHaveInAvailablePaths(const unsigned char cell_index);

	/**
		@brief Рекурсивный поиск пути движения выбранной фишки.
	*/
	void searchPaths(const unsigned char index, const unsigned char from, const unsigned char dest_index);

	const std::vector<unsigned char> getPath(const unsigned char start, const unsigned char finish);

private:
	CellTree _tree;
	unsigned int _VAO[3];
	unsigned int _VBO[3];
	int _point_program, _line_program, _circle_program;
	Checker* _selected;
	unsigned char _selected_id;
	std::vector<glm::vec2> _field_points;
	int _points_num;
	std::vector<glm::vec2> _field_lines;
	int _lines_num;
	std::vector<float> _field_circles;	
	std::vector<glm::ivec2> _cell_coords;	
	std::vector<unsigned char> _available_paths;
	std::vector<unsigned char> _path;
	std::vector<bool> _mark;
};

}

#endif