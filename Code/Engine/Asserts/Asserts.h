#pragma once
#include "Configuration.h"

#ifdef CS6610_ASSERTS_AREENABLED
#include <intrin.h>
#include <sstream>
#endif

#ifdef CS6610_ASSERTS_AREENABLED
namespace cs6610
{
	namespace Asserts
	{
		bool ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak(const unsigned int i_lineNumber, const char* const i_file,
			bool& io_shouldThisAssertBeIgnoredInTheFuture, const char* const i_message, ...);
		bool ShowMessageAndReturnWhetherToBreak(std::ostringstream& io_message, bool& io_shouldThisAssertBeIgnoredInTheFuture);
	}
}
#endif

#ifdef CS6610_ASSERTS_AREENABLED
#if defined( _WIN32  )
#define CS6610_ASSERTS_BREAK __debugbreak()
#else
#error "No implementation exists for breaking on asserts"
#endif

#define CS6610_ASSERT( i_assertion )	\
	{	\
		static bool shouldThisAssertBeIgnored = false;	\
		if ( !shouldThisAssertBeIgnored && !static_cast<bool>( i_assertion )	\
			&& cs6610::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( __LINE__, __FILE__, shouldThisAssertBeIgnored, "" ) )	\
		{	\
			CS6610_ASSERTS_BREAK;	\
		}	\
	}
#define CS6610_ASSERTF( i_assertion, i_messageToDisplayWhenAssertionIsFalse, ... )	\
	{	\
		static bool shouldThisAssertBeIgnored = false;	\
		if ( !shouldThisAssertBeIgnored && !static_cast<bool>( i_assertion ) \
			&& cs6610::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( __LINE__, __FILE__,	\
				shouldThisAssertBeIgnored, i_messageToDisplayWhenAssertionIsFalse, __VA_ARGS__ ) )	\
		{	\
			CS6610_ASSERTS_BREAK;	\
		}	\
	}
#else
// The macros do nothing when asserts aren't enabled
#define CS6610_ASSERT( i_assertion )
#define CS6610_ASSERTF( i_assertion, i_messageToDisplayWhenAssertionIsFalse, ... )
#endif