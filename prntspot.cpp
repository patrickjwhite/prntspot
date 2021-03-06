// prntspot.cpp
// desc: class function definitions for bufferCh and prntspot engine
// vers: 0.01

#include <sys/ioctl.h>
#include <string>
#include <iostream>

#include "prntspot.h"

bufferCh::bufferCh() {
	next_ch = NULL;
	color = 0;
	style = 0;
}

bufferCh::~bufferCh() {

}

void bufferCh::set_ch(char tmp_ch) {
	ch = tmp_ch;
}

void bufferCh::set_ch(char tmp_ch, int tmp_color, int tmp_style) {
	ch = tmp_ch;
	color = tmp_color;
	style = tmp_style;
}

char bufferCh::return_ch() {
	return ch;
}

void bufferCh::set_next_ch(bufferCh* pCh) {
	next_ch = pCh;
}

bufferCh* bufferCh::return_next_ch() {
	return next_ch;
}

void bufferCh::print_ch() {
	std::cout << "\033[";
	if (color != 0 && style != 0) {
		std::cout << color << ";" << style;
	} else if (color != 0) {
		std::cout << color;
	} else if (style != 0) {
		std::cout << style;
	}
	std::cout << "m" << ch << "\033[0m";
}


prntspot::prntspot() {
	set_window_size();
	first_ch = new bufferCh;
}

prntspot::~prntspot() {
	
}

void prntspot::print_buffer() {
	std::cout << '\r';
	bufferCh* ch = first_ch;
	while (ch->return_ch() != '\0') {
		ch->print_ch();
		ch = ch->return_next_ch();
	}
	std::cout << std::flush;
}

void prntspot::new_line() {
	std::cout << '\n';
	reset_buffer();
}

int prntspot::max_height() {
	return window_height;
}

int prntspot::max_width() {
	return window_width;
}

void prntspot::set_left_buffer(std::string str) {
	if (str.size() <= max_width()) {
		bufferCh* ch = first_ch;
		buffer_len = str.size();
		for (int i=0; i < buffer_len; i++) {
			ch->set_ch(str[i]);
			ch->set_next_ch(new bufferCh);
			ch = ch->return_next_ch();
		}
		ch->set_ch('\0');
	} else {
		throw_error(1);
	}
}

void prntspot::set_left_buffer(std::string str, int color, int style) {
	if (str.size() <= max_width()) {
		bufferCh* ch = first_ch;
		buffer_len = str.size();
		for (int i=0; i < buffer_len; i++) {
			ch->set_ch(str[i], color, style);
			ch->set_next_ch(new bufferCh);
			ch = ch->return_next_ch();
		}
		ch->set_ch('\0');
	} else {
		throw_error(1);
	}
}

void prntspot::append_buffer(std::string str) {
	if (buffer_len + str.size() <= max_width()) {
		bufferCh* ch = first_ch;
		while (ch->return_ch() != '\0') {
			ch = ch->return_next_ch();
		}
		for (int i=0; i < str.size(); i++) {
			ch->set_ch(str[i]);
			ch->set_next_ch(new bufferCh);
			ch = ch->return_next_ch();
		}
		ch->set_ch('\0');
		buffer_len = buffer_len + str.size();
	} else {
		throw_error(1);
	}
}

void prntspot::append_buffer(std::string str, int color, int style) {
	if (buffer_len + str.size() <= max_width()) {
		bufferCh* ch = first_ch;
		while (ch->return_ch() != '\0') {
			ch = ch->return_next_ch();
		}
		for (int i=0; i < str.size(); i++) {
			ch->set_ch(str[i], color, style);
			ch->set_next_ch(new bufferCh);
			ch = ch->return_next_ch();
		}
		ch->set_ch('\0');
		buffer_len = buffer_len + str.size();
	} else {
		throw_error(1);
	}
}

void prntspot::extend_buffer() {
	bufferCh* ch = first_ch;
	while (ch->return_ch() != '\0') {
		ch = ch->return_next_ch();
	}
	for (int i=0; i < (window_width - buffer_len); i++) {
		ch->set_ch(' ');
		ch->set_next_ch(new bufferCh);
		ch = ch->return_next_ch();
	}
	ch->set_ch('\0');
	buffer_len = window_width;
}

void prntspot::set_right_buffer(std::string str) {
	if (str.size() <= max_width()) {
		extend_buffer();
		bufferCh* ch = first_ch;
		for (int n=0; n < (buffer_len - str.size()); n++) {
			ch = ch->return_next_ch();
		}
		for (int i=0; i < str.size(); i++) {
			ch->set_ch(str[i]);
			ch = ch->return_next_ch();
		}
	} else {
		throw_error(1);
	}
}

void prntspot::set_right_buffer(std::string str, int color, int style) {
	if (str.size() <= max_width()) {
		extend_buffer();
		bufferCh* ch = first_ch;
		for (int n=0; n < (buffer_len - str.size()); n++) {
			ch = ch->return_next_ch();
		}
		for (int i=0; i < str.size(); i++) {
			ch->set_ch(str[i], color, style);
			ch = ch->return_next_ch();
		}
	} else {
		throw_error(1);
	}
}

void prntspot::set_point_buffer(std::string str, int offset) {
	if (offset + str.size() <= max_width()) {
		extend_buffer();
		bufferCh* ch = first_ch;
		for (int n=0; n < offset; n++) {
			ch = ch->return_next_ch();
		}
		for (int i=0; i < str.size(); i++) {
			ch->set_ch(str[i]);
			ch = ch->return_next_ch();
		}
	} else {
		throw_error(1);
	}
}

void prntspot::set_point_buffer(std::string str, int color, int style, int offset) {
	if (offset + str.size() <= max_width()) {
		extend_buffer();
		bufferCh* ch = first_ch;
		for (int n=0; n < offset; n++) {
			ch = ch->return_next_ch();
		}
		for (int i=0; i < str.size(); i++) {
			ch->set_ch(str[i], color, style);
			ch = ch->return_next_ch();
		}
	} else {
		throw_error(1);
	}
}

void prntspot::reset_buffer() {
	bufferCh* ch = first_ch;
	first_ch = new bufferCh;
	first_ch->set_ch('\0');
}

void prntspot::set_window_size() {
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	window_width = w.ws_col;
	window_height = w.ws_row;
}

void prntspot::throw_error(int n) {
	std::cout << "ERROR: ";
	switch(n) {
	case 1:
		std::cout << "Buffer exceeds maximum bounds.\n";
		break;
	default:
		std::cout << "Unknown error\n";
		break;
	}
}
