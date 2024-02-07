#include <bits/stdc++.h>
#include "headers/basic.h"
#include "headers/rsi.h"
#include "headers/mcad.h"
using namespace std;

int main(int argc, char *argv[])
{
    std::string strat = argv[1];
    std::string symbol = argv[2];
    int n = std::stoi(argv[3]);
    int x = std::stoi(argv[4]);
    std::string start = argv[5];
    std::string end = argv[6];
    stringstream ss;
    ss << "python3 ../main.py " << symbol << " " << n << " " << start << " " << end;
    system(ss.str().c_str());
    // rest of your code
    if (strat == "BASIC")
    {
        
    }
    else if (strat == "rsi")
    {
        std::cout << "Running RSI strategy for " << symbol << "\n";
    }
    else
    {
        std::cout << "Invalid strategy\n";
    }
}