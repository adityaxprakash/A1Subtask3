#include "adx.h"

adx::adx(string start, string end, int x_, int n_, double threshhold)
{
    start_date = start;
    end_date = end;
    x = x_;
    n = n_;
    adx_threshhold = threshhold;
}

void adx::write_daily_flow(string date, double cashflow)
{
    string to_write = date + "," + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void adx::write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

double adx::calculate_ewm(int k, double prev, double curr)
{
    double alpha = 2.0 / (k + 1);
    double ewm = curr * alpha + (1 - alpha) * prev;
    return ewm;
}

void adx::calculate_adx()
{
    int num_days = prices.size();
    vector<double>true_range={0};
    vector<double>plus_dm={0};
    vector<double>minus_dm={0};
    vector<double> atr={0};
    vector<double>plus_di={0};
    vector<double>minus_di={0};
    vector<double> plus_atr = {0};
    vector<double> minus_atr = {0};
    vector<double> dx={0};
    for(int i=1;i<num_days;i++)
    {
        true_range.push_back(max(high[i]-low[i],max(high[i]-prev_close[i],low[i]-prev_close[i])));
        plus_dm.push_back(max(0.0,high[i]-high[i-1]));
        minus_dm.push_back(max(0.0,low[i-1]-low[i]));
    }
    atr.push_back(true_range[n+1]);
    // cout<<atr[1]<<" ";
    plus_di.push_back(plus_dm[n+1]/atr[1]);
    minus_di.push_back(minus_dm[n+1]/atr[1]);
    // cout<<plus_di[1]<<" "<<minus_di[1]<<" ";
    dx.push_back(100*(plus_di[1]-minus_di[1])/(plus_di[1]+minus_di[1]));
    adx_arr.push_back(dx[1]);
    // cout<<adx_arr[1]<<" "<<dx[1]<<endl;
    for(int i=2;i<num_days-n;i++)
    {
        double curr_atr=calculate_ewm(n,atr[i-1],true_range[i+n]);
        // cout<<curr_atr<<endl;
        atr.push_back(curr_atr);
        plus_di.push_back(calculate_ewm(n,plus_di[i-1],plus_dm[i+n]/curr_atr));
        minus_di.push_back(calculate_ewm(n,minus_di[i-1],minus_dm[i+n]/curr_atr));
        dx.push_back(100*(plus_di[i]-minus_di[i])/(plus_di[i]+minus_di[i]));
        adx_arr.push_back(calculate_ewm(n,adx_arr[i-1],dx[i]));
        // cout<<adx_arr[i]<<" "<<dx[i]<<endl;
    }
}

double adx::simulate_trades()
{
    int sim_period = adx_arr.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_adx = adx_arr[i];
        double curr_price = prices[i + n];
        string today = dates[i + n];

        if (curr_adx > adx_threshhold && position < x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_adx <adx_threshhold && position > -x)
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

double adx::run(string infile, string cashflow_file, string order_stats_file, string pandl_file)
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
        string date, highp, lowp, prev_closep,price;
        line_number++;
        if (getline(ss, date, ',')&& getline(ss, price, ',') && getline(ss, highp, ',') && getline(ss, lowp, ',') && getline(ss, prev_closep, ','))
        {
            if (line_number == 1)
            {
                continue;
            }
            high.push_back(stod(highp));
            low.push_back(stod(lowp));
            prices.push_back(stod(price));
            prev_close.push_back(stod(prev_closep));
            dates.push_back(date);
        }
    }

    calculate_adx();

    double pl=simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();

    return pl;
}