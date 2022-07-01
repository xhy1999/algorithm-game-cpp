#include <iostream>
#include <map>

using namespace std;

string map_2_json_string(map<string, string> m) {
	string jsonString = "{";
	for (map<string, string>::iterator iter = m.begin(); iter != m.end(); iter++) {
		jsonString += "\"" + iter->first + "\": " + "\"" + iter->second + "\", ";
	}
	return jsonString.substr(0, jsonString.length() - 2) + "}";
}