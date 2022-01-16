#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

export module segy.tracedata;

import segy.construct;
import bytes.read;

template <typename T>
auto read_traces(SegyFile& segy_struct) {
	auto file_stream = open_file(segy_struct.filename, 'i');
	file_stream.seekg(3600, std::ios::beg);
	std::vector<std::vector<T>> traces_data;
	for (int i = 0; i < segy_struct.number_traces; i++) {
		file_stream.seekg(240, std::ios::cur);
		std::vector<T> trace(segy_struct.number_samples);
		std::generate(trace.begin(), trace.end(), [&file_stream]() {return bytes::read_ibm(file_stream); });
		traces_data.push_back(std::move(trace));
	}
	close_file(file_stream);
	return traces_data;
}

export namespace segy {

	void print_trace(SegyFile& segy_struct) {
		auto traces = read_traces<float>(segy_struct);
		std::string message = "Seismic line has " + std::to_string(segy_struct.number_traces) 
			+ " traces, please enter the trace number to print: ";
		auto number_trace = get_user_option<uint16_t>(message) - 1;
		for (auto x : traces[number_trace])
			std::cout << x << std::endl;
	}
	
}