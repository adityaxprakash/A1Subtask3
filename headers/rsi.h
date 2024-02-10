#include "strategy.h"

class rsi: public strategy
{
private:
    double oversold_threshold;
    double overbought_threshold;
    vector<double> avg_gain={0};
    vector<double> avg_loss={0};

    void calculate_gain_loss();
    double simulate_trades(); 

public:
    rsi(string start, string end,  int x_, int n_,double oversold, double overbought, string cashflow_file, string order_stats_file, string pandl_file);
    double predict(string filename) override;
};