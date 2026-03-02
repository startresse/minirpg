module;

#include <Windows.h>

export module input;

namespace input
{

export bool check_pressed(char key)
{
    short state = GetKeyState(key);
    return state & 0x8000;
}

} // namespace input
