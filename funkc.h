#ifndef FUNKC_H
#define FUNKC_H

#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <exception>
#include <limits>

void debug(std::string s){
    std::cout << s << std::endl;
}

template <class T>
int oom(T x){
    int temp = x;
    int oom = 0;
    while(temp > 0){
        temp/=10;
        oom++;
    }
    return oom;
}

template <class T>
std::string print_vec(std::vector<T> vec){
    std::string s;
    s+= "{ ";
    for(int i = 0; i < vec.size(); i ++){
        std::stringstream ss;
        ss << std::setprecision(oom(vec[i]) + 2) << vec[i];
        s+= ss.str();
        if(i != vec.size()-1){
            s+= ", ";
        }
    }
    s+= "}";
    return s;
}

template <class T>
std::string print_vec_json(std::vector<T> vec){
    std::string s;
    char index[15] = {'x', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};
    s+= "{ ";
    for(int i = 0; i < vec.size(); i ++){
        std::stringstream ss;
        ss << "\"" << index[i] << "\": "<< std::setprecision(oom(vec[i]) + 2) << vec[i];
        s+= ss.str();
        if(i != vec.size()-1){
            s+= ", ";
        }
    }
    s+= "}";
    return s;
}

template <class T>
std::string print(std::vector<T>** array, int N, int M, bool Print_in_terminal = 0, bool New_lines = 0){
    std::stringstream ss;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            ss << print_vec(array[i][j]);
            if(j!=M-1){ss << ", ";};
        }
        if(New_lines){ss << "\n";}
    }
    std::string s = ss.str();
    if(Print_in_terminal){std::cout << s << "\n ";}
    return s;
    
}

template <class T>
std::string print_json(std::vector<T>** array, int N, int M, bool Print_in_terminal = 0, bool New_lines = 0){
    std::stringstream ss;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            ss << "\"" << i  << "_"<< j  << "\": ";
            ss << print_vec_json(array[i][j]);
            if((j!=M-1 || !New_lines) && (j!=M-1 || i!= N-1)){ss << ", ";};
        }
        if(New_lines){ss << "\n";}
    }
    std::string s = ss.str();
    if(Print_in_terminal){std::cout << s << "\n ";}
    return s;
    
}

template <class T>
std::string print(T** array, int N, int M, bool Print_in_terminal = 0){
    std::stringstream ss;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            ss << array[i][j];
            if(j!=M-1){ss << ", ";};
        }
        ss << "\n";
    }
    std::string s = ss.str();
    if(Print_in_terminal){std::cout << s << "\n ";}
    return s;
}


template <class T, class V>
void to_file(V t, T** tab, std::ofstream& file, int N, int M){
    file << t << ", ";
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < M; j++){
            if(i != N-1 || j!= M-1){
                file << tab[i][j] << ", ";
            }else{
            file << tab[i][j];
            }
        }
    }
    file << "\n";
}

template <class T>
void null_array(T** array, int N, int M){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            array[i][j] = 0;
        }
    }
}

std::string lower(std::string& str){
    std::transform(str.begin(), str.end(), str.begin(),
       [](unsigned char c){ return std::tolower(c); });
    return str;
}

template <class T, class V, class U>
void read_value_and_check_type(T& value, V min = std::numeric_limits<T>::min(), U max = std::numeric_limits<T>::max(), bool can_be_equal = true){
    max = T(max);
    min = T(min);
    bool wrong_type_flag = true;
    while(wrong_type_flag){
        wrong_type_flag = false;
        try{
            std::cin >> value;

            if(std::cin.fail()) {
                throw std::invalid_argument("Podano niewlasciwy typ danych. ");
            }
            if(value < min) {
                std::stringstream ss;
                ss << "Podano zbyt mala wartosc. min = " << min << " ";
                throw std::out_of_range(ss.str());
            }
            else if(value > max){
                std::stringstream ss;
                ss << "Podano zbyt duza wartosc. max = " << max << " ";
                throw std::out_of_range(ss.str());
            }
            else if(!can_be_equal){
                if(value == min) {
                    std::stringstream ss;
                    ss << "Podano dolna niedozwolona wartosc graniczna. min = " << min << " ";
                    throw std::out_of_range(ss.str());
                }
                else if(value == max){
                    std::stringstream ss;
                    ss << "Podano gorna niedozwolona wartosc graniczna. max = " << max << " ";
                    throw std::out_of_range(ss.str());
                } 
            }
        }catch(const std::invalid_argument& e){
            std::cout << e.what() << "Sproboj ponownie: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignoruje błędne dane
            wrong_type_flag = true;
        }catch (const std::out_of_range& e) {
            std::cout << e.what() << "Sprobuj ponownie: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            wrong_type_flag = true;
        }
    }
    std::cin.clear();
}

template <class T, class V>
void read_value_and_check_type(T& value, V min = std::numeric_limits<T>::min(), bool can_be_equal = true){
    read_value_and_check_type(value, min, std::numeric_limits<T>::max(), can_be_equal);
}

