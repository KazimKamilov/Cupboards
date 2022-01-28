#include "Framework/Window.hpp"
#include <iostream>
#include <string>
#include "GL/gl3w.h"


namespace framework
{

Window::Window() :
	_window(nullptr),
	_additive_window(nullptr),
	_context(nullptr),
	_working(true),
	_background(0.01f, 0.0f, 0.075f)
{
	std::cout << "====================== Cupboards test app =======================" << std::endl;

	constexpr std::size_t SDL_FLAGS = SDL_INIT_VIDEO;

	if (SDL_Init(SDL_FLAGS) != 0)
	{
		std::cout << "Cannot init SDL2 library!" << std::endl;
		throw std::runtime_error("Cannot init SDL2 library! Reason:\n" + std::string(SDL_GetError()));
		SDL_Quit();
	}
	else
	{
		std::cout << "SDL2 started!" << std::endl;
	}

	constexpr std::size_t VIDEO_FLAGS = SDL_WINDOW_OPENGL |
									SDL_WINDOW_INPUT_FOCUS |
									SDL_WINDOW_MOUSE_FOCUS |
									SDL_WINDOW_MOUSE_CAPTURE |
									SDL_WINDOW_SHOWN;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	_window = SDL_CreateWindow("Cupboards", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, VIDEO_FLAGS);
	if (_window != nullptr)
	{
		std::cout << "SDL2 window created!" << std::endl;
		_context = SDL_GL_CreateContext(_window);
		SDL_GL_MakeCurrent(_window, _context);

		//Set VSync on...
		SDL_GL_SetSwapInterval(1);

		//Get main window position
		SDL_GetWindowPosition(_window, &_additive_pos.x, &_additive_pos.y);

		//Creating additive window
		_additive_window = SDL_CreateWindow("Winning condition", _additive_pos.x - 216, _additive_pos.y, 200, 200, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_TOOLTIP);
		if (_additive_window != nullptr)
		{
			std::cout << "SDL2 additive window created!" << std::endl;
			SDL_GL_MakeCurrent(_additive_window, _context);

			//Set VSync on...
			SDL_GL_SetSwapInterval(1);

			//Focused on main window
			SDL_SetWindowInputFocus(_window);
		}
		else
		{
			std::cout << "Cannot create additive SDL2 window!" << std::endl;
			throw std::runtime_error("Cannot create additive SDL2 window! Reason:\n" + std::string(SDL_GetError()));
			SDL_Quit();
		}

		if (gl3wInit())
		{
			std::cout << "Cannot ini GL3W!" << std::endl;
		}
		else
		{
			std::cout << "GL3W inited!" << std::endl;
		}

		if (gl3wIsSupported(3, 3))
		{
			std::cout << "OpenGL 3.3 core are supported!" << std::endl;
		}
		else
		{
			std::cout << "OpenGL 3.3 core not supported!" << std::endl;
			SDL_Quit();
		}

		std::cout << "======================== OpenGL info ============================" << std::endl;
		std::cout << "OpenGL version used: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "=================================================================" << std::endl;
	}
	else
	{
		std::cout << "Cannot create SDL2 window!" << std::endl;
		throw std::runtime_error("Cannot create SDL2 window! Reason:\n" + std::string(SDL_GetError()));
		SDL_Quit();
	}
}


Window::~Window()
{
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_additive_window);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}


void Window::setBackgroundColor(const glm::vec3& color)
{
	_background = color;
}


SDL_Event* Window::getEvent()
{
	return &_event;
}


SDL_Window* Window::getSDLWindow()
{
	return _window;
}


void Window::update()
{
	while (SDL_PollEvent(&_event))
	{
		SDL_PumpEvents();

		switch (_event.type)
		{
			case SDL_WINDOWEVENT:
			{
				switch (_event.window.event)
				{
					case SDL_WINDOWEVENT_MOVED:
						SDL_GetWindowPosition(_window, &_additive_pos.x, &_additive_pos.y);
						SDL_SetWindowPosition(_additive_window, _additive_pos.x - 216, _additive_pos.y);
					break;
				}
			}
			break;

			case SDL_KEYDOWN:
			{
				switch (_event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						_working = false;
					break;

					default:

					break;
				}
			}
			break;

			default:

			break;
		}
	}

	glClearColor(_background.x, _background.y, _background.z, 1.0f);
	
	//This function allocates additional memory. Leak?
	SDL_GL_SwapWindow(_window);
}


void Window::updateAdditive()
{
	glClearColor(_background.x, _background.y, _background.z, 1.0f);
	SDL_GL_SwapWindow(_additive_window);
}


void Window::PreUpdateMainWindow()
{
	SDL_GL_MakeCurrent(_window, _context);
}


void Window::PreUpdateAdditiveWindow()
{
	SDL_GL_MakeCurrent(_additive_window, _context);
}


const bool Window::isWorking() const
{
	return _working;
}


void Window::loadCursor(const std::string& cursor_bmp_file)
{
	SDL_ShowCursor(0);

	SDL_Surface* img = SDL_LoadBMP(cursor_bmp_file.c_str());
	SDL_Cursor* cursor = SDL_CreateColorCursor(img, 0, 0);

	SDL_SetCursor(cursor);

	SDL_ShowCursor(1);
}

}