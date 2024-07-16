#include <iostream>
#include "contacts.pb.h"
using namespace std;
int main()
{
    string people_str;  

    {
        contacts::PeopleInfo people;
        people.set_age(20);
        people.set_name("忘忧");

        if(!people.SerializeToString(&people_str))
        {
            cout << "序列化联系人失败" <<endl;
        }

        cout << "序列化之后的 people_str: " << people_str << endl;

        // 反序列化
        {
            contacts::PeopleInfo people;
            if(!people.ParseFromString(people_str))
            {
                cout << "反序列化联系人失败" <<endl;
            }
            
            cout << "Parse age: " << people.age() << endl;
            cout << "Parse name: " << people.name() << endl;
        }
    }
    return 0;
}