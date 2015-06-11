/*
	This project is licensed under the GNU GPL 2.0 license. Please respect that.

	Initial author: (https://github.com/)Convery
	Started: 2014-10-01
	Notes
*/

#pragma once
#define API __declspec(dllexport)

extern "C"
{
	// Standard plugin exports.
	API uint32_t PreInit(); // Called before the game loads.
	API uint32_t PostInit(); // Called after the game loads.

	API uint32_t AuthorInfo();
	API uint32_t ExtendedInfo(); // Info about the plugin.
	API uint32_t OfficialMod(); // Is it approved?

	// Dependencies that need to be loaded.
	API uint32_t DependencyCount();
	API const char *GetDependency(uint32_t Index);
	API uint64_t* SendMessageB(const char* Message, void* Callback);
}