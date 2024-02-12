#include "rsi.h"

rsi::rsi(string start, string end,  int x_, int n_, double oversold, double overbought, string cashflow_file, string order_stats_file, string pandl_file): strategy(start, end, x_, n_,cashflow_file, order_stats_file, pandl_file)
{
    oversold_threshold = oversold;
    overbought_threshold = overbought;
}


void rsi::calculate_gain_loss()
{
    int num_days = entries.size();
    vector<double> gain(num_days, 0);
    vector<double> loss(num_days, 0);

    for (int i = 1; i < num_days; i++)
    {
        gain[i] = gain[i - 1] + max(entries[i].close - entries[i-1].close, 0.0);
        loss[i] = loss[i - 1] + max(entries[i-1].close - entries[i].close, 0.0);
    }
    for (int i = n + 1; i < num_days; i++)
    {
        avg_gain.push_back((gain[i] - gain[i - n]) / n);
        avg_loss.push_back((loss[i] - loss[i - n]) / n);
    }
}

double rsi::simulate_trades()
{
    int sim_period = avg_gain.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_avg_gain = avg_gain[i];
        double curr_avg_loss = avg_loss[i];
        double curr_price = entries[i + n].close;

        string today = entries[i + n].date;
        double curr_rsi;
        if (abs(curr_avg_loss) < 1e-6)
        {
            curr_rsi = 100;
        }
        else
        {
            double rs = curr_avg_gain / curr_avg_loss;
            curr_rsi = 100 - (100.0 / (1 + rs));
        }

        if (overbought_threshold == oversold_threshold && curr_rsi == overbought_threshold)
        {
            write_daily_flow(today, cashflow);
            continue;
        }

        if (curr_rsi <= oversold_threshold && position <x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_rsi >= overbought_threshold && position >-x)
        {
            cashflow += curr_price;
            position--;
            write_orders(today, "SELL", "1", curr_price);
        }
        // cout<<today<<" "<<curr_avg_gain<<" "<<curr_avg_loss<< " "<<curr_rsi<<endl;

        write_daily_flow(today, cashflow);
    }

    double square_off = position * entries.back().close;
  
    write_pandl(square_off + cashflow);

    return square_off + cashflow;
}

double rsi::predict(string infile)
{
    for(auto entry:parser.parse_csv(infile))
    {
        entries.push_back(entry);
    }
    calculate_gain_loss();
    return simulate_trades();
}