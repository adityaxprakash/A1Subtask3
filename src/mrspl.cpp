#include "mrpsl.h"

mrpsl::mrpsl(string start, string end, double x_, int n_, double threshhold, string cashflow_file, string pandl_file, string order_stats1, string order_stats2, double stop_loss_threshhold) : strategy(start, end, x_, n_, cashflow_file, order_stats1, pandl_file), orders_2(order_stats2, "Date,Order_dir,Quantity,Price")
{

    mrp_threshold = threshhold;
    stop_loss_threshold = stop_loss_threshhold;
}

void mrpsl::write_orders_2(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price);
    orders_2.write(to_write);
}

void mrpsl::calculate_mrp()
{

    vector<double> sum = {0};
    vector<double> square_sum = {0};
    for (int i = 1; i < entries.size(); i++)
    {
        if (i <= (entries.size()) / 2)
            stock1_prices.push_back(entries[i].close);
        else
        {
            stock2_prices.push_back(entries[i].close);
        }
    }
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

double mrpsl::simulate_trades()
{
    for (int i = 1; i < spread.size(); i++)
    {
        if (i <= n)
        {
            continue;
        }

        string today = entries[i].date;
        set<int> to_remove;
        double currscore = z_score[i - n];

        int last_position = i;
        if (!openpositions.empty())
            last_position = *openpositions.begin();

        int mode = 0;
        int counter = 0;

        if (position != 0)
        {
            if (position < 0)
            {
                mode = 1;
            }
            else
            {
                mode = -1;
            }
            for (auto ele : openpositions)
            {
                if (abs(spread[i] - rolling_mean[ele]) > stop_loss_threshold * rolling_std[ele])
                {
                    counter += mode;
                    cashflow -= mode * spread[i];
                    to_remove.insert(ele);
                }
            }
            for (auto ele : to_remove)
            {
                openpositions.erase(ele);
            }
            position += counter;
        }

        if (currscore > mrp_threshold && position > -x)
        {
            if (position <= 0)
            {
                openpositions.insert(i);
            }
            if (position > 0 && openpositions.find(last_position) != openpositions.end())
            {
                openpositions.erase(last_position);
            }

            position--;
            counter--;
            cashflow += spread[i];
        }
        else if (currscore < -mrp_threshold && position < x)
        {
            if (position >= 0)
            {
                openpositions.insert(i);
            }
            if (position < 0 && openpositions.find(last_position) != openpositions.end())
            {
                openpositions.erase(last_position);
            }
            position++;
            counter++;
            cashflow -= spread[i];
        }
        if (counter > 0)
        {
            write_orders(today, "BUY", to_string(counter), stock1_prices[i]);
            write_orders_2(today, "SELL", to_string(counter), stock2_prices[i]);
        }
        else if (counter < 0)
        {
            write_orders(today, "SELL", to_string(-counter), stock1_prices[i]);
            write_orders_2(today, "BUY", to_string(-counter), stock2_prices[i]);
        }
        write_daily_flow(today, cashflow);
    }
    write_pandl(cashflow + position * spread[spread.size() - 1]);
    return cashflow + position * spread[spread.size() - 1];
}

double mrpsl::predict(string filename, string filename2)
{
    for (auto entry : parser.parse_csv(filename))
    {
        entries.push_back(entry);
    }
    for (auto entry : parser.parse_csv(filename2))
    {
        entries.push_back(entry);
    }
    calculate_mrp();
    return simulate_trades();
}
double mrpsl::predict(string filename)
{
    return 0;
}