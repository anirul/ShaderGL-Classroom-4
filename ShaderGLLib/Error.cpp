#include "Error.h"

#include <stdexcept>
#include <GL/glew.h>
#if defined(_WIN32) || defined(_WIN64)
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace sgl {

	void* Error::window_ptr_ = nullptr;

	std::string Error::GetLastError() const
	{
#pragma message ("You have to complete this code!")
		return "";
	}

	void Error::Display(
		const std::string& file /*= ""*/,
		const int line /*= -1*/) const
	{
#pragma message ("You have to complete this code!")
	}

} // End namespace sgl.
