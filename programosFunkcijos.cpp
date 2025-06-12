#include "biblioteka.h"

vector<string> nuoroduPabaigos = nuoroduFailas();
map<string, zodzioInfo> zodziaiSuTow;
int towCount = 0;

void nuskaitytiFaila(string fail, map<string, zodzioInfo>& zodziai, vector<string>& nuorodos){
    string eilut;
    int pazymys;
    ifstream failas(fail);
        if (!failas.is_open()){
            throw std::runtime_error("Failo nepavyko atidaryti arba jis neegzistuoja šiame aplankale");
        }

        int eilutesIndex = 1;
        while(getline(failas, eilut)){
            istringstream eilute(eilut);
            string zodis;
            while(eilute >> zodis){
                if(!zodis.empty()){
                    if (arNuoroda(zodis)){
                        while (!zodis.empty() && ispunct(zodis.front()) && zodis.front() != '.') {
                            zodis.erase(0, 1);
                        }
                        while (!zodis.empty() && ispunct(zodis.back()) && zodis.back() != '/') {
                            zodis.pop_back();
                        }
                        nuorodos.push_back(zodis);
                    } else {
                        string zod = zodzioTaisymas(zodis);
                        if(towChecker(zod)){
                            zodziaiSuTow[zod].pasikartojimai++;
                            towCount++;
                        }
                        if(!zod.empty()){
                            zodziai[zod].pasikartojimai++;                   
                            zodziai[zod].eilutes.push_back(eilutesIndex);     
                        }
                        }
                }
            }
            eilutesIndex++;
        }
        failas.close();
}

string pasirinktiFaila(){
    string eilut;
    int pazymys;
    std::stringstream buferis;

    while(true){
        try{
            #ifdef _WIN32
            system("dir /b *.txt > temp.txt");
            #else
            system("ls *.txt > temp.txt");
            #endif
            ifstream tempFail("temp.txt");
            vector<string> failuPav;
            std::unordered_set<string> nenorimiFailai = {"rezultatai.txt", "temp.txt", "link_endings.txt", "nuorodos.txt", "cross-reference.txt", "zodziaiSuTow.txt"};
            string failoPav;
            while(getline(tempFail, failoPav)){
                if (nenorimiFailai.find(failoPav) == nenorimiFailai.end()) {
                    failuPav.push_back(failoPav);
                }
            }
            tempFail.close();
            remove("temp.txt");
            string fail;

            cout << "Pasirinkite norimą failą" << endl;
            for(int i = 1; i <= failuPav.size(); i++){
                cout << i << "| " << failuPav[i-1] << endl;
            }
            int choice;
            cin >> choice;
            if (choice < 1 || choice > failuPav.size()) {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::runtime_error("Neteisingas failo pasirinkimas");
            } else {
                 fail = failuPav[choice-1];
            }
            return fail;
            break;
        } catch (const std::runtime_error &e) {
            cout << e.what() << endl;
            continue;
        } catch (const char* e){
            cout << e << endl;
            throw;
        }
    }
}

std::string zodzioTaisymas(std::string &zod) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring zod_w = converter.from_bytes(zod); //convertina is UTF-8 i wstring

    std::wstring nepageidaujami = L"'-=+@#$%*()±•/{}[]°′″≥ⓘ–—•−©·。€„“";
    std::wstring rez_w;

    for (wchar_t c : zod_w) {
        c = towlower(c);
        if (!iswpunct(c) && !iswdigit(c) && nepageidaujami.find(c) == std::wstring::npos) {
            rez_w += c;
        }
    }

    return converter.to_bytes(rez_w);
}

vector<string> nuoroduFailas(){
    ifstream nuoroduFail("link_endings.txt");
    vector<string> nuoroduPabaigos;

    if (!nuoroduFail.is_open()) {
        cout << "Nuorodų failas neatsidarė" << endl;
    }
    string nuorodosPabaiga;
    getline(nuoroduFail, nuorodosPabaiga); //pirmą eilutę praleidžia, nes ten komentaras
    while(getline(nuoroduFail, nuorodosPabaiga)){
        nuoroduPabaigos.push_back(nuorodosPabaiga);
    }
    return nuoroduPabaigos;
}

bool arNuoroda(string zodis){
    while (!zodis.empty() && ispunct(zodis.front()) && zodis.front() != '.') {
        zodis.erase(0, 1);
    }
    while (!zodis.empty() && ispunct(zodis.back()) && zodis.back() != '/') {
        zodis.pop_back();
    }

    if (zodis.find("http://") == 0 || zodis.find("https://") == 0 || 
        zodis.find("www.") == 0 || zodis.find("ftp://") == 0) {
        return true;
    }

    if (zodis.find(".") == 0){ //kai prasideda tašku
        return false;
    }
    
    size_t taskoPozicija = zodis.find_last_of(".");
    if (taskoPozicija == string::npos || taskoPozicija == zodis.length() - 1) {
        return false;
    }
    
    string poTasko = zodis.substr(taskoPozicija + 1);

    size_t slashPozicija = poTasko.find('/');
    string zodzioGalas;
    
    if (slashPozicija != string::npos) {
        zodzioGalas = poTasko.substr(0, slashPozicija);
    } else {
        zodzioGalas = poTasko;
    }
    
    std::transform(zodzioGalas.begin(), zodzioGalas.end(), zodzioGalas.begin(), ::tolower);
    for(const string& s : nuoroduPabaigos){
        string lowerS = s;
        std::transform(lowerS.begin(), lowerS.end(), lowerS.begin(), ::tolower);
        if(zodzioGalas == lowerS){
            return true;
        }
    }
    return false;
}

void isvestiFaila(map<string, zodzioInfo>& zodziai, vector<string>& nuorodos){
    std::ofstream failas1("rezultatai.txt");
    std::ofstream failas2("cross-reference.txt");
    std::ofstream failas3("nuorodos.txt");
    std::ofstream failas4("zodziaiSuTow.txt");

    if(!failas1.is_open() || !failas2.is_open() || !failas3.is_open()) {
        std::cerr << "Nepavyko atidaryti rezultatų failo rašymui." << std::endl;
        return;
    }

    failas1 << "Pasikartojimai\tŽodis\t" << std::endl;
    failas1 << "----------------------------------------------------------------------" << std::endl;

    for (const auto& i : zodziai) {
        failas1 << i.second.pasikartojimai << "\t" << i.first << "\t";
        failas1 << endl;
    }

    failas2 << "Žodis\tEilutės\t" << std::endl;
    failas2 << "----------------------------------------------------------------------" << std::endl;
    for (const auto& i : zodziai) {
        failas2 << std::left << std::setw(20) << i.first;
        for (size_t x = 0; x < i.second.eilutes.size(); ++x) {
            if (x > 0) failas2 << ", ";
            failas2 << i.second.eilutes[x];
        }
        failas2 << endl;
    }

    failas3 << "NUORODOS:" << std::endl;
    for (const auto& nuoroda : nuorodos) {
        failas3 << nuoroda << endl;
    }

    failas4 << "ŽODŽIAI SU TOW" << endl;
    for (const auto& i : zodziaiSuTow) {
        failas4 << i.second.pasikartojimai << "\t" << i.first << "\t";
        failas4 << endl;
    }

    failas4 << endl << "Iš viso žodžių su \"tow\": " << towCount << endl;

    failas1.close();
    failas2.close();
    failas3.close();
    failas4.close();
}

bool towChecker(const string& zod){
    string zodzisLower = zod;
    std::transform(zodzisLower.begin(), zodzisLower.end(), zodzisLower.begin(), ::tolower);
    return zodzisLower.find("tow") != string::npos;
}




