#include <bits/stdc++.h>
using namespace std;

class dma
{
private:
    string start_date;
    string end_date;
    int n;
    double x;
    int p;
    ofstream cashfile;
    ofstream statfile;
    ofstream pandlfile;
    vector<string> dates = {""};
    vector<double> prices = {0};
    vector<double> dmaverage = {0};
    vector<double> sd = {0};
    int position = 0;
    double cashflow = 0;

    void calculate_dma();
    void simulate_trades();
    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);

public:
    dma(string start, string end, int n_, double x_, int p_);
    void run(string infile, string cashflow_file, string order_stats_file, string pandl_file);
};