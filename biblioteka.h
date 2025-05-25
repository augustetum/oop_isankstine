#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <unordered_set>
#include <sstream>

using std::cout;
using std::cin;
using std::string;
using std::map;
using std::vector;
using std::endl;
using std::ifstream;
using std::istringstream;

struct zodzioInfo {
    int pasikartojimai;
    vector<int> eilutes; 
};

string pasirinktiFaila();
void nuskaitytiFaila(string fail, map<string, zodzioInfo>& zodziai);