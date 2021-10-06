#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <string>

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
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	std::cout << time_span * 1000 << "ms" << std::endl;
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

	template <typename T>
	std::vector<std::pair<T, T>> read_twocol_csv(std::string message) {
		std::vector<std::pair<T, T>> pairs;
		auto file_stream = open_file(get_user_input(message), 'i');
		T X, Y; char comma; std::string line;
		std::getline(file_stream, line);
		while (!file_stream.eof()) {
			std::getline(file_stream, line);
			std::stringstream line_stream(line);
			line_stream >> X >> comma >> Y;
			pairs.push_back(std::make_pair(X, Y));
		}
		close_file(file_stream);
		return pairs;
	}

}