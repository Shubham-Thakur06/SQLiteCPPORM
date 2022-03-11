#pragma once

#include "SQL.h"

//Base Model class for objects to be stored in DB
class Model
{
	friend class Database;

protected:

	std::vector<std::pair< std::pair<std::string, std::string >, std::string>> TableVector;

	void RegisterMember(const std::pair< std::pair<std::string, std::string>, std::string>& Pair);

	virtual void ExportMembers() = 0;

	virtual void ImportMembers(char** Vals) = 0;
public:

};