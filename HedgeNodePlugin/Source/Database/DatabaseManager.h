/*
This project is released under the GPL 2.0 license.
Please do no evil.

Initial author: (https://github.com/)Sokie
Started: 09-06-2015
Notes:
Manages persistence.
*/

#pragma once

namespace Database
{
	class DatabaseManager
	{
		//static std::unordered_map<int32_t, uint16_t> UDPSockets;
		//static std::unordered_map<int32_t, uint32_t> TCPSockets;
		//static std::mutex ThreadSafe;
		static sqlite3 *database;

	public:
		// Ctor & dtor since we use static data.
		static bool InitializeDatabase();
		static bool CloseDatabase();
		static std::vector<std::vector<std::string>> query(char* query);

		
	};
}
