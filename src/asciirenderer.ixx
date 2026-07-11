module;

#include <Windows.h>
#include <chrono>
#include <iostream>

export module asciirenderer;

namespace asciirenderer
{
    export
    {
        void init();
        void update();

        bool framerate_allow_new_frame();
    }

    namespace windows_console
    {
        // source: https://learn.microsoft.com/en-gb/windows/console/clearing-the-screen
        static DWORD init()
        {
            HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

            // Fetch existing console mode so we correctly add a flag and not turn off others
            DWORD mode = 0;
            if (!GetConsoleMode(hStdOut, &mode))
            {
                return ::GetLastError();
            }

            // Hold original mode to restore on exit to be cooperative with other command-line apps.
            DWORD originalMode = mode;
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

            // Try to set the mode.
            if (!SetConsoleMode(hStdOut, mode))
            {
                return ::GetLastError();
            }

            // Write the sequence for clearing the display.
            DWORD written = 0;
            constexpr PCWSTR sequence = L"\x1b[2J";
            if (!WriteConsoleW(hStdOut, sequence, (DWORD)wcslen(sequence), &written, NULL))
            {
                // If we fail, try to restore the mode on the way out.
                SetConsoleMode(hStdOut, originalMode);
                return ::GetLastError();
            }

            // To also clear the scroll back, emit L"\x1b[3J" as well.
            // 2J only clears the visible window and 3J only clears the scroll back.

            // Restore the mode on the way out to be nice to other command-line applications.
            SetConsoleMode(hStdOut, originalMode);

            return 0;
        }

        static void clear()
        {
            // [2J: Erase In Display: fill to bottom with empty line
            // [H: Home: Get cursor to beginning
            // [3J: Clear history
            wprintf(L"\x1b[2J\x1b[H\x1b[3J");
        }
    }

    using clock = std::chrono::high_resolution_clock;

    std::optional<float> max_framerate = 24.f;
    std::chrono::steady_clock::time_point time_point_last_frame;

    void init()
    {
        windows_console::init();
        time_point_last_frame = clock::now();
    }

    void update()
    {
        windows_console::clear();
        time_point_last_frame = clock::now();
    }

    bool framerate_allow_new_frame()
    {
        using namespace std::chrono;
        if (!max_framerate.has_value())
            return true;

        float framerate = max_framerate.value();
        const auto min_time_between_frame = round<nanoseconds>(duration<float>{1.f / framerate});
        return (clock::now() - time_point_last_frame > min_time_between_frame);
    }

}