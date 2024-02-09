#include "../headers/basic.h"

basic::basic(string start, string end, int n_, double x_)
{
    this->start_date = start;
    this->end_date = end;
    this->n = n_;
    this->x = x_;
}

void basic::calculate_basic()
{
    int num_days = dates.size();
    int lastinc = 0;   
    int lastdec = 0; 
    double lastday = 0; 
    int netstocks = 0;
    for (int i = 0; i < num_days; i++)
    {
        if (i == 0)
        {
            lastday = prices[i];
        }
        else if (i > 0)
        {
            if (prices[i] > lastday)
            {
                lastinc = i;
            }
            else if (prices[i] < lastday)
            {
                lastdec = i;
            }
            else{
                lastinc = i;
                lastdec = i;
            }
            lastday = prices[i];
        }   
        if (i > n - 1)
        {
            if (i -lastinc >n-1){
                last_n.push_back(-1);
            }
            else if (i -lastdec >n-1){
                last_n.push_back(1);
            }
            else{
                last_n.push_back(0);
            }
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
    int num_days = dates.size();
    cashflow = 0;
    position = 0;
    for (int i = 0; i < num_days; i++)
    {
        if (last_n[i] == -1)
        {
            if (position >-x)
            {
                position -= 1;
                cashflow += prices[i];
                write_orders(dates[i], "SELL", "1", prices[i]);
            }
        }
        else if (last_n[i] == 1)
        {
            if (position < x)
            {
                position += 1;
                cashflow -= prices[i];
                write_orders(dates[i], "BUY", "1", prices[i]);
            }
        }
        write_daily_flow(dates[i], cashflow);
    }
    double square_off = position * prices.back();
    string p_and_l = to_string(square_off + cashflow);
    pandlfile << "Basic" << "," << p_and_l << "\n";
}


void basic::run(string infile, string cashflow_file, string order_stats_file, string pandl_file){
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
    //cout << "Read " << prices[10] << " prices" << '\n';
    calculate_basic();

    simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();
}








