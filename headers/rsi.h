#include <bits/stdc++.h>
using namespace std;

class rsi
{
private:
    string start_date;
    string end_date;
    int n;
    double x;
    double oversold_threshold;
    double overbought_threshold;
    vector<double> avg_gain={0};
    vector<double> avg_loss={0};
    

    ofstream cashfile;
    ofstream statfile;
    ofstream pandlfile;
    vector<string> dates = {""};
    vector<double> prices = {0};
    int position = 0;
    double cashflow = 0;

    void calculate_gain_loss();
    void simulate_trades();
    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);

public:
    rsi(string start, string end, int n_, double x_, double oversold, double overbought);
    void run(string infile, string cashflow_file, string order_stats_file, string pandl_file);
};