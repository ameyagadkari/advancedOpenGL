#include "Asserts.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN

#include <cstdarg>
#include <cstdio>

#ifdef CS6610_ASSERTS_AREENABLED

bool cs6610::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak(const unsigned int i_lineNumber, const char* const i_file,
	bool& io_shouldThisAssertBeIgnoredInTheFuture, const char* const i_message, ...)
{
	// Construct the message
	std::ostringstream message;
	{
		message << "An assertion failed on line " << i_lineNumber << " of " << i_file;
		const size_t bufferSize = 512;
		char buffer[bufferSize];
		int formattingResult;
		{
			va_list insertions;
			va_start(insertions, i_message);
			formattingResult = vsnprintf(buffer, bufferSize, i_message, insertions);
			va_end(insertions);
		}
		if (formattingResult != 0)
		{
			message << ":\n\n";
			if (formattingResult > 0)
			{
				message << buffer;
				if (formattingResult >= bufferSize)
				{
					message << "\n\n"
						"(The internal buffer of size " << bufferSize
						<< " was not big enough to hold the formatted message of length " << (formattingResult + 1) << ")";
				}
			}
			else
			{
				message << "An encoding error occurred! The unformatted message is: \"" << i_message << "\"!";
			}
		}
		else
		{
			message << "!";
		}
	}
	// Display it and break if necessary
	return ShowMessageAndReturnWhetherToBreak(message, io_shouldThisAssertBeIgnoredInTheFuture);
}

bool cs6610::Asserts::ShowMessageAndReturnWhetherToBreak(std::ostringstream& io_message, bool& io_shouldThisAssertBeIgnoredInTheFuture)
{
	io_message << "\n\n"
		"Do you want to break into the debugger?"
		" Choose \"Yes\" to break, \"No\" to continue, or \"Cancel\" to disable this assertion until the program exits.";
	const int result = MessageBoxA(GetActiveWindow(), io_message.str().c_str(), "Assertion Failed!", MB_YESNOCANCEL);
	if ((result == IDYES)
		// MessageBox() returns 0 on failure; if this happens the code breaks rather than trying to diagnose why
		|| (result == 0))
	{
		return true;
	}
	else
	{
		if (result == IDCANCEL)
		{
			io_shouldThisAssertBeIgnoredInTheFuture = true;
		}
		return false;
	}
}

#endif