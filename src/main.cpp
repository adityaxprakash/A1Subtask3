#include <bits/stdc++.h>
#include <dma.h>
using namespace std;


string get_stock_data(string symbol, int n, string start_date, string end_date)
{
    string filename= "build/"+symbol+".csv";
    string command="python3 src/fetch_data.py "+symbol+" "+start_date+" "+end_date+" "+to_string(n)+" "+filename;
    system(command.c_str());

    return filename;
}

int main(int argc, char *argv[])
{
    string strat = argv[1];
    string symbol = argv[2];
    // cout<<argv[3]<<endl;
    int n = stoi(argv[3]);
    double x = stod(argv[4]);
    string start_date = argv[5];
    string end_date = argv[6];
    double p=stod(argv[7]);

    string infile_name = get_stock_data(symbol,n,start_date,end_date);
    string cashflow_name="build/daily_cashflow.csv";
    string order_name="build/order_statistics.csv";
    string pandl_name="build/final_pnl.txt";
    
    if(strat=="DMA")
    {
        dma tool(start_date,end_date, n, x, p);
     
        tool.run(infile_name,cashflow_name, order_name, pandl_name);
          
    }
    // if (strat == "BASIC")
    // {
    
    // }
    // else if (strat == "rsi")
    // {
    //     cout << "Running RSI strategy for " << symbol << "\n";
    // }
    // else
    // {
    //     cout << "Invalid strategy\n";
    // }
}