#pragma once

#include "Model.h"

//Contains SQLite functionalities
class Database
{
public:

	Database(const std::string& FileName);
	
	virtual ~Database();

	template<class C>
	ESQLResult CreateTable();
	
	template<class C>
	ESQLResult DeleteTable();

	template<class C>
	ESQLResult CLearTable();

	template<class C>
	ESQLResult Write(C* Obj);

	template<class C>
	ESQLResult Update(C* Obj, const std::string& SearchCondition);

	template<class C>
	C* Read(const std::string& SearchCondition);

	template<class C>
	std::vector<C*> ReadAll(const std::string& SearchCondition);

	template<class C>
	ESQLResult Delete(const std::string& SearchCondition);

private:

	sqlite3* m_DB = nullptr;

	Database(const Database&);

	const Database operator=(const Database&);


	void open(const std::string& FileName);

	int int_CreateTable(const std::string& Table, Model* Obj);
	
	int int_DeleteTable(const std::string& Table);
	
	int int_ClearTable(const std::string& Table);

	int int_Write(const std::string& Table, Model* Obj);

	int int_Update(const std::string& Table, Model* Obj, const std::string& SearchCondition);
	
	int int_Read(const std::string& Table, Model* Obj, const std::string& SearchCondition);

	int int_ReadAll(const std::string& Table, std::vector<Model*> Vector, const std::string& SearchCondition);

	int int_Delete(const std::string& Table, const std::string& SearchCondition);
	
	void close();

	int getcount(const std::string& Table,const std::string& SearchCondition);

	static int CountCallback(void* Obj, int argc, char** argv, char** azColName);

	static int Callback(void* Obj, int argc, char** argv, char** azColName);
};

template<class C>
ESQLResult Database::CreateTable()
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	Model* Object = dynamic_cast<Model*>(new C());

	int result = int_CreateTable(Table, Object);

	return ESQLResult::OK;
}

template<class C>
ESQLResult Database::DeleteTable()
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int result = int_DeleteTable(Table);

	return ESQLResult::OK;
}


template<class C>
ESQLResult ClearTable()
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int result = int_ClearTable(Table);

	return ESQLResult::OK;
}

template <class C>
ESQLResult Database::Write(C* Obj)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int result = int_Write(Table, Obj);
	
	return ESQLResult::OK;
}

template<class C>
ESQLResult Database::Update(C* Obj, const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int result = int_Update(Table, Obj, SearchCondition);

	return ESQLResult::OK;
}


template<class C>
C* Database::Read(const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	C* Obj = new C();

	Model* ObjPtr = dynamic_cast<Model*>(Obj);

	int result = int_Read(Table, ObjPtr, SearchCondition);

	return Obj;
}

template<class C>
std::vector<C*> Database::ReadAll(const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int ObjectCount = getcount(Table, SearchCondition);

	std::vector<C*> Vector;

	std::vector<Model*> ModelVector;

	for (int i = 0; i < ObjectCount; i++)
	{
		C* Ptr = new C();

		Model* ModelPtr = dynamic_cast<Model*>(Ptr);

		Vector.push_back(Ptr);

		ModelVector.push_back(ModelPtr);
	}

	int result = int_ReadAll(Table, ModelVector, SearchCondition);

	return Vector;
}

template<class C>
inline ESQLResult Database::Delete(const std::string& SearchCondition)
{
	std::string Table = SQLite_ORM_CPP::getClassName<C>();

	int result = int_Delete(Table, SearchCondition);

	return ESQLResult::OK;
}
