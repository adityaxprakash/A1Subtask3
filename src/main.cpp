#include <bits/stdc++.h>
#include "dma.h"
#include "dma_imp.h"
#include "rsi.h"
#include "macd.h"
#include "adx.h"
#include "basic.h"
#include "lr.h"
#include <omp.h>

using namespace std;


void get_stock_data(string symbol, int n, string start_date, string end_date, string strat, string filename)
{

    string command = "python3 src/fetch_data.py " + symbol + " " + start_date + " " + end_date + " " + to_string(n) + " " + filename+" "+strat;
    system(command.c_str());

}

void parallel(string start_date, string end_date,  string cashflow_name, string order_name, string pandl_name, string symbol)
{
    // int x=5;
    // vector<double> results(6,0);
    // #pragma omp parallel
    // {
    //     #pragma omp single
    //     {
    //         #pragma omp task
    //         {
    //             string infile_name = get_stock_data(symbol, 7, start_date, end_date,"BASIC");
    //             basic t1(start_date, end_date, 5, 7);
    //             results[0] = t1.run(infile_name, cashflow_name, order_name, pandl_name);
    //         }

    //         #pragma omp task
    //         {
    //             string infile_name = get_stock_data(symbol, 50, start_date, end_date,"DMA");
    //             dma t2(start_date, end_date, 50, x, 2);
    //             results[1] = t2.run(infile_name, cashflow_name, order_name, pandl_name);
    //         }

    //         #pragma omp task
    //         {
    //             string infile_name = get_stock_data(symbol, 14, start_date, end_date,"DMA++");
    //             dma_imp t3(start_date, end_date, 14, x, 5, 28, 2, 0.2);
    //             results[2] = t3.run(infile_name, cashflow_name, order_name, pandl_name);
    //         }
    //         #pragma omp task
    //         {
    //             string infile_name = get_stock_data(symbol, 14, start_date, end_date,"RSI");

    //             rsi t4(start_date, end_date, 14, x, 30, 70);
    //             results[3] = t4.run(infile_name, cashflow_name, order_name, pandl_name);
    //         }

    //         #pragma omp task
    //         {
    //             string infile_name = get_stock_data(symbol, 0, start_date, end_date,"MACD");

    //             macd t5(start_date, end_date, x);
    //             results[4] = t5.run(infile_name, cashflow_name, order_name, pandl_name);
    //         }

    //         #pragma omp task
    //         {
    //             string infile_name = get_stock_data(symbol, 14, start_date, end_date,"ADX");
    //             adx t6(start_date, end_date, x, 14, 25);
    //             results[5] = t6.run(infile_name, cashflow_name, order_name, pandl_name);
    //         }
    //     }
    // }
    // cout << "Results: \n";
    // cout << "Basic: " << results[0] << "\n";
    // cout << "DMA: " << results[1] << "\n";
    // cout << "DMA++: " << results[2] << "\n";
    // cout << "RSI: " << results[3] << "\n";
    // cout << "MACD: " << results[4] << "\n";
    // cout << "ADX: " << results[5] << "\n";
}

int main(int argc, char *argv[])
{
    string strat = argv[1];
    string symbol = argv[2];
    int n = stoi(argv[3]);
    int x = stoi(argv[4]);
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

    
    string cashflow_name = "data/daily_cashflow.csv";
    string order_name = "data/order_statistics.csv";
    string pandl_name = "data/final_pnl.txt";

    if(strat=="LINEAR_REGRESSION")
    {
        string trainfile="data/"+symbol+"_train.csv";
        string testfile="data/"+symbol+"_test.csv";
        get_stock_data(symbol, 1, train_start, train_end, strat, trainfile);
        get_stock_data(symbol, 1, start_date, end_date, strat, testfile);
        lr tool(start_date, end_date, x, p, train_start, train_end);
        tool.run(testfile, cashflow_name, trainfile, order_name, pandl_name);
    }
    else
    {
        string filename="data/"+symbol+".csv";
        get_stock_data(symbol, n, start_date, end_date, strat, filename);
    }

    string infile_name = "data/"+symbol+".csv";
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
        macd tool(start_date, end_date, x);
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
        cout<<tool.run(infile_name, cashflow_name, order_name, pandl_name)<<endl;
        
    }
    else if(strat=="BEST_OF_ALL")
    {
        parallel(start_date,end_date,cashflow_name, order_name, pandl_name, symbol);
    }
    else if(strat=="LINEAR_REGRESSION")
    {
        cout<<"\n";
    }
    else
    {
        cout << "Invalid strategy\n";
    }
}