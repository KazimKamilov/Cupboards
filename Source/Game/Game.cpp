#include "Game/Game.hpp"
#include "GL/gl3w.h"
#include "Game/ConfigFile.hpp"
#include <iostream>
#include "glm/glm.hpp"
#include "Framework/Utils.hpp"


namespace app
{

Game::Game() : 
	_current_time(0ull),
	_last_time(0ull),
	_delta_time(0.0),
	_window(nullptr),
	_field(nullptr),
	_mini(nullptr),
	_select_rect(80.0f),
	_config("game.cfg")
{
	_window = new framework::Window;
	_window->loadCursor("data/cursor.bmp");

	//This needed for OpenGL transparent support.. 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Game::~Game()
{
}


void Game::run()
{
	const auto checkers_num = _config.getCheckerNum();

	if (checkers_num > 255u)
	{
		std::cout << "Maximum of checkers - 255! Sorry, but thats enough!" << std::endl;

		delete _window;
		_window = nullptr;

		SDL_Quit();

		delete this;
	}

	const auto cells_num = _config.getAvailableCells();

	if (cells_num > 255u)
	{
		std::cout << "Maximum of cells - 255! Sorry, but thats enough!" << std::endl;

		delete _window;
		_window = nullptr;

		SDL_Quit();

		delete this;
	}

	if (checkers_num != _config.getStartPosList().size()) std::cout << "Hey! Why the hell does the number of checkers not match the number of starting positions?!" << std::endl;
	if (checkers_num != _config.getWinPosList().size()) std::cout << "Hey! Why the hell does the number of checkers not match the number of winning positions?!" << std::endl;
	
	std::vector<glm::vec3> _winning_colors(cells_num, glm::vec3(0.0f));
	std::vector<glm::vec3> temp_colors;

	for (auto i = 0u; i < checkers_num; i++)
	{
		const auto& clr = util::RandomColor();
		const unsigned int index = _config.getWinPosList().at(i);
		_winning_colors.at(index) = clr;
		temp_colors.push_back(clr);
	}

	_field = new app::Field(_config, _winning_colors);

	for (auto i = 0u; i < checkers_num; i++)
	{
		Checker* chk = new Checker;
		const unsigned int index = _config.getStartPosList().at(i);
		chk->setColor(temp_colors.at(i));
		_field->setCellChecker(index, chk);
		chk->setPosition(_field->getCellCoords(index));
		_checkers.push_back(chk);
	}

	_mini = new app::MiniField(_config, _field->getCellCoordsArray(), _winning_colors);

	std::cout << "Press \"ESCAPE\" to exit from game..." << std::endl;
}


const bool Game::isIntersect(const glm::vec2& position, const glm::vec2& rect) const
{
	if (((_mouse.x > (position.x - (rect.x * 0.5f))) && (_mouse.x < (position.x + (rect.x * 0.5f)))) && ((_mouse.y > (position.y - (rect.y * 0.5f))) && (_mouse.y < (position.y + (rect.y * 0.5f)))))
	{
		return true;
	}
	else
	{
		return false;
	}
}


const bool Game::isClicked(const glm::vec2& position, const glm::vec2& rect) const
{
	SDL_Event* evt = _window->getEvent();
	if (this->isIntersect(position, rect) && (evt->button.type == SDL_MOUSEBUTTONDOWN) && SDL_WaitEventTimeout(evt, evt->button.timestamp)) return true;
	else return false;
}


void Game::update()
{
	while (_window->isWorking())
	{
		_last_time = _current_time;
		_current_time = SDL_GetPerformanceCounter();

		////////////////////////////////////////////////
		//Additive window update
		////////////////////////////////////////////////

		_window->PreUpdateAdditiveWindow();

		glClear(GL_COLOR_BUFFER_BIT);

		_mini->draw();

		_window->updateAdditive();

		////////////////////////////////////////////////
		//Main window update
		////////////////////////////////////////////////

		_window->PreUpdateMainWindow();

		glClear(GL_COLOR_BUFFER_BIT);

		//Here we get mouse position.
		SDL_GetMouseState(&_mouse.x, &_mouse.y);

		_field->draw();

		for (auto i = 0u; i < _field->getTreeSize(); i++)
		{
			if (!_field->isCellFree(i))
			{
				if (_field->getChecker(i)->getColor() == _field->getTreeCell(i)->getWinColor())
				{
					_field->getChecker(i)->setWin(true);
				}
				else
				{
					_field->getChecker(i)->setWin(false);
				}
			}
		
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (isClicked(_field->getCellCoords(i), _select_rect))
			{
				if (!_field->isCellFree(i))
				{
					if (_field->haveSelected()) _field->unselectCell(i);
					else
					{
						if(!_field->getChecker(i)->isMoving()) _field->selectCell(i);
					}
				}
				else
				{
					_field->moveSelectedTo(i);
				}
			}
		}


		for (auto& checker : _checkers)
		{
			checker->moveProcess(_delta_time);
			checker->draw();
		}

		_window->update();

		_last_time = SDL_GetPerformanceCounter();
		_delta_time = (static_cast<double>(_current_time) - static_cast<double>(_last_time)) / static_cast<double>(SDL_GetPerformanceFrequency());
	}
}


void Game::destroy()
{
	for (auto& checker : _checkers)
	{
		if (checker != nullptr)
		{
			delete checker;
			checker = nullptr;
		}
	}

	delete _field;
	_field = nullptr;

	delete _mini;
	_mini = nullptr;

	delete _window;
	_window = nullptr;
}

}