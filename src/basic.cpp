#include "basic.h"

basic::basic(string start, string end, int x_, int n_, string cashflow_file, string order_stats_file, string pandl_file) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{
}
void basic::calculate_basic()
{
    int num_days = entries.size();
    increases.resize(num_days, 0);
    decreases.resize(num_days, 0);
    for (int i = 2; i < num_days; i++)
    {
        double price_today = entries[i].close;
        double price_yesterday = entries[i - 1].close;
        if (price_today > price_yesterday)
        {
            increases[i] = increases[i - 1] + 1;
        }
        else
        {
            increases[i] = increases[i - 1];
        }
        if (price_today < price_yesterday)
        {
            decreases[i] = decreases[i - 1] + 1;
        }
        else
        {
            decreases[i] = decreases[i - 1];
        }
    }
}

double basic::simulate_trades()
{
    int num_days = entries.size();

    for (int i = n + 1; i < num_days; i++)
    {
        double curr_price = entries[i].close;
        string today = entries[i].date;

        if (decreases[i] - decreases[i - n] == n && position > -x)
        {
            position--;
            cashflow += curr_price;
            write_orders(today, "SELL", "1", curr_price);
        }
        else if (increases[i] - increases[i - n] == n && position < x)
        {
            position++;
            cashflow -= curr_price;
            write_orders(today, "BUY", "1", curr_price);
        }
        write_daily_flow(today, cashflow);
    }
    double square_off = position * entries[num_days - 1].close;
    string p_and_l = to_string(square_off + cashflow);
    write_pandl(square_off + cashflow);

    return square_off + cashflow;
}

double basic::predict(string filename)
{
    for(auto entry:parser.parse_csv(filename))
    {
        entries.push_back(entry);
    }
    calculate_basic();
    return simulate_trades();
}
