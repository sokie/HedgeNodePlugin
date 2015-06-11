/*
This project is released under the GPL 2.0 license.
Please do no evil.

Initial author: (https://github.com/)Sokie
Started: 09-06-2015
Notes:
Manages persistence.
*/

#include "..\StdInclude.h"

namespace Database
{
	sqlite3* DatabaseManager::database;

	bool DatabaseManager::InitializeDatabase(){
		char *zErrMsg = 0;
		int rc;

		bool isNewDb = true;

		if (std::ifstream(Global::Database_name))
		{
			isNewDb = false;
		}

		rc = sqlite3_open(Global::Database_name, &database);

		if (rc){
			DBGPrint("Can't open database: %s\n", sqlite3_errmsg(database));
			return false;
		}
		else{
			DBGPrint("Opened database successfully\n");
		}

		if (isNewDb){
			DatabaseManager::query("CREATE TABLE Users (userId INTEGER PRIMARY KEY, steamId TEXT, lastSeen TEXT NOT NULL DEFAULT(NOW())); ");
			DatabaseManager::query("CREATE TABLE Friend (id INTEGER PRIMARY KEY INTEGER, user INTEGER, friend INTEGER, FOREIGN KEY(user) REFERENCES Users(userId), FOREIGN KEY(friend) REFERENCES Users(userId) );");
		}
		return true;
	}

	bool DatabaseManager::CloseDatabase(){
		sqlite3_close(database);
		return true;
	}

	std::vector<std::vector<std::string> > DatabaseManager::query(char* query)
	{
		sqlite3_stmt *statement;
		std::vector<std::vector<std::string> > results;

		if (sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)
		{
			int cols = sqlite3_column_count(statement);
			int result = 0;
			while (true)
			{
				result = sqlite3_step(statement);

				if (result == SQLITE_ROW)
				{
					std::vector<std::string> values;
					for (int col = 0; col < cols; col++)
					{
						std::string s;
						char *ptr = (char*)sqlite3_column_text(statement, col);
						if (ptr) s = ptr;

						values.push_back(s);
					}
					results.push_back(values);
				}
				else
				{
					break;
				}
			}

			sqlite3_finalize(statement);
		}

		std::string error = sqlite3_errmsg(database);
		if (error != "not an error") DBGPrint("Query executed %s", query);
		
		return results;
	}
}