#include "strategy.h"

class basic : public strategy
{
private:
    vector<int> increases;
    vector<int> decreases;

    void calculate_basic();
    double simulate_trades();

public:
    basic(string start, string end, int x_, int n_, string cashflow_file, string order_stats_file, string pandl_file);
    double predict(string filename) override;
};