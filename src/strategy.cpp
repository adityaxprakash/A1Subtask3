#include "strategy.h"


void strategy::write_daily_flow(string date, double cashflow)
{
    string to_write = date + "," + to_string(cashflow);
    daily_flow.write(to_write);
}

void strategy::write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price);
    orders.write(to_write);
}

void strategy::write_pandl( double p_and_l)
{
    string to_write = to_string(p_and_l);
    pandl.write(to_write);
}

double strategy::calculate_ewm(int k, double prev, double curr)
{
    double alpha = 2.0 / (k + 1);
    double ewm = curr * alpha + (1 - alpha) * prev;
    return ewm;
}

