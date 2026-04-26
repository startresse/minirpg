module;

#include <chrono>

export module gamestate;

namespace gamestate
{
    export
    {
        int loop_id = 0;
        std::chrono::microseconds total_elapsed_time;
    }
}
