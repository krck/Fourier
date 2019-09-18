
#ifndef FOURIER_EXCEPTION_H
#define FOURIER_EXCEPTION_H

#include "Settings.hpp"
#include <exception>
#include <sstream>
#include <string>

namespace Fourier {

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
#define FourierException(message) Exception(__LINE__, __FILE__, message)

}

#endif // FOURIER_EXCEPTION_H
