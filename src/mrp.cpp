#include "mrp.h"

mrp::mrp(string start, string end, double x_, int n_, double threshold, string cashflow_file, string pandl_file, string order_stats1, string order_stats2) : strategy(start, end, x_, n_, cashflow_file, order_stats1, pandl_file), orders_2(order_stats2, "Date,Order_dir,Quantity,Price")
{
    mrp_threshold = threshold;
}

void mrp::write_orders_2(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price);
    orders_2.write(to_write);
}

void mrp::store_stock_prices()
{
    for (int i = 1; i < entries.size(); i++)
    {
        if (i <= entries.size() / 2)
            stock1_prices.push_back(entries[i].close);
        else
        {
            stock2_prices.push_back(entries[i].close);
        }
    }
}

void mrp::calculate_mrp()
{

    vector<double> rolling_mean = {0};
    vector<double> rolling_std = {0};
    vector<double> sum = {0};
    vector<double> square_sum = {0};

    for (int i = 1; i < stock1_prices.size(); i++)
    {
        spread.push_back(stock1_prices[i] - stock2_prices[i]);
    }
    for (int i = 1; i < spread.size(); i++)
    {
        if (i <= n)
        {
            rolling_mean.push_back(0);
            rolling_std.push_back(0);
            sum.push_back(sum[i - 1] + spread[i]);
            square_sum.push_back(square_sum[i - 1] + spread[i] * spread[i]);
        }
        else
        {
            sum.push_back(sum[i - 1] + spread[i]);
            square_sum.push_back(square_sum[i - 1] + spread[i] * spread[i]);
            rolling_mean.push_back((sum[i] - sum[i - n]) / n);
            rolling_std.push_back(sqrt((square_sum[i] - square_sum[i - n]) / n - (rolling_mean[i] * rolling_mean[i])));
            z_score.push_back((spread[i] - rolling_mean[i]) / rolling_std[i]);
        }
    }
}

double mrp::simulate_trades()
{
    for (int i = 1; i < spread.size(); i++)
    {
        string today = entries[i].date;
        if (i <= n)
        {
            continue;
        }
        double curr_z_score = z_score[i - n];
        if (curr_z_score > mrp_threshold && position > -x)
        {
            position--;
            cashflow += spread[i];
            write_orders(today, "SELL", "1", stock1_prices[i]);
            write_orders_2(today, "BUY", "1", stock2_prices[i]);
        }
        else if (curr_z_score < -mrp_threshold && position < x)
        {
            position++;
            cashflow -= spread[i];
            write_orders(today, "BUY", "1", stock1_prices[i]);
            write_orders_2(today, "SELL", "1", stock2_prices[i]);
        }
        write_daily_flow(today, cashflow);
    }
    write_pandl(cashflow + position * spread[spread.size() - 1]);
    return cashflow + position * spread[spread.size() - 1];
}

double mrp::predict(string filename, string filename2)
{
    for (auto entry : parser.parse_csv(filename))
    {
        entries.push_back(entry);
    }
    for (auto entry : parser.parse_csv(filename2))
    {
        entries.push_back(entry);
    }
    store_stock_prices();
    calculate_mrp();
    return simulate_trades();
}
double mrp::predict(string filename)
{
    cout<<"Shouldn't be here"<<endl;
    return 0;
}