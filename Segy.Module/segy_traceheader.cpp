#include <iostream>
#include <vector>
#include <format>
#include <fstream>

export module segy.traceheader;

import segy.construct;
import utilities;
import bytes;

template <typename T, T f(std::fstream&)>
std::vector<T> traceheader_iter(SegyFile& segy_struct, uint16_t byte_pos){
	uint16_t bytes_read = sizeof(T);
	uint16_t scalar = 240 - bytes_read + segy_struct.number_samples * segy_struct.number_data_bytes;
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
	std::vector<std::pair<uint32_t, uint32_t>> coordinates;
	uint16_t scalar = 240 - 8 + segy_struct.number_samples * segy_struct.number_data_bytes;
	auto file_stream = open_file(segy_struct.filename, 'i');
	file_stream.seekg(3600 + 72, std::ios::beg);
	uint32_t X, Y;
	for (int i = 0; i < segy_struct.number_traces; i++) {
		X = bytes::read_ui32(file_stream); Y = bytes::read_ui32(file_stream);
		coordinates.push_back(std::make_pair(X,Y));
		file_stream.seekg(scalar, std::ios::cur);
	}
	close_file(file_stream);
	return coordinates;
}

void write_coord(std::string& filename, std::vector<std::pair<uint32_t, uint32_t>>& coord, 
	uint16_t scalar, uint16_t byte_pos) {
	auto segy_file = open_file(filename, 'o');
	segy_file.seekg(3600 + byte_pos, std::ios::beg);
	for (auto& pair : coord) {
		bytes::write_ui32(segy_file, pair.first);
		bytes::write_ui32(segy_file, pair.second);
		segy_file.seekg(scalar, std::ios::cur);
	}
	close_file(segy_file);
}

export namespace segy {

	void print_coord(SegyFile& segy_struct) {
		auto coordinates = get_coord(segy_struct);
		std::cout << std::format("{:-^30}\n", "");
		std::cout << std::format("{:^15} {:^15}\n", "X Coordinate", "Y Coordinate");
		for (auto& pair : coordinates)
			std::cout << std::format("{:^15} {:^15}\n", pair.first, pair.second);
	}

	void save_coord(SegyFile& segy_struct) {
		auto coordinates = get_coord(segy_struct);
		auto file_stream = open_file(get_user_input("Enter filename: "), 'w');
		file_stream << "X" << "," << "Y" << std::endl;
		for (auto& pair : coordinates)
			file_stream << pair.first << "," << pair.second << std::endl;
		close_file(file_stream);
	}

	void replace_coord(SegyFile& segy_struct) {
		auto coordinates = read_twocol_csv<uint32_t>("Enter coordinates filename: ");
		uint16_t scalar = 240 - 8 + segy_struct.number_samples * segy_struct.number_data_bytes;
		write_coord(segy_struct.filename, coordinates, scalar, 72);
		write_coord(segy_struct.filename, coordinates, scalar, 80);
	}

}