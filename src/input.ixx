module;

#include <Windows.h>
#include <array>

export module input;

// this namespace is an abstraction layer that allows to change the input backend without having to modify game code.
namespace input
{
    export enum class Key
    {
        esc,
        tab,
        space,
        q,
        COUNT, // TODO find a better way?
    };
    std::array<unsigned int, static_cast<size_t>(Key::COUNT)> key_codes = { VK_ESCAPE, VK_TAB, VK_SPACE, 'Q'};
    unsigned int get_keycode(const Key& key)
    {
        return key_codes[static_cast<unsigned int>(key)]; // find a better way that this cast
    }

    static bool is_currently_pressed(const Key& key)
    {
        short state = GetKeyState(get_keycode(key));
        return state & 0x8000;
    }

    using KeyPressedArray = std::array<bool, static_cast<size_t>(Key::COUNT)>;
    std::array<KeyPressedArray, 2> keys_pressed;
    unsigned int current_array_id = 0; // too staty ?
    static bool is_pressed_in_array(unsigned int array_id, const Key& key) { return keys_pressed[array_id][static_cast<unsigned int>(key)]; }
    static bool is_pressed_in_current_frame(const Key& key) { return is_pressed_in_array(current_array_id, key); }
    static bool is_pressed_in_pred_frame(const Key& key) { return is_pressed_in_array((current_array_id + 1) % 2, key); }

    static void clear()
    {
        for (KeyPressedArray& array : keys_pressed)
        {
            array.fill(false);
        }
    }

    export void init()
    {
        clear();
    }

    export void update()
    {
        current_array_id = (current_array_id + 1) % 2;
        for (int ikey = 0; ikey < keys_pressed[0].size(); ++ikey)
        {
            keys_pressed[current_array_id][ikey] = is_currently_pressed(static_cast<Key>(ikey));
        }
    }

    export bool is_pressed(const Key& key)
    {
        return is_pressed_in_current_frame(key);
    }
    export bool is_first_pressed(const Key& key)
    {
        return is_pressed_in_current_frame(key) && !is_pressed_in_pred_frame(key);
    }

} // namespace input
