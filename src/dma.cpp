#include "dma.h"

dma::dma(string start, string end, int x_, int n_, int p_, string cashflow_file, string order_stats_file, string pandl_file) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{
    p = p_;
}

void dma::calculate_dma()
{
    int num_days = entries.size();
    vector<double> sum(num_days, 0);
    vector<double> square_sum(num_days, 0);

    for (int i = 1; i < num_days; i++)
    {
        sum[i] = sum[i - 1] + entries[i].close;
        square_sum[i] = square_sum[i - 1] + (entries[i].close * entries[i].close);
    }

    for (int i = n + 1; i < num_days; i++)
    {
        double curr_avg = (sum[i] - sum[i - n]) / n;
        double curr_sd = sqrt(((square_sum[i] - square_sum[i - n]) / n) - (curr_avg * curr_avg));
        dmaverage.push_back(curr_avg);
        sd.push_back(curr_sd);
    }
}

double dma::simulate_trades()
{
    int sim_period = dmaverage.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_dma = dmaverage[i];
        double curr_sd = sd[i];
        double curr_price = entries[i + n].close;
        string today = entries[i + n].date;
        if (curr_price - curr_dma >= p * curr_sd && position < x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_dma - curr_price >= p * curr_sd && position > -x)
        {
            cashflow += curr_price;
            position--;
            write_orders(today, "SELL", "1", curr_price);
        }
        write_daily_flow(today, cashflow);
    }

    double square_off = position * entries.back().close;

    write_pandl(square_off + cashflow);

    return square_off + cashflow;
}

double dma::predict(string filename)
{
    for (auto entry : parser.parse_csv(filename))
    {
        entries.push_back(entry);
    }
    calculate_dma();
    return simulate_trades();
}