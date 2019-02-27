#include "../Base/WString.h"
#include "../Debug/Log.h"
#include "../Math/Math.h"
#include "Input.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_scancode.h>
#include <SDL_syswm.h>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Window::Window() :
	_handle(nullptr),
	_title("Auto3D Window"),
	_size(Vector2I::ZERO),
	_savedPosition(Vector2I(M_MIN_INT, M_MIN_INT)),
	_mousePosition(Vector2I::ZERO),
	_mouseWheelOffset(Vector2I::ZERO),
	_mouseMoveWheel(Vector2I::ZERO),
	_windowStyle(0),
	_minimized(false),
	_focus(false),
	_resizable(false),
	_fullscreen(false),
	_inResize(false),
	_mouseHide(false),
	_mouseLock(false),
	_mouseVisibleInternal(true)
{
	RegisterSubsystem(this);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ErrorString("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

Window::~Window()
{
	Close();
	RemoveSubsystem(this);
}
void Window::SetTitle(const String& newTitle)
{
	_title = newTitle;
}

bool Window::SetSize(const Vector2I& size, bool fullscreen, bool resizable, bool borderless, bool highDPI)
{
	_size = size;
	// Create the _window
	unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (fullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable)
		flags |= SDL_WINDOW_RESIZABLE;
	if (borderless)
		flags |= SDL_WINDOW_BORDERLESS;


	// The _position _size will be reset later
	_handle = SDL_CreateWindow(
		_title.CString(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		size._x, size._y, flags
	);

	if (_handle == NULL)
		ErrorString("Couldn't set video mode");

	return true;
}

void Window::SetPosition(const Vector2I& position)
{

}

void Window::SetMouseHide(bool enable)
{
	if (enable != _mouseHide)
	{
		_mouseHide = enable;
		
	}
}

void Window::SetMouseLock(bool enable)
{
	if (enable != _mouseLock)
	{
		_mouseLock = enable;
		SDL_SetWindowGrab(_handle, (SDL_bool)enable);
	}
}

void Window::SetMousePosition(const Vector2I& position)
{

}

void Window::Close()
{

}

void Window::Minimize()
{

}

void Window::Maximize()
{

}

void Window::Restore()
{

}

void Window::PumpMessages()
{
	if (!_handle)
		return;

	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		OnWindowMessage(&evt);
		
	}
}


Vector2I Window::Position() const
{
	return Vector2I();
}

bool Window::OnWindowMessage(void* sdlEvent)
{
	auto* input = GetSubsystem<Input>();

	SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		if (input)
			input->OnKey(evt.key.keysym.sym, (evt.key.keysym.sym >> 16) & 0xff, true);
		break;
	case SDL_KEYUP:
		if (input)
			input->OnKey(evt.key.keysym.sym, (evt.key.keysym.sym >> 16) & 0xff, false);
		break;
	case SDL_MOUSEMOTION:
		int x, y;
		SDL_GetMouseState(&x, &y);
		_mousePosition._x = x;
		_mousePosition._y = y;
		if (input)
			input->OnMouseMove(_mousePosition, Vector2I(evt.motion.xrel, evt.motion.yrel));
		//_isMouseMove = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		if (input)
			input->OnMouseWheel(Vector2I(evt.wheel.x, evt.wheel.y));
		break;
	case SDL_WINDOWEVENT:
	{
		switch (evt.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			//_isMinimized = true;
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			//_isMinimized = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			int x, y;
			SDL_GetWindowSize(_handle, &x, &y);
			SendEvent(resizeEvent);
			if (Vector2I(x, y) != _size) 
			{
				_size = Vector2I(x, y);
				resizeEvent._size = Vector2I(x, y);
				SendEvent(resizeEvent);
			}
			break;
		case SDL_WINDOWEVENT_MOVED:
			//_graphics->OnWindowMoved();
			break;

		default: break;
		}
	}
	break;
	case SDL_QUIT:
		return false;
		break;
	}
	//Handles whether the mouse is visible
	SDL_SetRelativeMouseMode((SDL_bool)_mouseHide);

	return true;
}

}