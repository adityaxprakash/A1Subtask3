#include "csv_parser.h"
#include "writer.h"
#pragma once

class strategy
{
    protected:
    csv_parser parser;
    writer daily_flow;
    writer orders;
    writer pandl;
    string start_date;
    string end_date;
    vector<date_entry> entries;
    int x;
    int n;
    int position = 0;
    double cashflow = 0;

    void write_daily_flow(string date, double cashflow);
    void write_orders(string date, string action, string quantity, double price);
    void write_pandl(double pandl);
    double calculate_ewm(int k, double prev, double curr);

    public:
    strategy(string start, string end, int x_, int n_, string cashflow_file, string order_stats_file, string pandl_file): daily_flow(cashflow_file, "Date,Cashflow"), orders(order_stats_file, "Date,Order_dir,Quantity,Price"), pandl(pandl_file,"NO_HEADER")
    {
        start_date = start;
        end_date = end;
        x = x_;
        n = n_;
        date_entry dummy;
        dummy.date = "";
        dummy.close = 0;
        dummy.open = 0;
        dummy.high = 0;
        dummy.low = 0;
        dummy.trades = 0;
        dummy.prev_close = 0;
        dummy.vwap = 0;
        entries.push_back(dummy);
    }
    virtual double predict(string filename)=0;
};