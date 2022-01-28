#include "Game/MiniField.hpp"
#include "Framework/OpenGLErrorCheck.hpp"
#include "Framework/Shader.hpp"
#include "Framework/Utils.hpp"


namespace app
{

MiniField::MiniField(const ConfigFile& cfg, const std::vector<glm::ivec2>& coords, const std::vector<glm::vec3>& colors) :
	_VAO{0u, 0u},
	_VBO{0u, 0u},
	_point_program(0),
	_line_program(0)
{
	/////////////////////////////////////////////////////////////////////////////////
	//MiniField creation
	/////////////////////////////////////////////////////////////////////////////////

	for (auto i = 0u; i < cfg.getAvailableCells(); i++)
	{
		const auto& crd = util::ScreenToMiniOpenGL(coords.at(i));
		_field_points.push_back(crd.x);
		_field_points.push_back(crd.y);

		const auto& clr = colors.at(i);

		_field_points.push_back(clr.x);
		_field_points.push_back(clr.y);
		_field_points.push_back(clr.z);
	}

	const auto& joints = cfg.getJointList();

	for (const auto& i : joints)
	{
		_field_lines.push_back(util::ScreenToMiniOpenGL(coords.at(i.first)));
		_field_lines.push_back(util::ScreenToMiniOpenGL(coords.at(i.second)));
	}

	_points_num = static_cast<GLsizei>(_field_points.size());
	_lines_num = static_cast<GLsizei>(_field_lines.size());


	{
		framework::Shader v_shader(framework::Shader::VERTEX);
		v_shader.loadFromFile("shaders/minifield_lines_vs.glsl");

		framework::Shader f_shader(framework::Shader::FRAGMENT);
		f_shader.loadFromFile("shaders/minifield_lines_fs.glsl");

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

		glGenVertexArrays(2, _VAO);
		glGenBuffers(2, _VBO);

		glBindVertexArray(_VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * _field_lines.size(), _field_lines.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

		glBindVertexArray(0);

		glCheckErrors();

	}

	//////////////////////////////////////////////////////////////////////////////////

	{
		framework::Shader v_shader(framework::Shader::VERTEX);
		v_shader.loadFromFile("shaders/minifield_vs.glsl");

		framework::Shader g_shader(framework::Shader::GEOMETRY);
		g_shader.loadFromFile("shaders/minifield_gs.glsl");

		framework::Shader f_shader(framework::Shader::FRAGMENT);
		f_shader.loadFromFile("shaders/minifield_fs.glsl");

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
		glBufferData(GL_ARRAY_BUFFER, _field_points.size() * sizeof(float), _field_points.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void*>(2 * sizeof(float)));

		glBindVertexArray(0);

		glCheckErrors();
	}

	/////////////////////////////////////////////////////////////////////////////////
}


MiniField::~MiniField()
{
	glDeleteProgram(_point_program);
	glDeleteProgram(_line_program);
	glDeleteBuffers(2, _VBO);
	glDeleteVertexArrays(2, _VAO);
}


void MiniField::draw()
{
	glUseProgram(_line_program);
	glBindVertexArray(_VAO[0]);
	glDrawArrays(GL_LINES, 0, _lines_num);
	glBindVertexArray(0);

	glUseProgram(_point_program);
	glBindVertexArray(_VAO[1]);
	glDrawArrays(GL_POINTS, 0, _points_num);
	glBindVertexArray(0);
}

}