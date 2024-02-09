#include <bits/stdc++.h>
using namespace std;

class lr
{
private:
    string start_date;
    string end_date;
    string train_start_date;
    string train_end_date;
    double x;
    double p;
    void read_file(string train_file,vector<vector<double>> &x, vector<vector<double>> &y);    
    double simulate_trade(string cashflow_file, string order_stats_file, string pandl_file);

public:
    lr(string start, string end, double x, double p, string train_start, string train_end);
    double run(string infile, string cashflow_file, string train_file, string order_stats_file, string pandl_file);
};