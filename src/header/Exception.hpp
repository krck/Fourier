#pragma once

#include "Settings.hpp"
#include <exception>
#include <sstream>
#include <string>

class Exception : public std::exception
{
private:
	const std::string _errorMessage;
	const int _line;
	const std::string _file;
protected:
	mutable std::string _whatBuffer;

public:
	Exception(int line, const char* file, std::string message) noexcept;
	const char* what() const noexcept override;
	int GetLine() const noexcept;
	std::string GetFile() const noexcept;
};

// Exception macros adding LINE and FILE automatically
#define sException(message) Exception(__LINE__, __FILE__, message)
