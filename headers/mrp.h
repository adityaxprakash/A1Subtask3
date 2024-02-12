#include "strategy.h"

class mrp : public strategy
{
    private:
        double mrp_threshold;
        string order_stats_file1;
        string order_stats_file2;
        vector<double> adx_arr={0};
        vector<double> stock1_prices={0};
        vector<double> stock2_prices={0};
        vector<double> spread={0};
        vector<double> rolling_mean={0};
        vector<double> rolling_std={0};
        vector<double> sum={0};
        vector<double> square_sum={0};
        vector<double> z_score={0};
        void calculate_mrp();
        double simulate_trades();

    public:
        mrp(string start, string end, double x_, int n_, double threshold,string cashflow_file, string order_stats_file, string pandl_file,string order_stats1,string order_stats2);
        double predict(string filename) override;  
        double predict1(string filename1, string filename2) ;
};