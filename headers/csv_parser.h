#include <bits/stdc++.h>
using namespace std;
#pragma once

struct date_entry
{
    string date;
    double close;
    double open;
    double high;    
    double low;
    double trades;
    double prev_close;
    double vwap;
};

class csv_parser
{
    private:
    ifstream file;

    public:
    vector<date_entry> parse_csv(string filename);
};