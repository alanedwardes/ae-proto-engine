#pragma once

#include <string>

class Compressor
{
public:
	static std::string Compress(std::string szData);
	static std::string Extract(std::string szCompressed);
};