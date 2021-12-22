#include <iostream>
#include <chrono>
#include <iomanip>

#include "app.h"
#include "processor.h"

using namespace std;

int main(int argc, char *argv[])
{
    App a("../roms/maze.ch8");
    // App a("roms/IBM Logo.ch8");
    a.run();
    // Processor p;
    // p.load("IBM Logo.ch8");
    // // p.load("maze.ch8");
    // while (true)
    // {
    //     auto start = chrono::high_resolution_clock::now();
    //     p.print();
    //     auto end = chrono::high_resolution_clock::now();
    //     auto ms = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //     cout << "\rElapsed display: " << setw(9)
    //               << ms << " ms  ";

    //     start = chrono::high_resolution_clock::now();
    //     p.step();
    //     end = chrono::high_resolution_clock::now();
    //     ms = chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //     cout << "Elapsed step: " << setw(9)
    //               << ms << " ms ";
    //     // getchar();
    // }
    return 0;
}