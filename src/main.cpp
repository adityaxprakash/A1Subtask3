#include <bits/stdc++.h>
#include "dma.h"
#include "dma_imp.h"
#include "rsi.h"
#include "macd.h"
#include "adx.h"
#include "basic.h"

using namespace std;

string get_stock_data(string symbol, int n, string start_date, string end_date, string strat)
{
    string filename = "build/" + symbol + ".csv";
    string command = "python3 src/fetch_data.py " + symbol + " " + start_date + " " + end_date + " " + to_string(n) + " " + filename+" "+strat;
    system(command.c_str());

    return filename;
}

int main(int argc, char *argv[])
{
    string strat = argv[1];
    string symbol = argv[2];
    int n = stoi(argv[3]);
    double x = stod(argv[4]);
    string start_date = argv[5];
    string end_date = argv[6];
    int p = stoi(argv[7]);
    int max_hold = stoi(argv[8]);
    double c1 = stod(argv[9]);
    double c2 = stod(argv[10]);
    double oversold = stod(argv[11]);
    double overbought = stod(argv[12]);
    double adx_thresh = stod(argv[13]);
    string train_start = argv[14];
    string train_end = argv[15];

    if(strat=="MACD")
        n=35;
    
    string infile_name = get_stock_data(symbol, n, start_date, end_date,strat);
    string cashflow_name = "build/daily_cashflow.csv";
    string order_name = "build/order_statistics.csv";
    string pandl_name = "build/final_pnl.txt";

    if (strat == "DMA")
    {
        dma tool(start_date, end_date, n, x, p);
        tool.run(infile_name, cashflow_name, order_name, pandl_name);
    }
    else if (strat == "DMA++")
    {
        dma_imp tool(start_date, end_date, n, x, p, max_hold, c1, c2);
        tool.run(infile_name, cashflow_name, order_name, pandl_name);
    }
    else if (strat == "RSI")
    {
        rsi tool(start_date, end_date, n, x, oversold, overbought);
        tool.run(infile_name, cashflow_name, order_name, pandl_name);
    }
    else if(strat=="MACD")
    {
        macd tool(start_date, end_date, x, n);
        tool.run(infile_name, cashflow_name, order_name, pandl_name);
    }
    else if(strat=="ADX")
    {
        adx tool(start_date,end_date, x, n, adx_thresh);
        tool.run(infile_name, cashflow_name, order_name, pandl_name);
    }
    else if(strat=="BASIC")
    {
        basic tool(start_date,end_date, x, n);
        tool.run(infile_name, cashflow_name, order_name, pandl_name);
    }
    else
    {
        cout << "Invalid strategy\n";
    }
}