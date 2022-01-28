#ifndef _FRAMEWORK_CONFIG_FILE_HPP_
#define _FRAMEWORK_CONFIG_FILE_HPP_

#include <string>
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
	@brief Класс файла конфигураций игры.
*/
class ConfigFile
{
public:

	/**
		@brief Конструктор.
		@param Имя файла конфигураций.
	*/
	ConfigFile(const std::string& filepath);
	~ConfigFile();

	/**
		@brief Возвращает количество указанных в файле фишек.
	*/
	const unsigned int getCheckerNum() const;
	
	/**
		@brief Возвращает количество указанных в файле ячеек.
	*/
	const unsigned int getAvailableCells() const;
	
	/**
		@brief Возвращает количество указанных в файле соединений.
	*/
	const unsigned int getJointNum() const;

	/**
		@brief Возвращает константный адрес на массив координат ячеек.
	*/
	const std::vector<glm::ivec2>& getCheckerCoordList() const;
	
	/**
		@brief Возвращает константный адрес на массив стартовых позиций фишек.
	*/
	const std::vector<unsigned int>& getStartPosList() const;
	
	/**
		@brief Возвращает константный адрес на массив выйгрышных позиций фишек.
	*/
	const std::vector<unsigned int>& getWinPosList() const;
	
	/**
		@brief Возвращает константный адрес на массив соединений ячеек.
	*/
	const std::vector< std::pair<unsigned int, unsigned int> >& getJointList() const;

private:
	unsigned int checker_num;
	unsigned int available_cells;
	std::vector<glm::ivec2> available_checker_coord_list;
	std::vector<unsigned int> start_checker_pos;
	std::vector<unsigned int> win_checker_pos;
	unsigned int joint_num;
	std::vector< std::pair<unsigned int, unsigned int> > joint_list;
};

}

#endif