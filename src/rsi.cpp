#include "rsi.h"

rsi::rsi(string start, string end, int n_, int x_, double oversold, double overbought)
{
    start_date = start;
    end_date = end;
    n = n_;
    x = x_;
    oversold_threshold = oversold;
    overbought_threshold = overbought;
}

void rsi::write_daily_flow(string date, double cashflow)
{
    string to_write = date + "," + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void rsi::write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

void rsi::calculate_gain_loss()
{
    int num_days = prices.size();
    vector<double> gain(num_days, 0);
    vector<double> loss(num_days, 0);

    for (int i = 1; i < num_days; i++)
    {
        gain[i] = gain[i - 1] + max(prices[i] - prices[i - 1], 0.0);
        loss[i] = loss[i - 1] + max(prices[i - 1] - prices[i], 0.0);
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
        double curr_price = prices[i + n];
        string today = dates[i + n];
        double curr_rsi;
        if (curr_avg_loss == 0)
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

        if (curr_rsi >= overbought_threshold && position < x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_rsi <= oversold_threshold && position > -x)
        {
            cashflow += curr_price;
            position--;
            write_orders(today, "SELL", "1", curr_price);
        }

        write_daily_flow(today, cashflow);
    }

    double square_off = position * prices.back();
    string p_and_l = to_string(square_off + cashflow);
    pandlfile << "Final Profit/Loss: " + p_and_l + "\n";

    return square_off + cashflow;
}

double rsi::run(string infile, string cashflow_file, string order_stats_file, string pandl_file)
{
    ifstream file(infile);
    cashfile.open(cashflow_file);
    statfile.open(order_stats_file);
    pandlfile.open(pandl_file);
    cashfile << "Date,Cashflow\n";
    statfile << "Date,Order_dir,Quantity,Price\n";

    if (!file.is_open() || !statfile.is_open() || !cashfile.is_open() || !pandlfile.is_open())
    {
        cout << "Error: couldn't open" << '\n';
    }

    int line_number = 0;
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string date, price;
        line_number++;
        if (getline(ss, date, ',') && getline(ss, price, ','))
        {
            if (line_number == 1)
            {
                continue;
            }
            prices.push_back(stod(price));
            dates.push_back(date);
        }
    }

    calculate_gain_loss();

    double pl = simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();

    return pl;
}