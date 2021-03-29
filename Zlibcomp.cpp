
#include <iostream>
#include <fstream>
#include <zlib.h>
#include <vector>

// Decompress file and save it to desired location
void decompressFile(std::string filePath,std::string toPath) {
	std::ifstream file;
	file.open(filePath, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Unable to open";
		return;
	}
	file.seekg(0, file.end);
	size_t len = file.tellg();
	std::cout << "Compressed File size: " << len << std::endl;
	file.seekg(0, file.beg);
	uint32_t decompsize;
	file.read((char*)&decompsize, sizeof(uint32_t)); //read size required for decompression

	std::vector<char> buffer;
	buffer.resize(len - sizeof(uint32_t));
	file.read(&buffer[0], buffer.size());

	std::vector<char> bufferDecomp;
	bufferDecomp.resize(decompsize);

	int result = uncompress((Bytef*)bufferDecomp.data(), (uLongf*)&decompsize, (Bytef*)buffer.data(), (uLongf)buffer.size());
	if (result != Z_OK) {
		std::cout << "Uncompress error" << std::endl;
		return;
	}

	std::ofstream deCompressedFile(toPath, std::ios::binary);
	if (!deCompressedFile.is_open()) {
		std::cout << "Unable to open decompressed file" << std::endl;
	}
	deCompressedFile.write((char*)bufferDecomp.data(), bufferDecomp.size());
	deCompressedFile.close();
	file.close();
}

// Compress file and save it to given location
void compressFile(std::string filePath,std::string toPath) {
	// Read the file
	std::ifstream file;
	file.open(filePath, std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Unable to open";
		return;
	}
	if (file) {
		file.seekg(0, file.end);
		size_t len = file.tellg(); // get file size in bits
		std::cout << "Initial file size: " <<len << std::endl;
		file.seekg(0, file.beg); 

		// write file to the buffer
		std::vector<char> buffer;
		buffer.resize(len);
		file.read(&buffer[0], len);

		file.close();
			
		uLong sizeDataCompressed = compressBound(len); // get bound required for compression 
		std::vector<Byte> dataCompressed;
		dataCompressed.resize(sizeDataCompressed);
		
		int res = compress(dataCompressed.data(), &sizeDataCompressed, (Bytef*)buffer.data(), (uLong)len); 

		if (res != Z_OK) {
			std::cout << "Compression Error" << std::endl;
			return;
		}
		std::ofstream compressedFile(toPath, std::ios::binary); // create compressed binary file
		if (!compressedFile.is_open()) {
			std::cout << "Unable to open" << std::endl;
		}
		uint32_t compFileSize = len;
		// write data to compressed file
		compressedFile.write((char*)&compFileSize, sizeof(uint32_t)); // write size required to uncompress
		compressedFile.write((char*)dataCompressed.data(), sizeDataCompressed);
		compressedFile.close();
		file.close();
	}
}
int main()
{

	/*Simple compression & decompression using zlib */
	compressFile("E:\\example.bmp","E:\\compressed.bin");
	decompressFile("E:\\compressed.bin", "E:\\deCompressed.bmp");
	system("Pause");
}

