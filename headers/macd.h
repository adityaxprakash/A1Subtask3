#include "strategy.h"

class macd: public strategy
{
private:
    int long_n=26;
    int short_n=12;
    int macd_n=9;
    vector<double> signal_line={0};
    vector<double> macd_arr={0};

    void calculate_macd();
    double simulate_trades();

public:
    macd(string start, string end, int x_, string cashflow_file, string order_stats_file, string pandl_file, int n_=0);
    double predict(string filename) override;
};