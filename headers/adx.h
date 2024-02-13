#include "strategy.h"

class adx: public strategy
{
private:
    double adx_threshhold;
    vector<double> adx_arr={0};
    vector<int> to_skip;
    void calculate_adx();
    double simulate_trades();

public:
    adx(string start, string end, int x_, int n_, double threshhold, string cashflow_file, string order_stats_file, string pandl_file);

    double predict(string filename) override;
};