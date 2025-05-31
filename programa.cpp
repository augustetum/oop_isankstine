#include "biblioteka.h"

int main(){
    map<string, zodzioInfo> zodziai;
    string pasirinktasFailas = pasirinktiFaila();
    vector<string> nuorodos;
    nuskaitytiFaila(pasirinktasFailas, zodziai, nuorodos);
    isvestiFaila(zodziai, nuorodos);
}