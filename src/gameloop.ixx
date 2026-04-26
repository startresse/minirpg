module;

#include <chrono>
#include <iostream>

export module gameloop;

import asciirenderer;
import gamestate;
import input;
import ui;

namespace gameloop
{
    export
    {
        void init();
        void run();
    }

    using clock = std::chrono::high_resolution_clock;

    bool is_running = true;
    std::chrono::steady_clock::time_point game_start_time;

    namespace pause_state
    {
        bool is_freerun = false;
        bool is_trigger_step = false;

        static bool can_step()
        {
            return (is_freerun || is_trigger_step) && asciirenderer::framerate_allow_new_frame();
        }
        static void swap_state()
        {
            is_freerun = !is_freerun;
            is_trigger_step = false;
        }
        static void trigger_step() { is_trigger_step = !is_freerun; }
        static void update() { is_trigger_step = false; }
    };

    void init()
    {
        game_start_time = clock::now();

        gamestate::loop_id = 0;
        gamestate::total_elapsed_time = std::chrono::milliseconds::zero();

        asciirenderer::init();
        ui::init();
        input::init();
    }

    static void process_inputs()
    {
        if (input::is_released(input::Key::q))
            is_running = false;
        if (input::is_first_pressed(input::Key::space))
            pause_state::swap_state();
        if (input::is_first_pressed(input::Key::tab))
            pause_state::trigger_step();
    }

    void run()
    {
        while (is_running)
        {
            input::update();
            process_inputs();
            if (!pause_state::can_step())
                continue;

            gamestate::total_elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - game_start_time);
            asciirenderer::update();
            ui::display_frame();

            pause_state::update();

            ++gamestate::loop_id;
        }
    }

}
