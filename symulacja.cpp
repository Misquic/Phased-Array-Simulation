#include <iostream>
#include <vector>
#include <complex>
#include "Wave.h"
#include "Waves.h"
#include "funkc.h"
#include "all.h"




int main(){
    int N = 15, M = 20, Nsmp = 100;
    type_param dx = 0.01, dy = 0.005, Fsmp = 5.0e9;

    std::ofstream signal_file;
    signal_file.open("signal.csv");

    std::ofstream details_file;
    details_file.open("details.json");

    std::vector<Wave<type_param>> Waves;

    explain();
    read_parameters(N, M, Nsmp, dx, dy, Fsmp);
    read_waves(Waves, Fsmp);

    std::vector<double>** positions = new std::vector<double>*[N]; // inicjowanie tablicy pozycji anten
    for(int i = 0; i < N; i++){
        positions[i] = new std::vector<double>[M];
    }

    for(int i = 0; i < N; i++){ // wypełnianie tablicy pozycji anten (kwadrat)
        for(int j = 0; j < M; j++){
            positions[i][j] = {j*dx, (N-i-1)*dy, 0.0};
        }
    }

    //print(positions, N, M, true, true); //wypisywanie tablicy pozycji 

    type_signal** signal = new type_signal*[N]; // inicjowanie tablicy wartości sygnału w antenach
    for(int i = 0; i < N; i++){
        signal[i] = new type_signal[M];
        for(int j = 0; j < M; j++){
            signal[i][j] = 0;
        }
    }

    type_param dt = 1/Fsmp;
    type_param t = 0.0;
    for(int i = 0; i < Nsmp; i ++){
        std::cout << "\r             \r" << float(i+1)/Nsmp*100.0 << "%"; 
        measure_signal(signal, positions, Waves, t, N, M);
        
        to_file(t, signal, signal_file, N, M);
        t+=dt;
    }
    std::cout << "\nzapisywanie\n";

    details_to_file(details_file, Nsmp, N, M, Fsmp, dx, dy, Waves, positions);
    
    signal_file.close();
    details_file.close();

    //cleanup
    for(int i = 0; i < N; i++){
        delete positions[i];
    }
    delete positions;

    for(int i = 0; i < N; i++){
        delete signal[i];
    }
    delete signal;

}