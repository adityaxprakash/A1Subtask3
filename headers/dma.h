#include "strategy.h"

class dma: public strategy
{
private:
    int p;
    vector<double> dmaverage = {0};
    vector<double> sd = {0};
    void calculate_dma();
    double simulate_trades();

public:
    dma(string start, string end, int x_, int n_, int p_, string cashflow_file, string order_stats_file, string pandl_file);
    double predict(string filename) override;
};