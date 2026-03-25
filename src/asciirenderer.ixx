module;

#include <iostream>
#include <Windows.h>

export module asciirenderer;

import gameloop;

namespace asciirenderer
{
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

    export void init()
    {
        windows_console::init();
        std::cout << "Game Start" << std::endl;
    }

    export void update()
    {
        windows_console::clear();
        std::cout << "Loop: " << gameloop::loop_id << std::endl;
    }

}