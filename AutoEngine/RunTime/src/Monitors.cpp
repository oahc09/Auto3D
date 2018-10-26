#include "Monitors.h"
#include "SDL2/SDL.h"
namespace Auto3D {

Monitors::Monitors()
{
	SDL_Rect rect;
	_monitorCount = SDL_GetNumVideoDisplays();
	for (int i = 0; i < _monitorCount; i++)
	{
		SDL_GetDisplayBounds(i, &rect);
		_mnitors.EmplaceBack(M_PAIR(rect.w, rect.h));
	}
}

Int32 Monitors::GetMonitorsCount()
{
	return _monitorCount;
}

Int32 Monitors::GetMonitorsHeight(int index)
{
	if (index > _monitorCount - 1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return _mnitors.At(index).second;
}

Int32 Monitors::GetMonitorsWidth(int index)
{
	if (index > _monitorCount - 1)
	{
		WarningString("Fail to get monitors with index(Maybe index beyond the limit)");
		return 0;
	}
	return _mnitors.At(index).first;
}

RectInt Monitors::GetMonitorsSize(int index)
{
	if (index > _monitorCount - 1)
	{
		WarningString("Fail to get monitors size index(Maybe index beyond the limit)");
	}
	RectInt rectSize;
	rectSize.width = _mnitors.At(index).first;
	rectSize.height = _mnitors.At(index).second;
	return rectSize;
}

}