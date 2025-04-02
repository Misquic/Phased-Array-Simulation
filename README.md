# Phased Array Simulation

This project is a simulation of planar waves hitting phased array (arrays set up in grid). 

Part in c++ simulates waves and produces signal recieved by array. It has UI in terminal.

Part in matlab processes obtained signal using FFT to work out where has/have the signal/signals came from.

More in documentation (in polish) [](Dokumentacja_DSP.pdf)

### How to build?
Compile:
```
g++ -o simulation *.cpp -O2
```
Then run simulation, using built in UI to create array and wave setup. Simulation results in details.json and signal.csv files. That files are processed in matlab, simply run analiza.m in matlab and results will be shown.

