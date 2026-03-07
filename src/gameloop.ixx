module;

#include <iostream>

export module gameloop;
import input;

namespace gameloop
{

    bool is_running = true;

    export void init()
    {
        input::init();
    }

    static void check_inputs()
    {
        if (input::is_first_pressed(input::Key::q))
        {
            is_running = false;
        }
    }

    export void run()
    {
        static int loop_id = 0;
        while (is_running)
        {
            input::update();
            check_inputs();
            printf("Loop %d\n", loop_id++);
        }
    }

} // namespace gameloop
