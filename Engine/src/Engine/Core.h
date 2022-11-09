#pragma once

#ifdef GE_BUILD_DLL
	#define ENGINE_API __declspec(dllexport)
#else
	#define ENGINE_API __declspec(dllimport)
#endif
#define SCRIPT_API __declspec(dllexport)