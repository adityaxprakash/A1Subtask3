#include "strategy.h"


class mrpsl : public strategy
{
    private:
        double mrp_threshold;
        double stop_loss_threshold;
        int pos1=0;
        int pos2=0;
        string order_stats_file1;
        string order_stats_file2;
        set<int> openpositions;

        vector<double> op_zscore={0};
        vector<double> stock1_prices={0};
        vector<double> stock2_prices={0};
        vector<double> spread={0};
        vector<double> rolling_mean={0};
        vector<double> rolling_std={0};
        vector<double> sum={0};
        vector<double> square_sum={0};
        vector<double> z_score={0};
        writer orders_2;
        void write_orders_2( string date, string action, string quantity, double price);
        
        void calculate_mrp();
        double simulate_trades();
    public:
        mrpsl(string start, string end, double x_, int n_, double threshhold,string cashflow_file, string pandl_file,string order_stats1,string order_stats2,double stop_loss_threshhold);
        double predict(string filename) override;  
        double predict(string filename1, string filename2) ;
};