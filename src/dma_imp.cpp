#include "dma_imp.h"

dma_imp::dma_imp(string start, string end, int n_, int x_, int p_, int max_hold, double c1_, double c2_)
{
    start_date = start;
    end_date = end;
    n = n_;
    x = x_;
    p = p_;
    max_hold_days = max_hold;
    c1 = c1_;
    c2 = c2_;
}

void dma_imp::write_daily_flow(string date, double cashflow)
{
    string to_write = date + "," + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void dma_imp::write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

void dma_imp::calculate_ama()
{
    int num_days = prices.size();
    vector<double> change_in_prices(num_days, 0);
    sf.resize(num_days - n, 0);
    ama.resize(num_days - n, 0);

    for (int i = 1; i < num_days; i++)
    {
        change_in_prices[i] = change_in_prices[i - 1] + abs(prices[i] - prices[i - 1]);
    }

    sf[1] = sf0;
    ama[1] = prices[n + 1];

    for (int i = 2; i < num_days - n; i++)
    {
        double denom = change_in_prices[i + n] - change_in_prices[i];
        if (denom == 0)
        {
            sf[i] = sf[i - 1];
        }
        double er = (prices[i + n] - prices[i]) / denom;
        double aux = (2 * er) / (1 + c2);

        sf[i] = sf[i - 1] + c1 * (((aux - 1) / (aux + 1)) - sf[i - 1]);
        // cout<<sf[i]<<endl;
        ama[i] = ama[i - 1] + sf[i] * (prices[i + n] - ama[i - 1]);
    }
}

double dma_imp::simulate_trades()
{
    int sim_period = ama.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_ama = ama[i];
        double change = p / 100.0;
        double curr_price = prices[i + n];
        string today = dates[i + n];
        // cout<<today<<" "<<curr_price<<" "<<curr_ama<<endl;
        int last_bought = i;
        if (!bought_date.empty())
        {
            last_bought = bought_date.front();
        }
        int last_sold = i;
        if (!sold_date.empty())
        {
            last_sold = sold_date.front();
        }

        if (curr_price - curr_ama >= change * curr_ama)
        {
            if (i - last_bought >= n)
            {
                bought_date.pop_front();
                bought_date.push_back(i);
                continue;
            }
            if (position == x)
            {
                continue;
            }
            if (last_sold < i)
            {
                sold_date.pop_front();
            }
            bought_date.push_back(i);
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_ama - curr_price >= change * curr_ama)
        {
            if (i - last_sold >= n)
            {
                sold_date.pop_front();
                sold_date.push_back(i);
                continue;
            }
            if (position == -x)
            {
                continue;
            }
            if (last_bought < i)
            {
                bought_date.pop_front();
            }
            sold_date.push_back(i);
            cashflow += curr_price;
            position--;
            write_orders(today, "SELL", "1", curr_price);
        }
        else
        {
            if (last_bought < last_sold)
            {
                if (position == -x)
                {
                    continue;
                }
                if (last_bought < i)
                {
                    bought_date.pop_front();
                }
                sold_date.push_back(i);
                cashflow += curr_price;
                position--;
                write_orders(today, "SELL", "1", curr_price);
            }
            else if (last_sold < last_bought)
            {
                if (position == x)
                {
                    continue;
                }
                if (last_sold < i)
                {
                    sold_date.pop_front();
                }
                bought_date.push_back(i);
                cashflow -= curr_price;
                position++;
                write_orders(today, "BUY", "1", curr_price);
            }
        }
        write_daily_flow(today, cashflow);
    }

    double square_off = position * prices.back();
    string p_and_l = to_string(square_off + cashflow);
    pandlfile << "Final Profit/Loss: " + p_and_l + "\n";
    return square_off + cashflow;
}

double dma_imp::run(string infile, string cashflow_file, string order_stats_file, string pandl_file)
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

    calculate_ama();

    double pl = simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();

    return pl;
}