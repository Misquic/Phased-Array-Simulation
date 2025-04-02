#ifndef WAVES_H
#define WAVES_H

// write as class?

#include "Wave.h"

template <class T>
std::string print(std::vector<Wave<T>> Waves, bool Print_in_terminal = 0, bool New_lines = 0){
    std::stringstream ss;
        for(int i = 0; i < Waves.size(); i++){
        ss << "\"wave_" << i << "\": \"" << Waves[i].print() << "\"";
        if(i!= Waves.size()-1){
            ss << ", ";
            if(New_lines){
                ss << "\n";
            }
        }
    }
    std::string s = ss.str();
    if(Print_in_terminal){std::cout << s << "\n";}
    return s;
}

template <class T>
std::string print_json(std::vector<Wave<T>> Waves, bool Print_in_terminal = 0, bool New_lines = 0){
    std::stringstream ss;
        for(int i = 0; i < Waves.size(); i++){
        ss << "\"wave_" << i << "\": " << Waves[i].print_json();
        if(i!= Waves.size()-1){
            ss << ", ";
            if(New_lines){
                ss << "\n";
            }
        }
    }
    std::string s = ss.str();
    if(Print_in_terminal){std::cout << s << "\n";}
    return s;
}

template <class T>
void details_to_file(std::ofstream& file, int Nsmp, int N, int M, T Fsmp, T dx, T dy, std::vector<Wave<T>> Waves, std::vector<double>** positions){
    file << "{";
    file << "\t\"Nsmp\": " << Nsmp << ", \n";
    file << "\t\"Fsmp\": " << Fsmp << ", \n";
    file << "\t\"dx\": " << dx << ", \n";
    file << "\t\"dy\": " << dy << ", \n";
    file << "\t\"N\": " << N << ", \n";
    file << "\t\"M\": " << M << ", \n";
    file << "\t\"Waves\": \n\t[\n{" << print_json(Waves, false, true) << "}\n],\n";
    file << "\t\"positions\": [{" << print_json(positions, N, M) << "}]";    
    file << "\n}\n";

};

template <class T>
std::string details_string(std::ofstream& file, int NNsmp, int NN, int MM, std::vector<Wave<T>> Waves, std::vector<double>** positions){
    std::stringstream ss;
    ss << "{\n\t\"details\":[\n{";
    ss << "\t\"Nsmp\": " << NNsmp << ", \n";
    ss << "\t\"NN\": " << NN << ", \n";
    ss << "\t\"MM\": " << MM << ", \n";
    ss << "\t\"Waves\": \n\t[\n{" << print(Waves, false, true) << "}\n],\n";
    ss << "\t\"positions\": \"" << print(positions, NN, MM) << "\"";    
    ss << "}\n\t\t]\n}";
    return ss.str();

};


template <class T, class V>
void measure_signal(T** signal, std::vector<V>** positions, std::vector<Wave<V>>& Waves, V t, int N, int M){
    null_array(signal, N, M);
    for(int l = 0; l < Waves.size(); l ++){
        for(int i = 0; i < N; i ++){
            for(int j = 0; j < M; j++){
                signal[i][j] += Waves[l].real(positions[i][j], t);
                //std::cout << Waves[l].real(positions[i][j], t) << "\n";
            }
        }
    }
}

template <class T>
void read_waves(std::vector<Wave<T>>& Waves, T Fsmp){
    std::string task = "read";
    bool first_iteration = true;
    while(task != "n" && task != "next"){
        if(first_iteration){
            std::cout << "w -> wprowadz nowa fale, g -> skorzystaj z przykladowych fal: ";
        }
        else{
            std::cout << "w -> wprowadz nowa fale, p -> popraw fale, g -> skorzystaj z przykladowych fal, n -> zakoncz wprowadzanie fal: ";
        }
        read_value_and_check_type(task);
        lower(task);
        if(!task.compare("wczytaj") || !task.compare("w") || !task.compare("write")){
            Wave<T> wave;
            wave.read(Fsmp);
            Waves.push_back(wave);
            print(Waves, true, true);
            if(first_iteration){
                first_iteration = false;
            }
            continue;
            
        }
        if(!task.compare("popraw") || !task.compare("p") || !task.compare("pop")){
            Wave<T> wave;
            int i;
            std::cout << "ktora fale poprawic? numer: ";
            read_value_and_check_type(i, 0, Waves.size()-1);

            std::cout << "wprowadz nowa fale\n";
            wave.read(Fsmp);
            Waves[i] = wave;
            print(Waves, true, true);
            if(first_iteration){
                first_iteration = false;
            }
            continue;
        }
        if(!task.compare("g") || !task.compare("gotowe")){
            T A1 = 1.0, f1 = Fsmp/5.0, A2 = 0.5, f2 = f1*2.0;
            std::vector<T> k1 = {-1,0,-1}, k2 = {-1, -1, -1}; // gdy 0 na y to możemy traktować jako 1D antenę i 2D fale na osi XZ
            Wave<T> wave1(A1, k1, f1);
            Wave<T> wave2(A2, k2, f2);
            Waves.push_back(wave1);
            Waves.push_back(wave2);
            print(Waves, true, true);
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


#endif
