#include "biblioteka.h"

void nuskaitytiFaila(string fail, map<string, zodzioInfo>& zodziai, vector<string>& nuorodos){
    string eilut;
    int pazymys;
    std::stringstream buferis;
    ifstream failas(fail);
        if (!failas.is_open()){
            throw std::runtime_error("Failo nepavyko atidaryti arba jis neegzistuoja šiame aplankale");
        }

        buferis << failas.rdbuf();
        failas.close();

        int eilutesIndex = 1;
        while(getline(buferis, eilut)){
            istringstream eilute(eilut);
            string zodis;
            while(eilute >> zodis){
                string zod = zodzioTaisymas(zodis);
                if(!zod.empty()){
                    if (arNuoroda(zodis)){
                        nuorodos.push_back(zodis);
                    } else {
                        zodziai[zod].pasikartojimai++;                   
                        zodziai[zod].eilutes.push_back(eilutesIndex);     
                    }
                }
            }
            eilutesIndex++;
        }
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
            std::unordered_set<string> nenorimiFailai = {"rezultatai.txt", "temp.txt", "link_endings.txt"};
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

string zodzioTaisymas(string &zod){
    std::locale loc("lt_LT.UTF-8");
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    std::wstring wzod = converter.from_bytes(zod);
    std::wstring wresult;
    
    for(wchar_t c: wzod){
        c = towlower(c);
        if (std::isalpha(c, loc)){  
            wresult += c;
        }
    }

    return converter.to_bytes(wresult);
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
    if (zodis.find("http://") == 0 || zodis.find("https://") == 0 || 
        zodis.find("www.") == 0 || zodis.find("ftp://") == 0) {
        return true;
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

    vector<string> nuoroduPabaigos = nuoroduFailas();
    for(const string& s : nuoroduPabaigos){
        string lowerS = s;
        std::transform(lowerS.begin(), lowerS.end(), lowerS.begin(), ::tolower);
        if(zodzioGalas == lowerS){
            return true;
        }
    }
    return false;
}

int skaiciuotiPlotiFormat(const std::string& str) {
    int width = 0;
    for (char c : str) {
        if ((c & 0xC0) != 0x80) {
            ++width;
        }
    }
    return width;
}

void isvestiFaila(map<string, zodzioInfo>& zodziai, vector<string>& nuorodos){
    std::ostringstream buferis;

    const int col1_width = 25;
    const int col2_width = 8;

    buferis << "Žodis" << std::string(col1_width - 5, ' ')
            << "Kartai" << std::string(col2_width - 6, ' ')
            << "Eilutės" << std::endl;

    buferis << std::string(70, '-') << std::endl;

    for (const auto& i : zodziai) {
        std::string zodis = i.first;
        int plotis = skaiciuotiPlotiFormat(zodis);
        buferis << zodis << std::string(col1_width - plotis, ' ');

        std::string kiek = std::to_string(i.second.pasikartojimai);
        buferis << kiek << std::string(col2_width - kiek.length(), ' ');

        for (size_t x = 0; x < i.second.eilutes.size(); ++x) {
            if (x > 0) buferis << ", ";
            buferis << i.second.eilutes[x];
        }

        buferis << std::endl;
    }

    buferis << std::endl
            << std::string(25, '-') << "NUORODOS" << std::string(25, '-') << std::endl;

    for (const auto& i : nuorodos) {
        buferis << i << std::endl;
    }

    std::ofstream failas1("rezultatai.txt");
    failas1 << buferis.str();
    failas1.close();
}

