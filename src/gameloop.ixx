module;

#include <iostream>

export module gameloop;
import input;

namespace gameloop
{

    bool is_running = true;
    namespace pause_state
    {
        bool is_freerun = false;
        bool is_trigger_step = false;

        static bool can_step() { return is_freerun || is_trigger_step; }
        static void swap_state()
        {
            is_freerun = !is_freerun;
            is_trigger_step = false;
        }
        static void trigger_step() { is_trigger_step = !is_freerun; }
        static void update() { is_trigger_step = false; }
    };

    export void init()
    {
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

    export void run()
    {
        static int loop_id = 0;
        while (is_running)
        {
            input::update();
            process_inputs();
            if (!pause_state::can_step())
                continue;

            pause_state::update();

            printf("Loop %d\n", loop_id++);
        }
    }

} // namespace gameloop
