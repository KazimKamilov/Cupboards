#include "Game/Checker.hpp"
#include "Framework/OpenGLErrorCheck.hpp"
#include "Framework/Shader.hpp"
#include "Framework/Utils.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace app
{

Checker::Checker() : 
	_cell_ptr(nullptr),
	_color(1.0f),
	_position(0.0f),
	_checkers_program(0),
	_color_u(0),
	_select_u(0),
	_win_u(0),
	_VAO(0u),
	_VBO(0u),
	_select(false),
	_win(false),
	_anim_speed(0.75f),
	_move(false),
	_path_iter(0u)
{
	framework::Shader v_shader(framework::Shader::VERTEX);
	v_shader.loadFromFile("shaders/checker_vs.glsl");

	framework::Shader g_shader(framework::Shader::GEOMETRY);
	g_shader.loadFromFile("shaders/checker_gs.glsl");

	framework::Shader f_shader(framework::Shader::FRAGMENT);
	f_shader.loadFromFile("shaders/checker_fs.glsl");

	_checkers_program = glCreateProgram();
	glAttachShader(_checkers_program, v_shader.getShaderID());
	glAttachShader(_checkers_program, g_shader.getShaderID());
	glAttachShader(_checkers_program, f_shader.getShaderID());
	glLinkProgram(_checkers_program);

	int result = 0;
	char log[512];

	glGetProgramiv(_checkers_program, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(_checkers_program, 512, nullptr, log);
		std::cout << "OpenGL program link failed! Reason:\n" << log << std::endl;
	}

	_color_u = glGetUniformLocation(_checkers_program, "color");
	_select_u = glGetUniformLocation(_checkers_program, "selected");
	_win_u = glGetUniformLocation(_checkers_program, "win");

	/////////////////////////////////////////////////////////////////////////////////

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), glm::value_ptr(_position), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

	glBindVertexArray(0);

	glCheckErrors();
}


Checker::~Checker()
{
	glDeleteProgram(_checkers_program);
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}


void Checker::addMovePath(const glm::ivec2& path)
{
	_move_path.push_back(path);
}


void Checker::clearPath()
{
	_move_path.clear();
}


void Checker::moveProcess(const double& delta)
{
	if (_move)
	{
		const glm::ivec2& current_position = this->getPosition();

		if(((_direction.x < 0) ? (current_position.x > _current_path.x) : (current_position.x < _current_path.x)) || 
			((_direction.y > 0) ? (current_position.y > _current_path.y) : (current_position.y < _current_path.y))) //Dirty check!;)
		{
			_path_iter++;

			if (_path_iter > _move_path.size() - 1u)
			{
				_move = false;
				_direction = glm::ivec2(0);
				_position = util::ScreenToOpenGL(_current_path);
				_path_iter = 0u;
				_move_path.clear();
				return;
			}

			_current_path = _move_path.at(_path_iter);
			_direction = (_current_path - current_position);
			_direction = glm::ivec2(-_direction.x, _direction.y);
		}
		else
		{
			_position += (glm::normalize(glm::vec2(_direction)) * (_anim_speed * float(delta)));
		}
	}
}


void Checker::moveToPoint(const glm::ivec2& point)
{
	if (!_move_path.empty())
	{
		_move = true;
		_current_path = _move_path.at(_path_iter);
		_direction = (_current_path - this->getPosition());
		_direction = glm::ivec2(-_direction.x, _direction.y);
	}
	else
	{
		this->setPosition(point);
	}
}


void Checker::setCellPtr(void* const cell_ptr)
{
	_cell_ptr = cell_ptr;
}


void Checker::setColor(const glm::vec3& color)
{
	_color = color;
}


void Checker::setPosition(const glm::vec2& position)
{
	_position = util::ScreenToOpenGL(position);
}


const glm::ivec2 Checker::getPosition() const
{
	return util::OpenGLToScreen(_position);
}


const glm::vec3& Checker::getColor() const
{
	return _color;
}


const void* Checker::getCellPtr() const
{
	return _cell_ptr;
}


void Checker::setSelect(const bool select)
{
	_select = select;
}


void Checker::setWin(const bool win)
{
	_win = win;
}


void Checker::toggleSelect()
{
	_select = !_select;
}


const bool Checker::isSelect() const
{
	return _select;
}


const bool Checker::isMoving() const
{
	return _move;
}


const bool Checker::isWin() const
{
	return _win;
}


void Checker::draw()
{
	glUseProgram(_checkers_program);

	glUniform3f(_color_u, _color.x, _color.y, _color.z);
	glUniform1i(_select_u, static_cast<int>(_select));
	glUniform1i(_win_u, static_cast<int>(_win));

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2), glm::value_ptr(_position), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

}