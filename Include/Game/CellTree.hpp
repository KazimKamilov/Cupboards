#ifndef _GAME_POINTER_TREE_HPP_
#define _GAME_POINTER_TREE_HPP_

#include <vector>
#include "Game/Checker.hpp"
#include "Game/ConfigFile.hpp"


/**
namespace app
@brief Пространство имен игры.
*/
namespace app
{

/**
	@brief Класс бинарного древа ячеек.
	Конфигурация соединений считывается из файла и записывается в виде древа указателей на ячейки.
*/
class CellTree
{
public:

	/**
		@brief Конструктор.
		@param Адрес на конфигурационный файл.
	*/
	CellTree(const ConfigFile& cfg, const std::vector<glm::vec3>& colors);
	~CellTree();

	/**
		@brief Класс ячейки.
	*/
	class Cell
	{
	public:
		Cell(const unsigned char index, const glm::vec3& color) : _index(index), _win_color(color), _checker_pointer(nullptr)
		{
		}

		~Cell()
		{
			_paths.clear();
			_checker_pointer = nullptr;
		}

		/**
			@brief Добавляет указатель на ячейку к текушему древу.
		*/
		void addPath(const unsigned char path_index)
		{
			_paths.push_back(path_index);
		}

		/**
			@brief Устанавливает указатель на фишку в ячейке.
		*/
		void setChecker(Checker* const checker)
		{
			_checker_pointer = checker;
		}

		/**
			@brief Возвращает индекс ячейки.
		*/
		const unsigned char getIndex() const
		{
			return _index;
		}

		/**
			@brief Возвращает указатель на фишку в текушей ячейке.
		*/
		Checker* getChecker() const
		{
			return _checker_pointer;
		}

		/**
			@brief Возвращает константный адрес на массив указателей для текушей ячейки.
		*/
		const std::vector<unsigned char>& getPaths() const
		{
			return _paths;
		}

		/**

		*/
		const std::size_t getPathsNum() const
		{
			return _paths.size();
		}

		/**
			@brief 
		*/
		const glm::vec3& getWinColor() const
		{
			return _win_color;
		}

		/**
			@brief Проверка, свободна ли ячейка.
		*/
		const bool isCellFree() const
		{
			return (_checker_pointer != nullptr) ? false : true;
		}

		/**
			@brief
		*/
		const bool isPathsDeadEnd() const
		{
			return (_paths.size() == 1u) ? true : false;
		}

	private:
		const unsigned char _index;
		glm::vec3 _win_color;
		Checker* _checker_pointer;
		std::vector<unsigned char> _paths;
	};

	/**
		@brief Проверка, есть ли фишка в заданной ячейке.
	*/
	const bool isCellHaveChecker(const unsigned char index) const;

	/**
		@brief Возвращает указатель на ячейку по заданному индексу.
	*/
	CellTree::Cell* getCell(const unsigned char index);
	
	/**
		@brief Возвращает указатель на фишку по заданному индексу.
	*/
	Checker* getCheckerPtr(const unsigned char index) const;

	/**

	*/
	const std::size_t getTreeCellsNum() const;

	/**

	*/
	const glm::vec3& getWinColor(const unsigned char index) const;

private:
	std::vector<Cell> _tree;

};

}

#endif