template <class T>
void read_value_and_check_type(T& value){
    bool wrong_type_flag = true;
    while(wrong_type_flag){
        wrong_type_flag = false;
        try{
            std::cin >> value;

            if (std::cin.fail()) {
                throw std::invalid_argument("Podano niewlasciwy typ danych.");
            }

        }catch(const std::invalid_argument& e){
            std::cout << e.what() << "Sproboj ponownie: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            wrong_type_flag = true;
        }
    }
    std::cin.clear();
}

void print_parameters(const int N, const int M, const int Nsmp, const double dx, const double dy, const double Fsmp){
    std::cout << "N = " << N << " M = " << M << " Nsmp = " << Nsmp << " dx = " << dx << " dy = " << dy << " Fsmp = " << Fsmp <<"\n";
}

void read_parameters(int& N, int& M, int& Nsmp, double& dx, double& dy, double& Fsmp){
    std::string task = "read";
    bool first_iteration = true;
    while(task != "n" && task != "next"){
        //pytanie o typ zadania
        if(first_iteration){
            std::cout << "w -> wprowadz parametry, g -> skorzystaj z przykladowych parametrow:\n";
        }
        else{
            std::cout << "w -> wprowadz wszystkie parametry ponownie, p -> popraw jeden parametr, g -> skorzystaj z przykladowych parametrow, n -> zakoncz wprowadzanie parametrow: ";
        }
        read_value_and_check_type(task);
        lower(task);  

        //opcje dla konkretnych zadan
        if(!task.compare("wprowadz") || !task.compare("w") || !task.compare("write")){
            std::cout << "N: ";
            read_value_and_check_type(N, 0, false);
            std::cout << "M: ";
            read_value_and_check_type(M, 0, false);
            std::cout << "Nsmp: ";
            read_value_and_check_type(Nsmp, 0, false);
            std::cout << "dx: ";
            read_value_and_check_type(dx, 0.0, false);
            std::cout << "dy: ";
            read_value_and_check_type(dy, 0.0, false);
            std::cout << "Fsmp: ";
            read_value_and_check_type(Fsmp, 0.0, false);
            print_parameters(N, M, Nsmp, dx, dy, Fsmp); 
            if(first_iteration){
                first_iteration = false;
            }
            continue;
        }
        else if(!task.compare("popraw") || !task.compare("p") || !task.compare("pop")){
            std::cout << "ktora wartosc poprawic? wprowadz jej nazwe: ";
            std::string name;
            read_value_and_check_type(name);
            lower(name);
            if(!name.compare("n")){
                std::cout << "wprowadz nowa wartosc dla N: ";
                read_value_and_check_type(N, 0, false);
            }else if(!name.compare("m")){
                std::cout << "wprowadz nowa wartosc dla M: ";
                read_value_and_check_type(M, 0, false);
            }else if(!name.compare("nsmp")){
                std::cout << "wprowadz nowa wartosc dla Nsmp: ";
                read_value_and_check_type(Nsmp, 0, false);
            }else if(!name.compare("dx")){
                std::cout << "wprowadz nowa wartosc dla dx: ";
                read_value_and_check_type(dx, 0.0, false);
            }else if(!name.compare("dy")){
                std::cout << "wprowadz nowa wartosc dla dy: ";
                read_value_and_check_type(dy, 0.0, false);
            }else if(!name.compare("Fsmp")){
                std::cout << "wprowadz nowa wartosc dla Fsmp: ";
                read_value_and_check_type(Fsmp, 0.0, false);
            }
            print_parameters(N, M, Nsmp, dx, dy, Fsmp);
            if(first_iteration){
                first_iteration = false;
            }
            continue;
        }
        else if(!task.compare("g") || !task.compare("gotowe")){
            print_parameters(N, M, Nsmp, dx, dy, Fsmp);
            if(first_iteration){
                first_iteration = false;
            }
            continue;
        }
        if(task.compare("n") && task.compare("next")){
            std::cout << "niepoprawna opcja, sproboj jeszcze raz\n";
        }
    }
}

void explain(){
    std::cout << "\nProgram symuluje padanie kilku roznych fal plaskich na szyk fazowany (phased array) w celu pozyskania probek.\n";
    std::cout << "Antena polozona jest na osi XY, jej elementy sa rozmieszczone w rownych odleglosciach o dx i kierunku osi X i dy w kierunku osi Y.\n";
    std::cout << "Skrajna lewa dolna (na rysunku) antena znajduje sie w punkcie o wspolrzednych (0 0 0).\n";
    std::cout << "Ilosc anten w rzedzie na osi X okresla zmienna M, a w osi Y zmienna N. Symbole # oznaczaja pojedyncze anteny.\n";
    std::cout << "Zbieranych jest Nsmp probek, czestotliwosc probkowania Fsmp.\n";
    std::cout << " Y\n ^ dx\n | <->       M\n | <------------------>\n | # # # # # # # # # # ^\n | # # # # # # # # # # |\n | # # # # # # # # # # |N\n | # # # # # # # # # # | ^ dy\n | # # # # # # # # # # v v\n-|-------------------------> X\n\n";
}

//poprawic wychodzenie, bo przy n pokazuje ze blad
#endif