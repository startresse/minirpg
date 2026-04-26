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

    bool is_running = true;

    namespace pause_state
    {
        bool is_freerun = false;
        bool is_trigger_step = false;
        bool just_left_trigger_step = false;

        static bool can_step()
        {
            return (is_freerun || is_trigger_step) && asciirenderer::framerate_allow_new_frame();
        }
        static void swap_state()
        {
            is_freerun = !is_freerun;
            is_trigger_step = false;
            just_left_trigger_step = true;
        }
        static void trigger_step() { is_trigger_step = !is_freerun; }
        static void update()
        {
            is_trigger_step = false;
            just_left_trigger_step = false;
        }
    };

    namespace time
    {
        using namespace std::chrono;
        using clock = high_resolution_clock;
        clock::time_point current_frame_data;
        clock::time_point last_frame_data;
        microseconds manual_delta_time = milliseconds(100);

        static microseconds delta_time()
        {
            return duration_cast<microseconds>(current_frame_data - last_frame_data);
        }

        static auto now()
        {
            return clock::now();
        }

        static void update_total_elapsed_time()
        {
            if (pause_state::is_trigger_step || pause_state::just_left_trigger_step)
                gamestate::total_elapsed_time += time::manual_delta_time;
            else
                gamestate::total_elapsed_time += time::delta_time();
        }
    }

    void init()
    {
        gamestate::loop_id = 0;
        gamestate::total_elapsed_time = std::chrono::milliseconds::zero();
        time::current_frame_data = time::last_frame_data = time::now();

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

            time::current_frame_data = time::now();
            time::update_total_elapsed_time();
            asciirenderer::update();
            ui::display_frame();

            pause_state::update();

            ++gamestate::loop_id;
            time::last_frame_data = time::current_frame_data;
        }
    }

}
