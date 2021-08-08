#include <fstream>

export module bytes:read;

void swap4_bytes(unsigned long& bytes) {
	bytes = ((bytes >> 24) | (bytes << 24) |
		((bytes >> 8) & 0xff00) | ((bytes & 0xff00) << 8));
}

export namespace bytes {

	int32_t read_i32(std::fstream& file_stream) {
		char temp[4];
		file_stream.read(temp, 4);
		int32_t value = (temp[0] << 24) + (temp[1] << 16) + (temp[2] << 8) + temp[3];
		return value;
	}

	uint32_t read_ui32(std::fstream& file_stream) {
		char temp[4];
		file_stream.read(temp, 4);
		uint32_t value = ((unsigned char)temp[0] << 24) + ((unsigned char)temp[1] << 16) + 
			((unsigned char)temp[2] << 8) + (unsigned char)temp[3];
		return value;
	}

	int16_t read_i16(std::fstream& file_stream) {
		char temp[2];
		file_stream.read(temp, 2);
		int16_t value = (temp[0] << 8) + temp[1];
		return value;
	}

	uint16_t read_ui16(std::fstream& file_stream) {
		char temp[2];
		file_stream.read(temp, 2);
		uint16_t value = ((unsigned char)temp[0] << 8) + (unsigned char)temp[1];
		return value;
	}

	uint8_t read_ui8(std::fstream& file_stream) {
		char temp[1];
		file_stream.read(temp, 1);
		uint8_t value = (unsigned char)temp[0];
		return value;
	}

	float read_ibm(std::fstream& file_stream) {
		unsigned long bytes;
		file_stream.read((char*)&bytes, 4);
		swap4_bytes(bytes);
		int sign = (bytes >> 31); bytes <<= 1;
		int exponent = (bytes >> 25); bytes <<= 7;
		float mantissa = (bytes >> 8) / (16777216.f);
		return (1 - 2 * sign) * mantissa * std::pow(16.f, static_cast<float>(exponent - 64));
	}

}