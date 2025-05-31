#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <cstring>
#include <locale>
#include <unicode/uchar.h>
#include <cwctype>
#include <codecvt>

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
void nuskaitytiFaila(string fail, map<string, zodzioInfo>& zodziai, vector<string>& nuorodos);
string zodzioTaisymas(string &zod);
vector<string> nuoroduFailas();
bool arNuoroda(string zodis);
void isvestiFaila(map<string, zodzioInfo>& zodziai, vector<string>& nuorodos);