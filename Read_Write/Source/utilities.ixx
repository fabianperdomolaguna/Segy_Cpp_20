module;

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

export module utilities;

const int BUF_SIZE = 4096;

void copy_write_single_buffer(std::fstream& ifstream, std::fstream& ofstream, int write_size) {
	char buffer[BUF_SIZE];
	ifstream.read(&buffer[0], write_size);
	ofstream.write(&buffer[0], ifstream.gcount());
}

void copy_write_buffers(std::fstream& ifstream, std::fstream& ofstream, int quotient, int remainder) {
	char buffer[BUF_SIZE];
	for (int i = 0; i < quotient; i++) {
		ifstream.read(&buffer[0], BUF_SIZE);
		ofstream.write(&buffer[0], ifstream.gcount());
	}
	ifstream.read(&buffer[0], remainder);
	ofstream.write(&buffer[0], ifstream.gcount());
}

void chrono_execution() {
	auto start = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	uint64_t microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	std::cout << microseconds;
}

export{

	void pause() {
		std::cout << std::endl << "Press <Enter> to continue...";
		#ifdef _WIN32
			std::cin.get();  std::cin.get();
		#else
			std::cin.get();
		#endif	
	}

	void clear_screen(){
	#ifdef _WIN32
		std::system("cls");
	#else
		std::system("clear");
	#endif
	}

	std::string get_user_input(std::string message) {
		std::string input;
		std::cout << message;
		std::cin >> input;
		return input;
	}

	template <typename T>
	T get_user_option(std::string message) {
		T selection;
		std::cout << message;
		std::cin >> selection;
		return selection;
	}

	//i=read_file - o=write_new_file - t=overwrite_file
	std::fstream open_file(std::string filename, char option) {
		std::fstream file_stream;
		switch (option)
		{
		case 'i': file_stream.open(filename.c_str(), std::fstream::in | std::ios::binary); break;
		case 'o': file_stream.open(filename.c_str(), std::fstream::out | std::ios::binary); break;
		case 't': file_stream.open(filename.c_str(), std::ios::binary | std::fstream::out | std::ios::in); break;
		}

		if (file_stream.fail()) {
			std::cout << "File could not be opened" << std::endl;
			exit(1);
		}
		return file_stream;
	}

	void close_file(std::fstream& file_stream) {
		file_stream.close();
	}

}