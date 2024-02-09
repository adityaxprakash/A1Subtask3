#include <bits/stdc++.h>
using namespace std;

class dma_imp
{
private:
    string start_date;
    string end_date;
    int n;
    double x;
    int p;
    int max_hold_days;
    double c1;
    double c2;
    vector<double> sf = {0};
    vector<double> ama = {0};
    ofstream cashfile;
    ofstream statfile;
    ofstream pandlfile;
    vector<string> dates = {""};
    vector<double> prices = {0};
    int position = 0;
    double cashflow = 0;

    double sf0 = 0.5;
    deque<int> bought_date;
    deque<int> sold_date;

    void calculate_ama();
    void simulate_trades();
    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);

public:
    dma_imp(string start, string end, int n_, double x_, int p_, int max_hold, double c1_, double c2_);
    void run(string infile, string cashflow_file, string order_stats_file, string pandl_file);
};