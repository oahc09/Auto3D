#include "Input.h"
#include "SDL2/SDL.h"
#include "GLWindow.h"
AUTO_BEGIN
SINGLETON_INSTANCE(Input);
Input::Input()
	:_isMouseMove(false)
	,_isLockCursor(false)
{
}


Input::~Input()
{
}

void Input::HandleWindowEvents()
{


}

void Input::Update()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		handleSDLEvent(&evt);
		if(_isLockCursor)
			lockCursor(_lockMousePosition.x, _lockMousePosition.y);
	}
		
}

void Input::handleSDLEvent(void* sdlEvent)
{
	SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		SetKey(evt.key.keysym.sym, true); break;
	case SDL_KEYUP:
		SetKey(evt.key.keysym.sym, false); break;
	case SDL_MOUSEMOTION:
		int x, y;
		SDL_GetMouseState(&x, &y);
		_mousePosition.x = x;
		_mousePosition.y = y;
		if (_isLockCursor) 
		{
			_mouseMove.x = -evt.motion.xrel;
			_mouseMove.y = evt.motion.yrel;
		}
		else
		{
			_mouseMove.x = evt.motion.xrel;
			_mouseMove.y = -evt.motion.yrel;
		}
		_isMouseMove = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		SetWheel(evt.wheel.y);
		break;
	case SDL_QUIT:
		break;
	}
}

void Input::SetKey(int key, bool newState)
{
	if (newState)
	{
		_keysDown.insert(key);
		_keysPress.insert(key);
	}
	else
	{
		_keysPress.erase(key);
	}
}
void Input::SetWheel(int delta)
{
	if (delta)
	{
		_mouseWheelOffset = delta;
		_mouseMoveWheel += delta;
	}
}
void Input::EndFrame()
{
	_keysDown.clear();
	_mouseMove.x = 0;
	_mouseMove.y = 0;
	_isMouseMove = false;
	_mouseWheelOffset = 0;
}

bool Input::GetKeyDown(int key)
{
	return !(_keysDown.find(key) == _keysDown.end());
}
bool Input::GetKeyPress(int key)
{
	return !(_keysPress.find(key) == _keysPress.end());
}
void Input::lockCursor(int x,int y)
{
	SDL_WarpMouseInWindow(INSTANCE(GLWindow).GetGLWindow(), x, y);
}
void Input::LockCursor(int x, int y)
{
	_lockMousePosition.x = x;
	_lockMousePosition.y = y;
	_isLockCursor = true;
}
void Input::LockCursorInCenter()
{
	_lockMousePosition.x = INSTANCE(GLWindow).GetWindowRectInt().width / 2;
	_lockMousePosition.y = INSTANCE(GLWindow).GetWindowRectInt().height / 2;
	_isLockCursor = true;
}
void Input::ShowCursor(bool enable)
{
	if (enable)
		SDL_ShowCursor(SDL_ENABLE);
	else if (!enable)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_QUERY);
}
AUTO_END