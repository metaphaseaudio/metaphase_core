//
// created by mzapp 4/5/2020
//
#pragma once


// Define what compilers we're using
#if defined __clang__
	#define COMPILER_IS_CLANG 1

#elif defined __GNUC__
	#define COMPILER_IS_GCC 1

#elif defined __MINGW32__
	#define COMPILER_IS_MINGW 1

#elif defined _MSC_VER)
	#define COMPILER_IS_MSVC 1

#else
#error unrecognized compiler
#endif

// MSVC has some fancy additional markers for external calls
#if COMPILER_IS_MSVC
	#define STD_CALL   __stdcall
	#define C_DECL      __cdecl
#else
	#define STD_CALL
	#define C_DECL
#endif
