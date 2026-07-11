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
        const int nb_filled = int(progress * width);
        std::string progressbar_filled(nb_filled, '=');
        std::string progressbar_empty(width - nb_filled - 1, '-');
        std::cout << "[" << progressbar_filled << progressbar_empty << "]" << std::endl;
    }

    void init()
    {
        std::cout << "Game Start" << std::endl;
    }

    void print_elapsed_time()
    {
        const auto h = std::chrono::duration_cast<std::chrono::hours>(gamestate::total_elapsed_time);
        const auto m = std::chrono::duration_cast<std::chrono::minutes>(gamestate::total_elapsed_time - h);
        const auto s = std::chrono::duration_cast<std::chrono::seconds>(gamestate::total_elapsed_time - h - m);
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(gamestate::total_elapsed_time - h - m - s);
        std::cout << "Loop: " << gamestate::loop_id << std::endl;
        std::cout << "Total Elapsed Time:" << h << " " << m << " " << s << " " << ms << std::endl;
    }

    void display_frame()
    {
        print_elapsed_time();
        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(gamestate::total_elapsed_time);
        float progress = static_cast<float>(elapsed_seconds.count()) / std::chrono::milliseconds(16000).count();
        display_progress_bar(fmod(progress * 3.928, 1.f));
        display_progress_bar(fmod(progress * 2.121, 1.f));
        display_progress_bar(fmod(progress    , 1.f));
        display_progress_bar(fmod(progress / 1.98, 1.f));
        display_progress_bar(fmod(progress / 4.23, 1.f));
        for (int i = 0; i < 10; ++i)
            std::cout << std::endl;
    }
}
