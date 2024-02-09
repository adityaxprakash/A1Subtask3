#include "../headers/basic.h"

basic::basic(string start, string end, double x_, int n_)
{
    this->start_date = start;
    this->end_date = end;
    this->n = n_;
    this->x = x_;
}

void basic::calculate_basic()
{
    int num_days = dates.size();
    increases.resize(num_days, 0);
    decreases.resize(num_days, 0);
    for (int i = 2; i < num_days; i++)
    {
        if (prices[i] > prices[i - 1])
        {
            increases[i] = increases[i - 1] + 1;
        }
        else 
        {
            increases[i] = increases[i - 1];
        }
        if (prices[i] < prices[i - 1])
        {
            decreases[i] = decreases[i - 1] + 1;
        }
        else
        {
            decreases[i] = decreases[i - 1];
        }
    }
}

void basic::write_daily_flow(string date, double cashflow)
{
    string to_write = date + " " + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void basic::write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}
void basic::simulate_trades()
{
    int num_days = increases.size();

    cashflow = 0;
    position = 0;

    for (int i = n + 1; i < num_days; i++)
    {
        double curr_price = prices[i];
        string today = dates[i];

        if (decreases[i] - decreases[i - n] == n && position > -x)
        {
            position -= 1;
            cashflow += curr_price;
            write_orders(today, "SELL", "1", curr_price);
        }
        else if (increases[i] - increases[i - n] == n && position < x)
        {
            position += 1;
            cashflow -= curr_price;
            write_orders(today, "BUY", "1", curr_price);
        }
        write_daily_flow(dates[i], cashflow);
    }
    double square_off = position * prices.back();
    string p_and_l = to_string(square_off + cashflow);
    pandlfile << "Final Profit/Loss: " + p_and_l + "\n";
    
}

void basic::run(string infile, string cashflow_file, string order_stats_file, string pandl_file)
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
    // cout << "Read " << prices[10] << " prices" << '\n';
    calculate_basic();

    simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();
}
