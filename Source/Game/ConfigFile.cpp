#include "Game/ConfigFile.hpp"
#include <fstream>
#include <iostream>



namespace app
{

ConfigFile::ConfigFile(const std::string& filepath) : 
	checker_num(0u),
	available_cells(0u),
	joint_num(0u)
{
	std::fstream file;

	file.open(filepath, std::ios::in);
	if (!file.good())
	{
		std::cout << "Cannot open config file \"" << filepath << "\"!" << std::endl;
	}
	else
	{
		std::string temp;
		unsigned int line = 0u;

		//line 1 - checker num
		line++;
		std::getline(file, temp);
		checker_num = static_cast<unsigned int>(std::stoi(temp));
#ifdef DEBUG
		std::cout << "Checker num is: " << checker_num << std::endl;
#endif
		temp.clear();

		//line 2 - available sockets
		line++;
		std::getline(file, temp);
		available_cells = static_cast<unsigned int>(std::stoi(temp));
#ifdef DEBUG
		std::cout << "Sockets num is: " << available_cells << std::endl;
#endif
		temp.clear();

		////////////////////////////////////////////////////////////////////////////////////////////
		//coords
		for (auto i = 0u; i < available_cells; i++)
		{
			line++;
			std::getline(file, temp);
			const std::size_t pos = temp.find(',');
			if (pos != std::string::npos)
			{
				const glm::ivec2 coord(std::stoi(temp.substr(0u, pos)), std::stoi(temp.substr(pos + 1u, (temp.length() - pos) - 1u)));
#ifdef DEBUG
				std::cout << "Coords is: " << coord.x << ", " << coord.y << std::endl;
#endif
				available_checker_coord_list.push_back(coord);
			}
			else
			{
				std::cout << "Hey! Game config file parsing error at line " << line << "!" << std::endl;
			}

			temp.clear();
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		//Start pos
		line++;
		std::getline(file, temp);
		std::string number;
		for (auto i = 0u; i < temp.size(); i++)
		{
			if (temp.at(i) == ',')
			{
				//In array order! 0...8
				start_checker_pos.push_back(static_cast<unsigned int>(std::stoi(number)) - 1u);
#ifdef DEBUG
				std::cout << "Start position is: " << number << std::endl;
#endif
				number.clear();
			}
			else if (i == (temp.size() - 1u))
			{
				number += temp.at(i);
				//In array order! 0...8
				start_checker_pos.push_back(static_cast<unsigned int>(std::stoi(number)) - 1u);
#ifdef DEBUG
				std::cout << "Start position is: " << number << std::endl;
#endif
				number.clear();
			}
			else if(temp.at(i) >= '0' && temp.at(i) <= '9')
			{
				number += temp.at(i);
			}
			else
			{
				std::cout << "Hey! Game config file parsing error at line " << line << "!" << std::endl;
			}
		}

		temp.clear();

		///////////////////////////////////////////////////////////////////////////////////////////
		//Win pos
		line++;
		std::getline(file, temp);
		for (auto i = 0u; i < temp.size(); i++)
		{
			if (temp.at(i) == ',')
			{
				//In array order! 0...8
				win_checker_pos.push_back(static_cast<unsigned int>(std::stoi(number)) - 1u);
#ifdef DEBUG
				std::cout << "Win position is: " << number << std::endl;
#endif
				number.clear();
			}
			else if (i == (temp.size() - 1u))
			{
				number += temp.at(i);
				//In array order! 0...8
				win_checker_pos.push_back(static_cast<unsigned int>(std::stoi(number)) - 1u);
#ifdef DEBUG
				std::cout << "Win position is: " << number << std::endl;
#endif
				number.clear();
			}
			else if (temp.at(i) >= '0' && temp.at(i) <= '9')
			{
				number += temp.at(i);
			}
			else
			{
				std::cout << "Hey! Game config file parsing error at line " << line << "!" << std::endl;
			}
		}

		temp.clear();

		//////////////////////////////////////////////////////////////////////////////////////////
		//Num of joints
		line++;
		std::getline(file, temp);
		joint_num = static_cast<unsigned int>(std::stoi(temp));
#ifdef DEBUG
		std::cout << "Joint num is: " << joint_num << std::endl;
#endif
		temp.clear();

		////////////////////////////////////////////////////////////////////////////////////////////
		//joints
		for (auto i = 0u; i < joint_num; i++)
		{
			line++;
			std::getline(file, temp);
			const std::size_t pos = temp.find(',');
			if (pos != std::string::npos)
			{
				//In array order! 0...8
				const glm::uvec2 coord(static_cast<unsigned int>(std::stoi(temp.substr(0u, pos))) - 1u, static_cast<unsigned int>(std::stoi(temp.substr(pos + 1u, (temp.length() - pos) - 1u))) - 1u);
#ifdef DEBUG
				std::cout << "Joint is: " << coord.x << ", " << coord.y << std::endl;
#endif
				joint_list.push_back(std::pair<unsigned int, unsigned int>(coord.x, coord.y));
			}
			else
			{
				std::cout << "Hey! Game config file parsing error at line " << line << "!" << std::endl;
			}

			temp.clear();
		}
	}

	file.close();
}


ConfigFile::~ConfigFile()
{
	available_checker_coord_list.clear();
	start_checker_pos.clear();
	win_checker_pos.clear();
	joint_list.clear();
}


const unsigned int ConfigFile::getCheckerNum() const
{
	return checker_num;
}


const unsigned int ConfigFile::getAvailableCells() const
{
	return available_cells;
}

const unsigned int ConfigFile::getJointNum() const
{
	return joint_num;
}


const std::vector<glm::ivec2>& ConfigFile::getCheckerCoordList() const
{
	return available_checker_coord_list;
}


const std::vector<unsigned int>& ConfigFile::getStartPosList() const
{
	return start_checker_pos;
}


const std::vector<unsigned int>& ConfigFile::getWinPosList() const
{
	return win_checker_pos;
}


const std::vector< std::pair<unsigned int, unsigned int> >& ConfigFile::getJointList() const
{
	return joint_list;
}

}