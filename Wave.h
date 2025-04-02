#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <complex>
#include "funkc.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cstring>
#include <sstream>
using namespace std::complex_literals;

template <class T>
class Wave{
    private:
    const std::complex<T> j = 1i;
    T A; //amplitude
    std::vector <T> k; //Wave vector 
    T w; //angular frequency
    void correctk();


    public:
    Wave(T AA, std::vector<T> kk, T ww);
    Wave();
    Wave& operator=(const Wave& waveB);
    bool operator!=(const Wave& waveB);

    void set(std::string name, T value);
    void set(std::string name, std::vector<T> value);
    void read(T Fsmp);
    T real(std::vector<T> x, T t);
    T imag(std::vector<T> x, T t);
    T abs(std::vector<T> x, T t);
    T point(std::vector<T> x, std::vector<T> y);
    std::string print(bool Print_in_terminal = 0);
    std::string print_json(bool Print_in_terminal = 0);

};

///////////////////////////////////////////////////////

template <class T>
void Wave<T>::correctk(){ // ensures wave is propagating with volocity of light
    T len = sqrt(point(this->k, this->k));
    T new_len = this->w/299792458.0;
    this->k[0] = this->k[0]/len*new_len;
    this->k[1] = this->k[1]/len*new_len;
    this->k[2] = this->k[2]/len*new_len;
    len = sqrt(point(this->k, this->k));
}

template <class T>
Wave<T>::Wave(T AA, std::vector <T> kk, T ff){
    this->A = AA;
    this->w = 2.0 * M_PI * ff;
    if(kk.size() == 3){
        this->k = kk;
        correctk();
    }
    else{
        std::cout << "error Wave(..., k, ...)";
    }
    
};

template <class T>
Wave<T>::Wave(){
    this->A = 1.0;
    this->w = 1.0;
    std::vector<T> kk = {1, 1, 1};
    this->k = kk;
    correctk();
    
};

template <class T>
Wave<T>& Wave<T>::operator=(const Wave<T>& waveB){
    if(this != &waveB){
        this->A = waveB.A;
        this->w = waveB.w;
        this->k = waveB.k;
        correctk();
    }
    return *this;
}

template <class T>
bool Wave<T>::operator!=(const Wave<T>& waveB){
    if(this->A != waveB.A) return 0;
    if(this->w != waveB.w) return 0;
    if(this->k != waveB.k) return 0;
    return 1;
}

template <class T>
T Wave<T>::real(std::vector<T> x, T t){return std::real( this->A * std::exp( this->j * (point(this->k, x) - this->w*t)) );};

template <class T>
T Wave<T>::imag(std::vector<T> x, T t){return std::imag( this->A * std::exp( this->j * (point(this->k, x) - this->w*t)) );};

template <class T>
T Wave<T>::abs(std::vector<T> x, T t){return std::abs( this->A * std::exp( this->j * (point(this->k, x) - this->w*t)) );};

template <class T> 
T Wave<T>::point(std::vector<T> x, std::vector<T> y){
    return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
};

template <class T>
void Wave<T>::set(std::string name, T value){
    lower(name);

    if(!name.compare("w")){this->w = value; return;}
    if(!name.compare("f")){this->w = 2 * M_PI * value; return;}
    if(!name.compare("a")){this->A = value; return;}
    std::cout << "error Wave.set";
};

template <class T>
void Wave<T>::set(std::string name, std::vector<T> value){
    if(!name.compare("k") && value.size() == 3){
        this->k = value;
        correctk();
    }
    else{
        std::cout << "error Wave.set(\"k\")";
    }
};

template <class T> 
std::string Wave<T>::print(bool Print_int_terminal){
    std::vector<T> kk = this->k;
    std::stringstream ss;
    ss << A << " * exp[ i*(" << print_vec(kk) << " * x - " << this->w/2.0/M_PI << " * 2PI t)], lambda = " << 2*M_PI/sqrt(point(this->k, this->k));
    std::string s = ss.str();
    if(Print_int_terminal){std::cout << s << "\n ";}
    return s;
};

template <class T> 
std::string Wave<T>::print_json(bool Print_int_terminal){
    std::stringstream ss;
    
    ss << "[{ \"A\":" << this->A << ", \"k\": [" << print_vec_json(this->k) << "], \"f\": "<<this->w/2.0/M_PI<< "}]";
    std::string s = ss.str();
    if(Print_int_terminal){std::cout << s << "\n ";}
    return s;
};

template <class T>
void Wave<T>::read(T Fsmp){
    T value;
    std::cout << "Podaj amplitude: ";
    read_value_and_check_type(value, 0);
    this->A = value;
    
    std::cout << "Podaj czestotliwosc: ";
    read_value_and_check_type(value, 0, false);
    if(value >= Fsmp/2.0){
        std::cout << "Zaleca się by czest fali byla mniejsza od czest Nyquista, by uniknac aliasingu\n";
    }
    this->w = 2 * M_PI * value;

    T x, y, z;
    std::cout << "Podaj wspolrzedne wektora falowego x y z: ";
    read_value_and_check_type(x);
    read_value_and_check_type(y);
    read_value_and_check_type(z);
    std::vector kk = {x, y, z};
    this->k = kk;
    correctk();

    

}





#endif