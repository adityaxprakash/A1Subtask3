#include <bits/stdc++.h>
using namespace std;

class basic
{
private:
    string start_date;
    string end_date;
    int n;
    int x;
    ofstream cashfile;
    ofstream statfile;
    ofstream pandlfile;
    vector<string> dates = {""};
    vector<double> prices = {0};
    vector<int> increases;
    vector<int> decreases;

    
    int position = 0;
    double cashflow = 0;

    void calculate_basic();
    double simulate_trades();
    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);

public:
    basic(string start, string end,  int x_, int n_);
    double run(string infile, string cashflow_file, string order_stats_file, string pandl_file);
};