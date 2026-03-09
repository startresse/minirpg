module;

// API includes
#include <Windows.h>

// utility includes
#include <array>
#include <cassert>
#include <unordered_map>

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
    using KeyCode = unsigned int;
    const std::unordered_map<Key, KeyCode> key_codes
    {
        {Key::esc, VK_ESCAPE},
        {Key::tab, VK_TAB},
        {Key::space, VK_SPACE},
        {Key::q, 'Q'},
    };

    static bool is_currently_pressed(const Key& key)
    {
        assert(key != Key::COUNT);
        short state = GetKeyState(key_codes.at(key));
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
        // Ensure map has all Key implemented
        // (Can't static_assert because std::unordered_map can't be constexpr)
        assert(key_codes.size() == static_cast<size_t>(Key::COUNT));

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
    export bool is_released(const Key& key)
    {
        return is_pressed_in_pred_frame(key) && !is_pressed_in_current_frame(key);
    }

} // namespace input
