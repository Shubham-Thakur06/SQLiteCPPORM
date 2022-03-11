#include "Database.h"

Database::Database(const std::string& FileName)
{
	open(FileName);
}

Database::~Database()
{
	close();
}

void Database::open(const std::string& FileName)
{
	m_DB = nullptr;

	try 
	{
		//TODO UTF-8 string
		int rc = sqlite3_open(FileName.c_str(), &m_DB);
		if (rc != SQLITE_OK)
			throw "[Error] Database error: " + std::string(sqlite3_errmsg(m_DB));
	}
	catch (std::runtime_error e) 
	{
		if (m_DB)
			sqlite3_close(m_DB);
		
		throw e;
	}
}

void Database::close()
{
	std::cout << "Closing File!"<<std::endl;
}

int Database::int_CreateTable(const std::string& Table, Model* Obj)
{
	std::string Query = "CREATE TABLE IF NOT EXISTS ";
	Query += Table;
	Query += " (";

	Obj->ExportMembers();

	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
	{
		Query += Obj->TableVector[i].first.first + " ";
		Query += Obj->TableVector[i].first.second + ", ";
	}

	Obj->TableVector.clear();

	Query.pop_back();
	Query.pop_back();
	Query += ");";
	char* Err;
	
	return sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), nullptr, 0, &Err);
}

int Database::int_DeleteTable(const std::string& Table)
{
	std::string Query = "DROP TABLE " + Table + ";";
	
	char* Err;

	return sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), nullptr, 0, &Err);
}

int Database::int_ClearTable(const std::string& Table)
{
	std::string Query = "DELETE FROM " + Table + ";";

	char* Err;

	return sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), nullptr, 0, &Err);
}

int Database::int_Write(const std::string& Table, Model* Obj)
{
	std::string Query = "INSERT INTO " + Table + " (";

	Obj->ExportMembers();

	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
		Query += Obj->TableVector[i].first.first + ", ";

	Query.pop_back();
	Query.pop_back();

	Query += ") VALUES (";

	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
		Query += Obj->TableVector[i].second + ", ";

	Obj->TableVector.clear();

	Query.pop_back();
	Query.pop_back();
	Query += ");";

	char* Err;

	return sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), nullptr, 0, &Err);
}

int Database::int_Update(const std::string& Table, Model* Obj, const std::string& SearchCondition)
{
	std::string Query = "UPDATE " + Table + " SET ";

	Obj->ExportMembers();

	for (unsigned int i = 0; i < Obj->TableVector.size(); i++)
	{
		Query += Obj->TableVector[i].first.first + " = ";
		Query += Obj->TableVector[i].second + ", ";
	}

	Obj->TableVector.clear();

	Query.pop_back();
	Query.pop_back();
	Query += " WHERE ";
	Query += SearchCondition;

	char* Err;

	return sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), nullptr, 0, &Err);
}

int Database::CountCallback(void* Obj, int argc, char** argv, char** azColName)
{
	int* Count = static_cast<int*>(Obj);

	*Count = std::stoi(argv[0]);
	
	return 0;
}


int Database::getcount(const std::string& Table, const std::string& SearchCondition)
{
	std::string Query = "SELECT COUNT(*) FROM " + Table + " WHERE " + SearchCondition + ";";

	int* count = new int();

	char* Err;

	int result = sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), CountCallback, count, &Err);

	return *count;
}

int Database::Callback(void* Obj, int argc, char** argv, char** azColName)
{
	Model* Object = static_cast<Model*>(Obj);
	
	Object->ImportMembers(argv);
	
	return 0;
}

int Database::int_Read(const std::string& Table, Model* Obj, const std::string& SearchCondition)
{
	std::string Query = "SELECT * FROM " + Table + " WHERE " + SearchCondition;

	char* Err;

	int result = sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), Callback, Obj, &Err);
	
	return result;
}

int Database::int_ReadAll(const std::string& Table, std::vector<Model*> Vector, const std::string& SearchCondition)
{
	std::string Query = "SELECT * FROM " + Table + " WHERE " + SearchCondition;

	sqlite3_stmt* Statement;

	int result = sqlite3_prepare(m_DB, const_cast<char*>(Query.c_str()), -1, &Statement, nullptr);

	int Step = 0, i = 0;

	for (Step = sqlite3_step(Statement); Step == SQLITE_ROW && Step != SQLITE_DONE; Step = sqlite3_step(Statement), i++)
	{
		std::vector<char*> Values;// = { };

		for (int Column = 0; Column < sqlite3_column_count(Statement); Column++)
		{
			Values.push_back((char*)sqlite3_column_text(Statement, Column));
		}

		Vector[i]->ImportMembers(Values.data());
	}

	return result;
}

int Database::int_Delete(const std::string& Table, const std::string& SearchCondition)
{
	std::string Query = "DELETE FROM " + Table + " WHERE " + SearchCondition + " ;";

	char* Err;

	int result = sqlite3_exec(m_DB, const_cast<char*>(Query.c_str()), nullptr, 0, &Err);
	
	return result;
}
