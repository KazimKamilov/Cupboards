#include "Game/Field.hpp"
#include "Framework/OpenGLErrorCheck.hpp"
#include "Framework/Shader.hpp"
#include "Framework/Utils.hpp"
#include "glm/glm.hpp"



namespace app
{

Field::Field(const ConfigFile& cfg, const std::vector<glm::vec3>& colors) :
	_tree(cfg, colors),
	_VAO{ 0u, 0u, 0u },
	_VBO{ 0u, 0u, 0u },
	_point_program(0),
	_line_program(0),
	_circle_program(0),
	_selected(nullptr),
	_selected_id(0u)
{
	/////////////////////////////////////////////////////////////////////////////////
	//Field creation
	/////////////////////////////////////////////////////////////////////////////////

	for (auto i = 0u; i < cfg.getAvailableCells(); i++)
	{
		_cell_coords.push_back(cfg.getCheckerCoordList().at(i));
		_field_points.push_back(util::ScreenToOpenGL(_cell_coords.at(i)));
	}

	const auto& joints = cfg.getJointList();

	for (const auto& i : joints)
	{
		_field_lines.push_back(util::ScreenToOpenGL(_cell_coords.at(i.first)));
		_field_lines.push_back(util::ScreenToOpenGL(_cell_coords.at(i.second)));
	}

	_points_num = static_cast<GLsizei>(_field_points.size());
	_lines_num = static_cast<GLsizei>(_field_lines.size());


	{
		framework::Shader v_shader(framework::Shader::VERTEX);
		v_shader.loadFromFile("shaders/field_lines_vs.glsl");

		framework::Shader f_shader(framework::Shader::FRAGMENT);
		f_shader.loadFromFile("shaders/field_lines_fs.glsl");

		_line_program = glCreateProgram();
		glAttachShader(_line_program, v_shader.getShaderID());
		glAttachShader(_line_program, f_shader.getShaderID());
		glLinkProgram(_line_program);

		int result = 0;
		char log[512];

		glGetProgramiv(_line_program, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(_line_program, 512, nullptr, log);
			std::cout << "OpenGL program link failed! Reason:\n" << log << std::endl;
		}

		/////////////////////////////////////////////////////////////////////////////////

		glGenVertexArrays(3, _VAO);
		glGenBuffers(3, _VBO);

		glBindVertexArray(_VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _field_lines.size(), _field_lines.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

		glBindVertexArray(0);

		glCheckErrors();
	}

	{
		framework::Shader v_shader(framework::Shader::VERTEX);
		v_shader.loadFromFile("shaders/field_vs.glsl");

		framework::Shader g_shader(framework::Shader::GEOMETRY);
		g_shader.loadFromFile("shaders/field_gs.glsl");

		framework::Shader f_shader(framework::Shader::FRAGMENT);
		f_shader.loadFromFile("shaders/field_fs.glsl");

		_point_program = glCreateProgram();
		glAttachShader(_point_program, v_shader.getShaderID());
		glAttachShader(_point_program, g_shader.getShaderID());
		glAttachShader(_point_program, f_shader.getShaderID());
		glLinkProgram(_point_program);

		int result = 0;
		char log[512];

		glGetProgramiv(_point_program, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(_point_program, 512, nullptr, log);
			std::cout << "OpenGL program link failed! Reason:\n" << log << std::endl;
		}

		/////////////////////////////////////////////////////////////////////////////////

		glBindVertexArray(_VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _field_points.size(), _field_points.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

		glBindVertexArray(0);

		glCheckErrors();
	}

	/////////////////////////////////////////////////////////////////////////////////
	//Selection circles
	/////////////////////////////////////////////////////////////////////////////////

	{
		framework::Shader v_shader(framework::Shader::VERTEX);
		v_shader.loadFromFile("shaders/field_circles_vs.glsl");

		framework::Shader g_shader(framework::Shader::GEOMETRY);
		g_shader.loadFromFile("shaders/field_circles_gs.glsl");

		framework::Shader f_shader(framework::Shader::FRAGMENT);
		f_shader.loadFromFile("shaders/field_circles_fs.glsl");

		_circle_program = glCreateProgram();
		glAttachShader(_circle_program, v_shader.getShaderID());
		glAttachShader(_circle_program, g_shader.getShaderID());
		glAttachShader(_circle_program, f_shader.getShaderID());
		glLinkProgram(_circle_program);

		int result = 0;
		char log[512];

		glGetProgramiv(_circle_program, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(_circle_program, 512, nullptr, log);
			std::cout << "OpenGL program link failed! Reason:\n" << log << std::endl;
		}

		/////////////////////////////////////////////////////////////////////////////////

		glBindVertexArray(_VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

		glBindVertexArray(0);

		glCheckErrors();
	}
}


Field::~Field()
{
	_field_points.clear();
	_field_lines.clear();
	_field_circles.clear();

	glDeleteProgram(_point_program);
	glDeleteProgram(_line_program);
	glDeleteProgram(_circle_program);
	glDeleteBuffers(3, _VBO);
	glDeleteVertexArrays(3, _VAO);
}


void Field::setCellChecker(const unsigned char cell_index, Checker* const checker)
{
	auto cell = _tree.getCell(cell_index);
	cell->setChecker(checker);
	checker->setCellPtr(cell);
}


void Field::setCellFree(const unsigned char cell_index)
{
	auto cell = _tree.getCell(cell_index);
	cell->setChecker(nullptr);
}


const bool Field::isCellFree(const unsigned char cell_index) const
{
	return (_tree.isCellHaveChecker(cell_index) ? false : true);
}


const glm::ivec2 Field::getCellCoords(const unsigned char cell_index) const
{
	return _cell_coords.at(cell_index);
}


const std::vector<glm::ivec2>& Field::getCellCoordsArray() const
{
	return _cell_coords;
}


Checker* Field::getChecker(const unsigned char cell_index) const
{
	return _tree.getCheckerPtr(cell_index);
}


CellTree::Cell* Field::getTreeCell(const unsigned char cell_index)
{
	return _tree.getCell(cell_index);
}


void Field::selectCell(const unsigned char cell_index)
{
	_selected = _tree.getCheckerPtr(cell_index);
	_selected_id = cell_index;
	_selected->setSelect(true);

	this->searchAvailablePaths(_selected_id);

	_selected->clearPath();

	for (const auto& i : _available_paths)
	{
		const glm::vec2& vec = util::ScreenToOpenGL(this->getCellCoords(i));
		_field_circles.push_back(vec.x);
		_field_circles.push_back(vec.y);
	}
}


void Field::unselectCell(const unsigned char cell_index)
{
	if (_selected != nullptr)
	{
		_selected->setSelect(false);
		_selected_id = 0u;
		_available_paths.clear();
		_selected = nullptr;
		_field_circles.clear();
	}
}


const bool Field::haveSelected() const
{
	return (_selected != nullptr) ? true : false;
}


const std::size_t Field::getTreeSize() const
{
	return _tree.getTreeCellsNum();
}


void Field::searchAvailablePaths(const unsigned char cell_index)
{
	for (const auto& cell : _tree.getCell(cell_index)->getPaths())
	{
		if (this->isCellFree(cell))
		{
			if (!this->isHaveInAvailablePaths(cell))
			{
				_available_paths.push_back(cell);
				this->searchAvailablePaths(cell);
			}
		}
	}
}


const bool Field::isHaveInAvailablePaths(const unsigned char cell_index)
{
	for (const auto& i : _available_paths)
	{
		if (cell_index == i) return true;
	}

	return false;
}


const std::vector<unsigned char> Field::getPath(const unsigned char start, const unsigned char finish)
{
	std::vector<unsigned char> result;

	for (unsigned int i = finish; i != start; i = _path.at(i))
	{
		result.push_back(i);
	}

	std::reverse(result.begin(), result.end());

	return result;
}


void Field::searchPaths(const unsigned char index, const unsigned char from, const unsigned char dest_index)
{
	if (!this->isCellFree(index) && _selected_id != index)
	{
		_mark.at(index) = true;
		_path.at(index) = from;
		return;
	}

	//DFS method
	const auto& paths = _tree.getCell(index)->getPaths();

	_mark.at(index) = true;
	_path.at(index) = from;

	for (auto i : paths)
	{
		if (!_mark.at(i))
		{
			searchPaths(i, index, dest_index);
		}
	}
}


void Field::moveSelectedTo(const unsigned char cell_index)
{
	if (_selected != nullptr && this->isHaveInAvailablePaths(cell_index))
	{
		//Don't worry, it's safe.
		const auto old_cell = static_cast<const CellTree::Cell*>(_selected->getCellPtr());
		const auto old = old_cell->getIndex();

		//Clear cell pointer for selected checker.
		_selected->setCellPtr(nullptr);

		//Sets new checker pointer for cell
		this->setCellChecker(cell_index, _selected);
		////////////////////////////////

		const auto size = _tree.getTreeCellsNum();
		if(_path.size() != size) _path.resize(size);
		std::fill(_path.begin(), _path.end(), 0u);

		if (_mark.size() != size) _mark.resize(size);
		std::fill(_mark.begin(), _mark.end(), false);

		this->searchPaths(_selected_id, _selected_id, cell_index);

		const auto& path = this->getPath(_selected_id, cell_index);

		for (const auto& i : path)
		{
			_selected->addMovePath(this->getCellCoords(i));
		}

		_selected->moveToPoint(getCellCoords(cell_index));

		////////////////////////////////

		this->unselectCell(old);
		this->setCellFree(old);
	}
}


void Field::draw()
{
	glUseProgram(_line_program);
	glBindVertexArray(_VAO[0]);
	glDrawArrays(GL_LINES, 0, _lines_num);
	glBindVertexArray(0);

	glUseProgram(_point_program);
	glBindVertexArray(_VAO[1]);
	glDrawArrays(GL_POINTS, 0, _points_num);
	glBindVertexArray(0);

	//Draw available free cells
	if (this->haveSelected())
	{
		const size_t size = _field_circles.size();
		if (size > 0u)
		{
			glUseProgram(_circle_program);
			glBindVertexArray(_VAO[2]);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), _field_circles.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_POINTS, 0, (size / 2u));
			glBindVertexArray(0);
		}
	}
}

}