
#include "../header/Exception.hpp"

using namespace Fourier;

Exception::Exception(int line, const char* file, std::string message) noexcept
	: _line(line), _file(file), _errorMessage(message) {}

const char* Exception::what() const noexcept
{
	std::ostringstream oss;
	// Fill stringstream with Exception Information
	oss << "[File]: " << _file << std::endl
		<< "[Line]: " << _line << std::endl
		<< std::endl << _errorMessage;

	// Store in buffer so the c_str ptr outlives this scope
	_whatBuffer = oss.str();
	return _whatBuffer.c_str();
}

int Exception::GetLine() const noexcept { return _line; }

std::string Exception::GetFile() const noexcept { return _file; }
