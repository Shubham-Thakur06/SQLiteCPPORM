// Sample.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Database.h"

Database db("Sample.db");

class Person : public Model
{
public:

    Person() {   }

    Person(std::string Name, int Age)
        :m_Name(Name),m_Age(Age)
    {   }

protected:

    void ExportMembers() override
    {
        REGISTER_MEMBER(m_Name, ESQLDataType::TEXT);

        REGISTER_MEMBER(m_Age, ESQLDataType::INTEGER);
    }

    void ImportMembers(char** Vals) override
    {
        this->m_Name = Vals[0];

        this->m_Age = std::stoi(Vals[1]);
    }

private:

    std::string m_Name;
    
    int m_Age=0;
};

EXPORT_CLASS(Person)

class Dog : public Model
{
public:
    Dog() {     }

    Dog(const std::string& name, const std::string& owner)
    {
        this->m_Name = name;

        this->m_Owner = owner;
    }

protected:
    
    void ExportMembers() override
    {
        REGISTER_MEMBER(m_Name, ESQLDataType::TEXT);

        REGISTER_MEMBER(m_Owner, ESQLDataType::TEXT);
    }

    void ImportMembers(char** Vals) override
    {
        this->m_Name = Vals[0];

        this->m_Owner = Vals[1];
    }

private:

    std::string m_Name;

    std::string m_Owner;
};

EXPORT_CLASS(Dog)

class Employee : public Model
{
public:

    Employee() {    }
    
    Employee(int id, std::string name)
        : m_ID(id),m_Name(name)
    {   }

protected:
    
    void ExportMembers() override
    {
        REGISTER_MEMBER_WITH_CONSTRAINT(m_ID, ESQLDataType::INTEGER, ESQLConstraint::PRIMARY_KEY);

        REGISTER_MEMBER(m_Name, ESQLDataType::TEXT);
    }

    void ImportMembers(char** Vals) override
    {
        this->m_ID = std::stoi(Vals[0]);

        this->m_Name = Vals[1];
    }

private:

    int m_ID = 0;
    
    std::string m_Name;
};

EXPORT_CLASS(Employee)

static int CountCallback(void* Obj, int argc, char** argv, char** azColName)
{
    int* Count = static_cast<int*>(Obj);

    *Count = std::stoi(argv[0]);
    return 0;
}

int main()
{

    ESQLResult result1 = db.CreateTable<Person>();
    ESQLResult result2 = db.CreateTable<Dog>();
    ESQLResult result3 = db.CreateTable<Employee>();
    
    Person P1("Harry", 25);
    db.Write(&P1);

    Dog D1("Pluto", "Sam");
    db.Write(&D1);

    Employee EMP1(101,"Robert");
    db.Write(&EMP1);

    Person* P2 = db.Read<Person>("m_Name = 'Harry'");

    db.Delete<Dog>("m_Name = 'Pluto'");

    Dog D2("Jack", "Jason");
    db.Write(&D2);

    ESQLResult result4 = db.DeleteTable<Employee>();

    Dog D3("Sam", "Jason");
    db.Update<Dog>(&D3, "m_Name = 'Jack'");

    Dog D4("Lucie", "John");
    db.Write(&D4);

    Dog D5("Ronnie", "John");
    db.Write(&D5);

    Dog D6("Luno", "John");
    db.Write(&D6);

    std::vector<Dog*> DBDATA = db.ReadAll<Dog>("m_Owner = 'John'");

    return 0;
}