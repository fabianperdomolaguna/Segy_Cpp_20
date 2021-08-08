#include <compare>
#include <iostream>
#include <format>
#include <fstream>
#include <unordered_map>
#include <thread>

import utilities;
import bytes;

export module segy:traceheader;
import :construct;

template <typename T, T f(std::fstream&)>
struct traceheader_iterator {
	void operator()(SegyFile& segy_struct, std::vector<T>& vector, uint16_t byte_pos){
		auto file_stream = open_file(segy_struct.filename, 'i');
		uint16_t bytes_read = (typeid(vector[0]) == typeid(unsigned int)) ? 4 : 2;
		uint16_t scalar = 240 - bytes_read + segy_struct.number_samples * segy_struct.data_bytes;
		file_stream.seekg(3600 + byte_pos, std::ios::beg);
		for (int i = 0; i < vector.size(); i++) {
			vector[i] = f(file_stream);
			file_stream.seekg(scalar, std::ios::cur);
		}
		close_file(file_stream);
	}
};

auto get_coord(SegyFile& segy_struct) {
	traceheader_iterator <uint32_t, &bytes::read_ui32> iterator;
	std::vector<std::vector<uint32_t>> coordinates(2, std::vector<uint32_t>(segy_struct.number_traces));
	std::thread t(iterator, std::ref(segy_struct), std::ref(coordinates[0]), 72);
	iterator(segy_struct, coordinates[1], 76);
	t.join();
	return coordinates;
}

export namespace segy {

<<<<<<< HEAD
=======
	void print_coord(SegyFile& segy_struct) {
		auto coordinates = get_coord(segy_struct);
		std::cout << std::format("{:-^30}\n", "");
		std::cout << std::format("{:^15} {:^15}\n", "X Coordinate", "Y Coordinate");
		for (int i = 0; i < segy_struct.number_traces; i++)
			std::cout << std::format("{:^15} {:^15}\n", coordinates[0][i], coordinates[1][i]);
	}

>>>>>>> 08af693 (Function Added: XY Coordinates reader and print)
}