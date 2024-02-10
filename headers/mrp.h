#include <bits/stdc++.h>
using namespace std;


class mrp{
    private:
        string start_date;
        string end_date;
        string stock1;
        string stock2;
        int n;
        double x;
        double mrp_threshold;
        vector<double> adx_arr={0};

        ofstream cashfile;
        ofstream statfile1;
        ofstream statfile2;
        ofstream pandlfile;

        vector<string> dates = {""};
        vector<double> stock1_prices={0};
        vector<double> stock2_prices={0};
        vector<double> spread={0};
        vector<double> rolling_mean={0};
        vector<double> rolling_std={0};
        vector<double> sum={0};
        vector<double> square_sum={0};
        vector<double> z_score={0}; 
        int position = 0;
        double cashflow = 0;

        void calculate_mrp();
        void simulate_trades(string cashflow_file, string order_stats_file1,string order_stats_file2, string pandl_file);
        void write_daily_flow(string date, double cashflow);
        void write_orders(string date, string action, string quantity, double price,string stock);
        void write_orders_1(string date, string action, string quantity, double price, string stock);
        void write_orders_2(string date, string action, string quantity, double price, string stock);
        
    public:
        mrp(string start, string end, double x_, int n_, double ,string stock1_,string stock2_);
        void run(string infile1,string infile2, string cashflow_file, string order_stats_file, string oder_stats2_file,string pandl_file);    
};