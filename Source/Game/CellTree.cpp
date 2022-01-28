#include "Game/CellTree.hpp"
#include <iostream>


namespace app
{

CellTree::CellTree(const ConfigFile& cfg, const std::vector<glm::vec3>& colors)
{
	for (auto i = 0u; i < cfg.getAvailableCells(); i++)
	{
		_tree.push_back(Cell(i, colors.at(i)));
	}

	const std::vector< std::pair<unsigned int, unsigned int> >& joints = cfg.getJointList();

	if(joints.size() != cfg.getJointNum())  std::cout << "Hey! Why the hell does the number of joints not match the number of joint list?!" << std::endl;

	//Here we create all available paths by cell pointers
	//From file... Argh...
	for (const auto& pair : joints)
	{
		_tree.at(pair.first).addPath(pair.second);
		_tree.at(pair.second).addPath(pair.first);
	}
}


CellTree::~CellTree()
{
}


const bool CellTree::isCellHaveChecker(const unsigned char index) const
{
	if (index > (_tree.size() - 1u))
	{
		std::cout << "Array out of bounds!" << std::endl;
		return false;
	}
	else
	{
		if (_tree.at(index).getChecker() != nullptr) return true;
		else return false;
	}
}


CellTree::Cell* CellTree::getCell(const unsigned char index)
{
	if (index > (_tree.size() - 1u))
	{
		std::cout << "Array out of bounds! Nullptr will be returned..." << std::endl;
		return nullptr;
	}
	else
	{
		return &_tree.at(index);
	}
}


Checker* CellTree::getCheckerPtr(const unsigned char index) const
{
	if (index > (_tree.size() - 1u))
	{
		std::cout << "Array out of bounds! Nullptr will be returned..." << std::endl;
		return nullptr;
	}
	else
	{
		return _tree.at(index).getChecker();
	}
}


const std::size_t CellTree::getTreeCellsNum() const
{
	return _tree.size();
}


const glm::vec3& CellTree::getWinColor(const unsigned char index) const
{
	return _tree.at(index).getWinColor();
}

}