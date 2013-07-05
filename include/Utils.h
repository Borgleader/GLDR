#pragma once

#include <exception>
#include <fstream>

template<typename Container>
Container loadFileContent(std::string filename)
{
	struct FileNotFoundException : public std::exception
	{
		FileNotFoundException() {}
	};

	std::ifstream file(filename, std::ios::binary);
	if(!file)
	{
		throw FileNotFoundException();
	}

	return Container(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
