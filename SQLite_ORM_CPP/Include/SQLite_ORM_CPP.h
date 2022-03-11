#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "sqlite3.h"

namespace SQLite_ORM_CPP
{
    //Function returns Name of Class
    template<class C>
    std::string getClassName();
}


//Argument List: Member to be Mapped, Type of Data
#define REGISTER_MEMBER(Column, Type) RegisterMember(MakePair(#Column, Column, Type))

//Argument List: Member to be Mapped, Type of Data, Column Constraint
#define REGISTER_MEMBER_WITH_CONSTRAINT(Column, Type, Constraint) RegisterMember(MakePair(#Column, Column, Type, Constraint))

#define EXPORT_CLASS(ClName)					\
template<>										\
std::string SQLite_ORM_CPP::getClassName<ClName>()	    \
{	std::string temp(#ClName);std::replace(temp.begin(), temp.end(), ':', '_');return temp;}