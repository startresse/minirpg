module;

#include <cassert>
#include <chrono>
#include <iostream>

export module ui;

import gamestate;

namespace ui
{
    export
    {
        void init();
        void display_frame();
    }

    constexpr int width = 100;

    static void display_progress_bar(float progress)
    {
        assert(0.f <= progress && progress <= 1.f);
        const int nb_filled = width / int(progress);
        const int nb_unfilled = width - nb_filled;
        std::cout << "[" << std::string('=', nb_filled) << std::string(' ', nb_unfilled) << "]" << std::endl;
    }

    void init()
    {
        std::cout << "Game Start" << std::endl;
    }

    void display_frame()
    {
        const auto h = std::chrono::duration_cast<std::chrono::hours>(gamestate::total_elapsed_time);
        const auto m = std::chrono::duration_cast<std::chrono::minutes>(gamestate::total_elapsed_time - h);
        const auto s = std::chrono::duration_cast<std::chrono::seconds>(gamestate::total_elapsed_time - h - m);
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(gamestate::total_elapsed_time - h - m - s);
        std::cout << "Loop: " << gamestate::loop_id << std::endl;
        std::cout << "Total Elapsed Time:" << h << " " << m << " " << s << " " << ms << std::endl;
    }
}
