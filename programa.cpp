#include "biblioteka.h"

int main(){
    map<string, zodzioInfo> zodziai;
    string pasirinktasFailas = pasirinktiFaila();
    vector<string> nuorodos;
    nuskaitytiFaila(pasirinktasFailas, zodziai, nuorodos);
    for (auto i : zodziai){
        cout << i.first << " " << i.second.pasikartojimai <<  " Eilutėse: ";
         cout << i.second.eilutes[0];
        for(int x = 1; x < i.second.eilutes.size(); x++){
            cout << ", ";
            cout << i.second.eilutes[x];
        }
        cout << endl;
    }
    cout << "--------------NUORODOS-------------"<<endl;
    for(auto i : nuorodos){
        cout << i << endl;
    }
}