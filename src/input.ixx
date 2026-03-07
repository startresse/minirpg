module;

#include <Windows.h>
#include <list>

export module input;

// this namespace is an abstraction layer that allows to change the input backend without having to modify game code.
namespace input
{

using Key = unsigned int;
export namespace key
{
constexpr Key esc = VK_ESCAPE;
constexpr Key tab = VK_TAB;
constexpr Key q = 'Q';
}

export bool check_pressed(Key key)
{
    short state = GetKeyState(key);
    return state & 0x8000;
}

} // namespace input
