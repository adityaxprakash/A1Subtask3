#include "strategy.h"

class mrp : public strategy
{
    private:
        double mrp_threshold;
        writer orders_2;
        void write_orders_2( string date, string action, string quantity, double price);
        vector<double> stock1_prices={0};
        vector<double> stock2_prices={0};
        vector<double> spread={0};
        vector<double> z_score={0};
        void calculate_mrp();
        double simulate_trades();
        void store_stock_prices();

    public:
        mrp(string start, string end, double x_, int n_, double threshold,string cashflow_file, string pandl_file,string order_stats1,string order_stats2);
        double predict(string filename) override;  
        double predict(string filename1, string filename2) ;
};