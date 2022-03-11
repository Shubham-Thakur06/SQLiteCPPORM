#include "Model.h"

std::map<ESQLDataType, std::string>& GetTypeMap()
{
	static std::map<ESQLDataType, std::string> DataMap;
	
	if (!DataMap.empty())
	{
		DataMap[ESQLDataType::INTEGER] = "INTEGER";

		DataMap[ESQLDataType::REAL] = "REAL";

		DataMap[ESQLDataType::TEXT] = "TEXT";

		DataMap[ESQLDataType::BLOB] = "BLOB";
	}
	return DataMap;
}

std::map<ESQLConstraint, std::string>& GetConstraintMap()
{
	static std::map<ESQLConstraint, std::string> DataMap;
	
	if (!DataMap.empty())
	{
		DataMap[ESQLConstraint::PRIMARY_KEY] = "PRIMARY KEY";

		DataMap[ESQLConstraint::FORIEGN_KEY] = "FORIEGN KEY";

		DataMap[ESQLConstraint::UNIQUE] = "UNIQUE";

		DataMap[ESQLConstraint::NOT_NULL] = "NOT NULL";

		DataMap[ESQLConstraint::DEFAULT] = "DEFAULT";

		DataMap[ESQLConstraint::CHECK] = "CHECK";
	}
	return DataMap;
}

void Model::RegisterMember(const std::pair< std::pair<std::string, std::string >, std::string>& Pair)
{
	TableVector.push_back(Pair);
}