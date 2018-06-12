#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Journal
{
	string title;
	vector<string> entries;

	Journal(const string& title):title(title){}

	void add_entries(string entry)
	{
		static int count = 1;
		
		entries.push_back(to_string(count++) + ": " + entry);
	}

	
};

class PersistenceManager	// Create a persistence manager class to deal with 
{
public:
	void static save(const Journal& j,const string& filename)
	{
		ofstream ofs(filename);
		for (auto& e : j.entries)
		{
			ofs << e << endl;
		}
	}
};

int main(int argc, char* argv[])
{
	Journal journal("NewYork");
	journal.add_entries("Disney");
	journal.add_entries("Global cinema");
	PersistenceManager::save(journal, "somewhere");
	
	for (auto entry : journal.entries)
	{
		cout << entry << " ";
	}
	char c;
	cin >> c;
}
