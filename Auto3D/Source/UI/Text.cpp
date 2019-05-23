#include "Text.h"
#include "UI.h"
#include <imgui.h>

#include "../Debug/Log.h"

namespace Auto3D
{

Text::Text():
	_text(String::EMPTY),
	_color(UIStyleColors::Text)
{
}

Text::~Text() = default;

void Text::RegisterObject()
{
	RegisterFactory<Text>();
}

void Text::SetText(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	_text =	String::FormatV(fmt, args);
	va_end(args);

}
void Text::SetText(const String& fmt, ...)
{
	va_list args;
	const char* tFmt = fmt.CString();
	va_start(args, tFmt);
	_text = String::FormatV(tFmt, args);
	va_end(args);
}

void Text::SetColor(const Color& color)
{
	_color = color;
}

void Text::DefineNode()
{
	ImVec4 vec(_color._r, _color._g, _color._b, _color._a);
	ImGui::TextColored(vec,_text.CString());
}

}