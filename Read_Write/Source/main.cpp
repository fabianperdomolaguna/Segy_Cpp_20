#include <iostream>
#include <format>

import utilities;
import segy;

void textual_header_menu(SegyFile& segy_struct) {
	clear_screen();
	std::cout << std::format("{:-^30}\n", "Menu");
	std::cout << std::format("{}\n", "1. Print textual header");
	std::cout << std::format("{}\n", "2. Save textual header");
	std::cout << std::format("{}\n", "3. Replace textual header");
	std::cout << std::format("{:-^30}\n", "");

	switch (get_user_option<uint16_t>("Enter option number: ")) {
	case 1: segy::print_textual_header(segy_struct); break;
	case 2: segy::save_textual_header(segy_struct); break;
	case 3: segy::replace_textual_header(segy_struct); break;
	default: std::cout << "You enter an invalid option"; exit(1);
	}
}

void trace_header_menu(SegyFile& segy_struct) {
	clear_screen();
	std::cout << std::format("{:-^30}\n", "Menu");
	std::cout << std::format("{}\n", "1. Print XY coordinates");
	std::cout << std::format("{}\n", "2. Save XY coordinates");
	std::cout << std::format("{}\n", "3. Replace coordinates");
	std::cout << std::format("{:-^30}\n", "");

	switch (get_user_option<uint16_t>("Enter option number: ")) {
	case 1: segy::print_coord(segy_struct); break;
	case 2: segy::save_coord(segy_struct); break;
	case 3: segy::replace_coord(segy_struct); break;
	default: std::cout << "You enter an invalid option"; exit(1);
	}
}

void main_menu(SegyFile& segy_struct) {
	clear_screen();
	std::cout << std::format("{:-^30}\n", "Main Menu");
	std::cout << std::format("{}\n", "1. SEG-Y file summary");
	std::cout << std::format("{}\n", "2. Textual header");
	std::cout << std::format("{}\n", "3. Trace header");
	std::cout << std::format("{:-^30}\n", "");

	switch (get_user_option<uint16_t>("Enter option number: ")) {
	case 1: segy::print_main_info(segy_struct); break;
	case 2: textual_header_menu(segy_struct); break;
	case 3: trace_header_menu(segy_struct); break;
	default: std::cout << "You enter an invalid option"; exit(1);
	}
}

int main() {
	clear_screen();
	auto segy_struct = segy::segy_struct_construct();
	main_menu(segy_struct);
}