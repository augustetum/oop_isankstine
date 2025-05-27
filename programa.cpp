#include "biblioteka.h"

int main(){
    map<string, zodzioInfo> zodziai;
    string pasirinktasFailas = pasirinktiFaila();
    nuskaitytiFaila(pasirinktasFailas, zodziai);
    for (auto i : zodziai){
        cout << i.first << " " << i.second.pasikartojimai <<  " Eilutėse: ";
        for(int i : i.second.eilutes){
            cout << i << ", ";
        }
        cout << endl;
    }
}