#include <bits/stdc++.h>
using namespace std;

class macd
{
private:
    string start_date;
    string end_date;
    int long_n=26;
    int short_n=12;
    int macd_n=9;
    int n;
    double x;
    vector<double> signal_line={0};
    vector<double> macd_arr;

    ofstream cashfile;
    ofstream statfile;
    ofstream pandlfile;
    vector<string> dates = {""};
    vector<double> prices = {0};
    int position = 0;
    double cashflow = 0;

    void calculate_macd();
    void simulate_trades();
    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);

public:
    macd(string start, string end, double x_, int n_);
    void run(string infile, string cashflow_file, string order_stats_file, string pandl_file);
};