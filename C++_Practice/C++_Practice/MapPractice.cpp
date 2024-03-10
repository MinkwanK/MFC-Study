#include <iostream>
#include <map>
using namespace std;


int main()
{
	map<string, int> m;
	m.insert({ "kim", 25 });
	m.insert({ "jin", 25 });



	if (m.find("kim") != m.end())
	{
		cout << "find" << endl;
	}
	else
	{
		cout << "not find" << endl;
	}

	for (map<string,int>::iterator iter = m.begin(); iter != m.end(); iter++)
	{
		iter->second = 12;
		cout << iter->first << " " << iter->second << endl;
		
	}

	cout << endl;
}