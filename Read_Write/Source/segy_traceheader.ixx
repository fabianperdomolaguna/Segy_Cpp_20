#include <compare>
#include <iostream>
#include <format>
#include <fstream>
#include <unordered_map>
#include <future>

import utilities;
import bytes;

export module segy:traceheader;
import :construct;

template <typename T, T f(std::fstream&)>
std::vector<T> traceheader_iter(SegyFile& segy_struct, uint16_t byte_pos){
	uint16_t bytes_read = (typeid(T) == typeid(unsigned int)) ? 4 : 2;
	uint16_t scalar = 240 - bytes_read + segy_struct.number_samples * segy_struct.data_bytes;
	std::vector<T> data;
	auto file_stream = open_file(segy_struct.filename, 'i');
	file_stream.seekg(3600 + byte_pos, std::ios::beg);
	for (int i = 0; i < segy_struct.number_traces; i++) {
		data.push_back(f(file_stream));
		file_stream.seekg(scalar, std::ios::cur);
	}
	close_file(file_stream);
	return data;
}

auto get_coord(SegyFile& segy_struct) {
	std::vector<std::vector<uint32_t>> coordinates(2, std::vector<uint32_t>(segy_struct.number_traces));
	auto f = std::async(traceheader_iter<uint32_t, &bytes::read_ui32>, std::ref(segy_struct), 72);
	coordinates[1] = std::move(traceheader_iter<uint32_t, &bytes::read_ui32>(segy_struct, 76));
	coordinates[0] = std::move(f.get());
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