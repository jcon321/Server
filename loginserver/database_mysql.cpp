/*	EQEMu: Everquest Server Emulator
	Copyright (C) 2001-2010 EQEMu Development Team (http://eqemulator.net)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY except by those people which sell it, which
	are required to give you total support for your newly bought product;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "../common/global_define.h"
#include "database.h"

#ifdef EQEMU_MYSQL_ENABLED
#include "database_mysql.h"
#include "error_log.h"
#include "login_server.h"

extern ErrorLog *server_log;
extern LoginServer server;

DatabaseMySQL::DatabaseMySQL(string user, string pass, string host, string port, string name)
{
	this->user = user;
	this->pass = pass;
	this->host = host;
	this->name = name;

	database = mysql_init(nullptr);
	if(database)
	{
		my_bool r = 1;
		mysql_options(database, MYSQL_OPT_RECONNECT, &r);
		if(!mysql_real_connect(database, host.c_str(), user.c_str(), pass.c_str(), name.c_str(), atoi(port.c_str()), nullptr, 0))
		{
			mysql_close(database);
			server_log->Log(log_database, "Failed to connect to MySQL database. Error: %s", mysql_error(database));
			exit(1);
		}
	}
	else
	{
		server_log->Log(log_database, "Failed to create db object in MySQL database.");
	}
}

DatabaseMySQL::~DatabaseMySQL()
{
	if(database)
	{
		mysql_close(database);
	}
}

bool DatabaseMySQL::GetLoginDataFromAccountName(string name, string &password, unsigned int &id)
{
	if (!database)
	{
		return false;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;
	stringstream query(stringstream::in | stringstream::out);
	query << "SELECT LoginServerID, AccountPassword FROM " << server.options.GetAccountTable() << " WHERE AccountName = '";
	query << name;
	query << "'";

	if (mysql_query(database, query.str().c_str()) != 0)
	{
		server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
		return false;
	}

	res = mysql_use_result(database);

	if (res)
	{
		while ((row = mysql_fetch_row(res)) != nullptr)
		{
			id = atoi(row[0]);
			password = row[1];
			mysql_free_result(res);
			return true;
		}
	}

	server_log->Log(log_database, "Mysql query returned no result: %s", query.str().c_str());
	return false;
}

bool DatabaseMySQL::GetWorldRegistration(string long_name, string short_name, unsigned int &id, string &desc, unsigned int &list_id,
	unsigned int &trusted, string &list_desc, string &account, string &password)
{
	if (!database)
	{
		return false;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;
	char escaped_short_name[101];
	unsigned long length;
	length = mysql_real_escape_string(database, escaped_short_name, short_name.substr(0, 100).c_str(), short_name.substr(0, 100).length());
	escaped_short_name[length + 1] = 0;
	stringstream query(stringstream::in | stringstream::out);
	query << "SELECT ifnull(WSR.ServerID,999999) AS ServerID, WSR.ServerTagDescription, ifnull(WSR.ServerTrusted,0) AS ServerTrusted, ifnull(SLT.ServerListTypeID,3) AS ServerListTypeID, ";
	query << "SLT.ServerListTypeDescription, ifnull(WSR.ServerAdminID,0) AS ServerAdminID FROM " << server.options.GetWorldRegistrationTable();
	query << " AS WSR JOIN " << server.options.GetWorldServerTypeTable() << " AS SLT ON WSR.ServerListTypeID = SLT.ServerListTypeID";
	query << " WHERE WSR.ServerShortName = '";
	query << escaped_short_name;
	query << "'";

	if (mysql_query(database, query.str().c_str()) != 0)
	{
		server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
		return false;
	}

	res = mysql_use_result(database);
	if (res)
	{
		if ((row = mysql_fetch_row(res)) != nullptr)
		{
			id = atoi(row[0]);
			desc = row[1];
			trusted = atoi(row[2]);
			list_id = atoi(row[3]);
			list_desc = row[4];
			int db_account_id = atoi(row[5]);
			mysql_free_result(res);

			if (db_account_id > 0)
			{
				stringstream query(stringstream::in | stringstream::out);
				query << "SELECT AccountName, AccountPassword FROM " << server.options.GetWorldAdminRegistrationTable();
				query << " WHERE ServerAdminID = " << db_account_id;

				if (mysql_query(database, query.str().c_str()) != 0)
				{
					server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
					return false;
				}

				res = mysql_use_result(database);
				if (res)
				{
					if ((row = mysql_fetch_row(res)) != nullptr)
					{
						account = row[0];
						password = row[1];
						mysql_free_result(res);
						return true;
					}
				}

				server_log->Log(log_database, "Mysql query returned no result: %s", query.str().c_str());
				return false;
			}
			return true;
		}
	}

	server_log->Log(log_database, "Mysql query returned no result: %s", query.str().c_str());
	return false;
}

void DatabaseMySQL::UpdateLSAccountData(unsigned int id, string ip_address)
{
	if (!database)
	{
		return;
	}

	stringstream query(stringstream::in | stringstream::out);
	query << "UPDATE " << server.options.GetAccountTable() << " SET LastIPAddress = '";
	query << ip_address;
	query << "', LastLoginDate = now() where LoginServerID = ";
	query << id;

	if (mysql_query(database, query.str().c_str()) != 0)
	{
		server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
	}
}

void DatabaseMySQL::UpdateLSAccountInfo(unsigned int id, string name, string password, string email)
{
	if (!database)
	{
		return;
	}

	stringstream query(stringstream::in | stringstream::out);
	query << "REPLACE " << server.options.GetAccountTable() << " SET LoginServerID = ";
	query << id << ", AccountName = '" << name << "', AccountPassword = sha('";
	query << password << "'), AccountCreateDate = now(), AccountEmail = '" << email;
	query << "', LastIPAddress = '0.0.0.0', LastLoginDate = now()";

	if (mysql_query(database, query.str().c_str()) != 0)
	{
		server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
	}
}

void DatabaseMySQL::UpdateWorldRegistration(unsigned int id, string long_name, string ip_address)
{
	if (!database)
	{
		return;
	}

	char escaped_long_name[101];
	unsigned long length;
	length = mysql_real_escape_string(database, escaped_long_name, long_name.substr(0, 100).c_str(), long_name.substr(0, 100).length());
	escaped_long_name[length + 1] = 0;
	stringstream query(stringstream::in | stringstream::out);
	query << "UPDATE " << server.options.GetWorldRegistrationTable() << " SET ServerLastLoginDate = now(), ServerLastIPAddr = '";
	query << ip_address;
	query << "', ServerLongName = '";
	query << escaped_long_name;
	query << "' WHERE ServerID = ";
	query << id;

	if (mysql_query(database, query.str().c_str()) != 0)
	{
		server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
	}
}

bool DatabaseMySQL::CreateWorldRegistration(string long_name, string short_name, unsigned int &id)
{
	if (!database)
	{
		return false;
	}

	MYSQL_RES *res;
	MYSQL_ROW row;
	char escaped_long_name[201];
	char escaped_short_name[101];
	unsigned long length;
	length = mysql_real_escape_string(database, escaped_long_name, long_name.substr(0, 100).c_str(), long_name.substr(0, 100).length());
	escaped_long_name[length + 1] = 0;
	length = mysql_real_escape_string(database, escaped_short_name, short_name.substr(0, 100).c_str(), short_name.substr(0, 100).length());
	escaped_short_name[length + 1] = 0;
	stringstream query(stringstream::in | stringstream::out);
	query << "SELECT ifnull(max(ServerID),0) FROM " << server.options.GetWorldRegistrationTable();

	if (mysql_query(database, query.str().c_str()) != 0)
	{
		server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
		return false;
	}

	res = mysql_use_result(database);
	if (res)
	{
		if ((row = mysql_fetch_row(res)) != nullptr)
		{
			id = atoi(row[0]) + 1;
			mysql_free_result(res);

			stringstream query(stringstream::in | stringstream::out);
			query << "INSERT INTO " << server.options.GetWorldRegistrationTable() << " SET ServerID = " << id;
			query << ", ServerLongName = '" << escaped_long_name << "', ServerShortName = '" << escaped_short_name;
			query << "', ServerListTypeID = 3, ServerAdminID = 0, ServerTrusted = 0, ServerTagDescription = ''";

			if (mysql_query(database, query.str().c_str()) != 0)
			{
				server_log->Log(log_database, "Mysql query failed: %s", query.str().c_str());
				return false;
			}
			return true;
		}
	}
	server_log->Log(log_database, "World registration did not exist in the database for %s %s", long_name.c_str(), short_name.c_str());
	return false;
}

#endif

