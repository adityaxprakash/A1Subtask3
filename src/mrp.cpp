#include "../headers/mrp.h"

mrp::mrp(string start, string end, double x_, int n_, double threshhold, string stock1_, string stock2_)
{
    start_date = start;
    end_date = end;
    x = x_;
    n = n_;
    mrp_threshold = threshhold;
    stock1 = stock1_;
    stock2 = stock2_;
}

void mrp::calculate_mrp()
{
    for (int i = 1; i <= stock1_prices.size(); i++)
    {
        spread.push_back(stock1_prices[i] - stock2_prices[i]);
    }
    for (int i = 1; i <= spread.size(); i++)
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
            sum.push_back(sum[i - 1] + spread[i] - spread[i - n]);
            square_sum.push_back(square_sum[i - 1] + spread[i] * spread[i] - spread[i - n] * spread[i - n]);
            rolling_mean.push_back((sum[i] - sum[i - n]) / n);
            rolling_std.push_back(sqrt((square_sum[i] - square_sum[i - n]) / n - (rolling_mean[i] * rolling_mean[i])));
            z_score.push_back((spread[i] - rolling_mean[i]) / rolling_std[i]);
        }
    }
}
void mrp::write_daily_flow(string date, double cashflow)
{
    string to_write = date + " " + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void mrp::write_orders(string date, string action, string quantity, double price, string stock)
{
    string to_write = date + "," + action + "," + stock + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

void mrp::simulate_trades(string cashflow_file, string order_stats_file, string pandl_file)
{
    cashfile.open(cashflow_file);
    statfile.open(order_stats_file);
    pandlfile.open(pandl_file);
    for (int i = 1; i <= spread.size(); i++)
    {
        if (i <= n)
        {
            continue;
        }
        double curr_z_score = z_score[i - n];
        if (curr_z_score > mrp_threshold && position > -x)
        {
            position--;
            cashflow += spread[i];
            write_orders(dates[i], "SELL", to_string(1), stock1_prices[i], stock1);
            write_orders(dates[i], "BUY", to_string(1), stock2_prices[i], stock2);
            
        }
        else if (curr_z_score < -mrp_threshold && position < x)
        {
            position++;
            cashflow -= spread[i];
            write_orders(dates[i], "BUY", to_string(1), stock1_prices[i], stock1);
            write_orders(dates[i], "SELL", to_string(1), stock2_prices[i], stock2);
        }
        write_daily_flow(dates[i], cashflow);
    }
    cashfile.close();
    statfile.close();
    pandlfile.close();
}

void mrp::run(string infile1, string infile2, string cashflow_file, string order_stats_file, string pandl_file)
{
    ifstream file1(infile1);
    ifstream file2(infile2);
    string line;
    string date;
    double price1, price2;
    getline(file1, line);
    getline(file2, line);
    int line_number = 0;
    while (getline(file1, line))
    {
        istringstream ss(line);
        line_number += 1;
        if (getline(ss, date, ',') && (ss, price1, ','))
        {
            if (line_number == 1)
            {
                continue;
            }
            stock1_prices.push_back(price1);
            dates.push_back(date);
        }
    }
    line_number = 0;
    while (getline(file2, line))
    {
        istringstream ss(line);
        line_number += 1;
        if (getline(ss, date, ',') && (ss, price2, ','))
        {
            if (line_number == 1)
            {
                continue;
            }
            stock2_prices.push_back(price2);
        }
    }
    calculate_mrp();
    simulate_trades(cashflow_file, order_stats_file, pandl_file);
}