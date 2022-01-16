#include <fstream>

export module bytes.write;

export namespace bytes {

	void write_ui32(std::fstream& file_stream, uint32_t value) {
		file_stream.put((value >> 24) & 0xff);
		file_stream.put((value >> 16) & 0xff);
		file_stream.put((value >> 8) & 0xff);
		file_stream.put(value & 0xff);
	}

	void write_ui16(std::fstream& file_stream, uint16_t value) {
		file_stream.put((value >> 8) & 0xff);
		file_stream.put(value & 0xff);
	}

}