#include<bits/stdc++.h>
using namespace std;

class dma
{
    private:
    string start_date;
    string end_date;
    int n;
    double x;
    double p;
    void calculate_dma();
    void simulate_trades();
    
    public:
    dma(string start, string end, int n_, double x_, double p_);
    void run(string infile, string cashflow_file, string order_stats_file, string pandl_file);

};