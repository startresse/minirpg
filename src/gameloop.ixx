module;

#include <iostream>

export module gameloop;
import input;

namespace gameloop
{

bool is_running = true;

void check_inputs()
{
	if (input::check_pressed(0x1B)) // ESC
	{
		is_running = false;
	}
}

export void run()
{
	static int loop_id = 0;
	while (is_running)
	{
		check_inputs();
		printf("Loop %d\n", loop_id++);
	}
}

} // namespace gameloop
