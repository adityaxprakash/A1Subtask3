#include "strategy.h"

class dma_imp: public strategy
{
private:
    int p;
    int max_hold_days;
    double c1;
    double c2;
    vector<double> sf = {0};
    vector<double> ama = {0};

    double sf0 = 0.5;
    deque<int> bought_date;
    deque<int> sold_date;

    void calculate_ama();
    double simulate_trades();

public:
    dma_imp(string start, string end,  int x_, int n_,int p_, int max_hold, double c1_, double c2_, string cashflow_file, string order_stats_file, string pandl_file);
    double predict(string filename) override;
};