#include "macd.h"

macd::macd(string start, string end, double x_, int n_)
{
    start_date = start;
    end_date = end;
    x = x_;
    n = n_;
}

void macd::write_daily_flow(string date, double cashflow)
{
    string to_write = date + " " + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void macd::write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

double calculate_ewm(int k, vector<double> &base, int idx)
{
    double ewm = base[idx - k];
    double alpha = 2 / (k + 1);
    for (int i = k - 1; i >= 1; i--)
    {
        ewm = alpha * base[idx - i] + (1 - alpha) * ewm;
    }
    return ewm;
}

void macd::calculate_macd()
{
    int num_days = prices.size();
    vector<double> short_ewm(num_days, 0);
    vector<double> long_ewm(num_days, 0);
    macd_arr.resize(num_days, 0);
    for (int i = n + 1 - macd_n; i < num_days; i++)
    {
        short_ewm[i] = calculate_ewm(short_n, prices, i);
        long_ewm[i] = calculate_ewm(long_n, prices, i);
        macd_arr[i] = short_ewm[i] - long_ewm[i];
    }
    for (int i = n + 1; i < num_days; i++)
    {
        signal_line.push_back(calculate_ewm(macd_n, macd_arr, i));
    }
}

void macd::simulate_trades()
{
    int sim_period = signal_line.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_signal = signal_line[i];
        double curr_macd = macd_arr[i+n];
        double curr_price = prices[i + n];
        string today = dates[i + n];

        if (curr_macd > curr_signal && position<x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_macd < curr_signal && position>-x)
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
}

void macd::run(string infile, string cashflow_file, string order_stats_file, string pandl_file)
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

    calculate_macd();

    simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();
}