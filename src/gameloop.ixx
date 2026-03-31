module;

#include <iostream>

export module gameloop;

import asciirenderer;
import gamestate;
import input;

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
        gamestate::loop_id = 0;

        asciirenderer::init();
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

            asciirenderer::update();

            pause_state::update();

            ++gamestate::loop_id;
        }
    }

} // namespace gameloop
