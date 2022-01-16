#include <string>
#include <iostream>
#include <format>
#include <fstream>
#include <unordered_map>

export module segy.construct;

import utilities;
import bytes.read;

std::unordered_map<int16_t, uint16_t> format_number_bytes{ {1,4},{2,4},{3,2},{4,4},{5,4},{6,8},
		{7,3},{8,1},{9,8},{10,4},{11,2},{12,8},{15,3},{16,1} };

std::unordered_map<int16_t, std::string> data_format_name{ {1,"4-byte IBM floating point"},
			{2,"4-byte, two's complement integer"}, {3,"2-byte, two's complement integer"},
			{4,"4-byte fixed-point with gain"}, {5,"4-byte IEEE floating-point"}, {6,"8-byte IEEE floating-point"},
			{7,"3-byte two’s complement integer"}, {8,"1-byte, two's complement integer"},
			{9,"8-byte, two's complement integer"}, {10,"4-byte, unsigned integer"},
			{11,"2-byte, unsigned integer"}, {12,"8-byte, unsigned integer"}, {15,"3-byte, unsigned integer"},
			{16,"1-byte, unsigned integer"} };

std::unordered_map<int32_t, std::string> sort_code{ {0,"Unknown"}, {1,"As recorded"}, {2,"CDP emsemble"},
	{3,"Single fold continuous profile"}, {4,"Horizontally stacked"}, {5,"Common source point"},
	{6,"Common receiver point"}, {7,"Common offset point"}, {8,"Common mid-point"}, {9,"Common conversion point"} };

std::unordered_map<int32_t, std::string> measure_system{ {1,"Meters"}, {2,"Feets"} };

std::unordered_map<int32_t, std::string> polarity{ {0,"Unknown"}, {1,"European"}, {2,"American"} };

export struct SegyFile {
	std::string filename;
	std::streamsize filesize;
	uint16_t number_samples;
	int16_t data_format_code;
	uint16_t number_data_bytes;
	uint64_t number_traces;

	SegyFile() {
		filename = get_user_input("Enter filename: ");
		std::fstream file_stream = open_file(filename, 'i');
		file_stream.seekg(0, std::ios::end); filesize = file_stream.tellg();
		file_stream.seekg(3220, std::ios::beg); number_samples = bytes::read_ui16(file_stream);
		file_stream.seekg(3224, std::ios::beg); data_format_code = bytes::read_i16(file_stream);
		number_data_bytes = format_number_bytes[data_format_code];
		number_traces = (filesize - 3600) / (240 + static_cast<uint64_t>(number_samples)
			* static_cast<uint64_t>(number_data_bytes));
		close_file(file_stream);
	}
};

//I=i32 - U=ui32 - S=i16 - V=ui16 - 8=ui8
std::vector<int32_t> get_main_params(std::fstream& file_stream) {
	std::vector<std::pair<uint16_t, char>> bytes_position = { {3204,'I'},{3212,'S'},{3216,'V'},
		{3228,'S'},{3254,'S'},{3256,'S'},{3500,'8'},{3501,'8'} };
	std::vector<int32_t> main_params;
	for (std::pair<uint16_t, char> pair : bytes_position) {
		file_stream.seekg(pair.first, std::ios::beg);
		switch (pair.second)
		{
		case 'I': main_params.push_back(bytes::read_i32(file_stream)); break;
		case 'S': main_params.push_back(bytes::read_i16(file_stream)); break;
		case 'V': main_params.push_back(bytes::read_ui16(file_stream)); break;
		case '8': main_params.push_back(bytes::read_ui8(file_stream)); break;
		}
	}
	return main_params;
}

export namespace segy {

	void print_main_info(SegyFile& segy_struct) {

		std::fstream file_stream = open_file(segy_struct.filename, 'i');
		std::vector<int32_t> params = get_main_params(file_stream);

		std::cout << std::format("{:-^68}\n", "SEG-Y Info");
		std::cout << std::format("{:>34} {}\n", "Filename:", segy_struct.filename);
		std::cout << std::format("{:>34} {:.2f} MB\n", "Filesize:", static_cast<double>(segy_struct.filesize) / 1000000.);
		std::cout << std::format("{:>34} {}\n", "Line number:", params[0]);
		std::cout << std::format("{:>34} {}\n", "Number of data traces per record:", params[1]);
		std::cout << std::format("{:>34} {} (us)\n", "Sample interval:", params[2]);
		std::cout << std::format("{:>34} {}\n", "Number of samples per data trace:", segy_struct.number_samples);
		std::cout << std::format("{:>34} {}\n", "Data sample format:", data_format_name[segy_struct.data_format_code]);
		std::cout << std::format("{:>34} {} ({})\n", "Trace sorting code:", params[3], sort_code[params[3]]);
		std::cout << std::format("{:>34} {} ({})\n", "Measure system:", params[4], measure_system[params[4]]);
		std::cout << std::format("{:>34} {} ({})\n", "Signal polarity:", params[5], polarity[params[5]]);
		std::cout << std::format("{:>34} {}.{}\n", "SEG-Y Rev:", params[6], params[7]);
		std::cout << std::format("{:>34} {}\n", "Number of traces:", segy_struct.number_traces);
		std::cout << std::format("{:>34} {} (s)\n", "Time length:", static_cast<float>(params[2] *
			static_cast<int32_t>(segy_struct.number_samples - 1)) / 1000000.f);

		close_file(file_stream);
	}

}