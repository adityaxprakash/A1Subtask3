#include "adx.h"

adx::adx(string start, string end, int x_, int n_, double threshhold, string cashflow_file, string order_stats_file, string pandl_file) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{
    adx_threshhold = threshhold;
}

void adx::calculate_adx()
{
    int num_days = entries.size();
    vector<double> true_range = {0};
    vector<double> plus_dm = {0};
    vector<double> minus_dm = {0};
    vector<double> atr = {0};
    vector<double> plus_di = {0};
    vector<double> minus_di = {0};
    vector<double> plus_atr = {0};
    vector<double> minus_atr = {0};
    vector<double> dx = {0};

    for (int i = 1; i < num_days; i++)
    {
        double curr_high = entries[i].high;
        double curr_low = entries[i].low;
        double curr_prev_close = entries[i].prev_close;
        double prev_high = entries[i - 1].high;
        double prev_low = entries[i - 1].low;

        true_range.push_back(max(curr_high - curr_low, max(curr_high - curr_prev_close, curr_low - curr_prev_close)));
        plus_dm.push_back(max(0.0, curr_high - prev_high));
        minus_dm.push_back(max(0.0, prev_low - curr_low));
        // cout<<true_range[i]<<" "<<plus_dm[i]<<" "<<minus_dm[i]<<" "<<curr_low<<endl;
    }
    atr.push_back(true_range[n + 1]);
    // cout<<atr[1]<<" ";
    plus_di.push_back(plus_dm[n + 1] / atr[1]);
    minus_di.push_back(minus_dm[n + 1] / atr[1]);
    // cout<<plus_di[1]<<" "<<minus_di[1]<<" ";
    dx.push_back(100 * (plus_di[1] - minus_di[1]) / (plus_di[1] + minus_di[1]));
    adx_arr.push_back(dx[1]);
    // cout<<adx_arr[1]<<" "<<dx[1]<<endl;
    for (int i = 2; i < num_days - n; i++)
    {
        double curr_atr = calculate_ewm(n, atr[i - 1], true_range[i + n]);
        // cout<<curr_atr<<endl;
        atr.push_back(curr_atr);
        plus_di.push_back(calculate_ewm(n, plus_di[i - 1], plus_dm[i + n] / curr_atr));
        minus_di.push_back(calculate_ewm(n, minus_di[i - 1], minus_dm[i + n] / curr_atr));
        dx.push_back(100 * (plus_di[i] - minus_di[i]) / (plus_di[i] + minus_di[i]));
        adx_arr.push_back(calculate_ewm(n, adx_arr[i - 1], dx[i]));
        // cout<<adx_arr[i]<<" "<<dx[i]<<endl;
    }
}

double adx::simulate_trades()
{
    int sim_period = adx_arr.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_adx = adx_arr[i];
        double curr_price = entries[i + n].close;
        string today = entries[i + n].date;

        if (curr_adx > adx_threshhold && position < x)
        {
            cashflow -= curr_price;
            position++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_adx < adx_threshhold && position > -x)
        {
            cashflow += curr_price;
            position--;
            write_orders(today, "SELL", "1", curr_price);
        }

        write_daily_flow(today, cashflow);
    }

    double square_off = position * entries.back().close;
    write_pandl(square_off + cashflow);
    return square_off + cashflow;
}

double adx::predict(string infile)
{
    for (auto entry : parser.parse_csv(infile))
    {
        entries.push_back(entry);
    }
    calculate_adx();
    return simulate_trades();
}