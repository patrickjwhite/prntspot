// tests.cpp
// desc: main function to run tests for prntspot
// vers: 0.01

#include <iostream>
#include <string>
#include <unistd.h>

#include "prntspot.h"

int main() {
	prntspot printer;
	printer.set_left_buffer("Loading:", RED_F, BOLD);
	printer.append_buffer(" [", YELLOW_F, BOLD);
	printer.set_right_buffer("]", YELLOW_F, BOLD);
	printer.print_buffer();
	for (int x=0; x < (printer.max_width() - 11); x++) {
		printer.set_point_buffer("#", BLUE_F, BOLD, 10+x);
		usleep(100000);
		printer.print_buffer();
	}
	printer.new_line();
	printer.reset_buffer();
	printer.set_left_buffer("New line here");
	printer.print_buffer();
	printer.new_line();
}
