#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

vector <string> xml_vector(string xml) {
	stack <string> s;
	ifstream file(xml);
	string line;
	vector <string> temp;
	int i = 0;


	for (; getline(file, line); i++) {
		bool whiteSpacesOnly = std::all_of(line.begin(), line.end(), isspace);
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
		if (whiteSpacesOnly)temp.push_back(line + " ");
		else temp.push_back(line);
	}
	return temp;
}
string formatting(string xml) {

	vector <string> x;
	string line;
	x = xml_vector(xml);
	string formatted;
	int space_count = 0;
	for (int i = 0; i < x.size(); i++) {
		line = x[i];
		if (line[0] == '<') {
			if (line.find_first_of('<') != line.find_last_of('<')) {
				for (int j = 1; j < space_count; j++) {
					formatted.append("\t");
				}
				formatted.append(line);
				formatted.append("\n");
			}
			else if (line[1] == '/') {
				space_count--;
				for (int j = 1; j < space_count; j++) {
					formatted.append("\t");
				}
				formatted.append(line);
				formatted.append("\n");

			}
			else {
				for (int j = 1; j < space_count; j++) {
					formatted.append("\t");
				}
				formatted.append(line);
				formatted.append("\n");
				space_count++;
			}
		}
		else {
			for (int j = 1; j < space_count; j++) {
				formatted.append("\t");
			}
			formatted.append(line);
			formatted.append("\n");
		}
	}
	return formatted;
}
int main()
{
	string s,c;
	s = formatting("sample.xml");
	cout << s;
}	
