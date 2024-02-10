#include "macd.h"

macd::macd(string start, string end, int x_, string cashflow_file, string order_stats_file, string pandl_file, int n_) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{
        // n = n_;
}


void macd::calculate_macd()
{
    int num_days = entries.size();
    vector<double> short_ewm(num_days, 0);
    vector<double> long_ewm(num_days, 0);
    short_ewm[1] = entries[1].close;
    long_ewm[1] = entries[1].close;
    macd_arr.push_back(0);
    signal_line.push_back(0);
    for (int i = 2; i < num_days; i++)
    {
        short_ewm[i] = calculate_ewm(short_n, short_ewm[i - 1], entries[i].close);
        long_ewm[i] = calculate_ewm(long_n, long_ewm[i - 1], entries[i].close);
        macd_arr.push_back(short_ewm[i] - long_ewm[i]);
    }
    for (int i = 2; i < num_days; i++)
    {
        signal_line.push_back(calculate_ewm(macd_n, signal_line[i - 1], macd_arr[i]));
    }
}

double macd::simulate_trades()
{
    int sim_period = signal_line.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_signal = signal_line[i];
        double curr_macd = macd_arr[i];
        double curr_price = entries[i ].close;
        // cout<<curr_signal<<" "<<curr_macd<<" "<<curr_price<<endl;
        string today = entries[i].date;

        if (curr_macd > curr_signal && position < x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_macd < curr_signal && position > -x)
        {
            cashflow += curr_price;
            position--;
            write_orders(today, "SELL", "1", curr_price);
        }

        write_daily_flow(today, cashflow);
    }

    double square_off = position * entries[sim_period - 1].close;
    string p_and_l = to_string(square_off + cashflow);
    write_pandl(square_off + cashflow);

    return square_off + cashflow;
}

double macd::predict(string filename)
{
    for(auto entry: parser.parse_csv(filename))
    {
        entries.push_back(entry);
    }

    calculate_macd();
    return simulate_trades();
}
