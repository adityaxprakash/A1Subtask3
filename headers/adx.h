#include <bits/stdc++.h>
using namespace std;

class adx
{
private:
    string start_date;
    string end_date;
    int n;
    int x;
    double adx_threshhold;
    vector<double> adx_arr={0};

    ofstream cashfile;
    ofstream statfile;
    ofstream pandlfile;
    vector<string> dates = {""};
    vector<double> high = {0};
    vector<double> low = {0};
    vector<double> prices = {0};
    vector<double> prev_close = {0};

    int position = 0;
    double cashflow = 0;

    void calculate_adx();
    double simulate_trades();
    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);
    double calculate_ewm(int k, double prev, double curr);

public:
    adx(string start, string end, int x_, int n_, double threshhold);
    double run(string infile, string cashflow_file, string order_stats_file, string pandl_file);
};