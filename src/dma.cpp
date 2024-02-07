#include <dma.h>

ofstream cashfile;
ofstream statfile;
ofstream pandlfile;

vector<string> dates = {""};
vector<double> prices = {0};
vector<double> dmaverage = {0};
vector<double> sd = {0};
int curr_stocks = 0;
int curr_position = 0;

dma::dma(string start, string end, int n_, double x_, double p_)
{
    start_date = start;
    end_date = end;
    n = n_;
    x = x_;
    p = p_;
}

void write_daily_flow(string date, double cashflow)
{
    string to_write = date + " " + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

void dma::calculate_dma()
{
    int num_days = prices.size();
    vector<double> sum(num_days, 0);
    vector<double> square_sum(num_days, 0);

    for (int i = 1; i < num_days; i++)
    {
        sum[i] = sum[i - 1] + prices[i];
        square_sum[i] = square_sum[i - 1] + (prices[i] * prices[i]);
    }

    for (int i = n+1; i < num_days; i++)
    {
        double curr_avg = (sum[i-1] - sum[i - n-1]) / n;
        double curr_sd = sqrt(((square_sum[i-1] - square_sum[i - n-1]) / n) - (curr_avg * curr_avg));
        dmaverage.push_back(curr_avg);
        sd.push_back(curr_sd);
    }
}

void dma::simulate_trades()
{
    int sim_period = dmaverage.size();

    for (int i = 1; i < sim_period; i++)
    {
        double cashflow = 0;
        double curr_dma = dmaverage[i];
        double curr_sd = sd[i];
        double curr_price = prices[i + n];
        string today = dates[i + n];

        if (curr_price - curr_dma >= p * curr_sd)
        {

            if (curr_position + curr_price > x)
            {
                continue;
            }
            curr_position += curr_price;
            curr_stocks++;
            write_orders(today, "BUY", "1", curr_price);
        }
        else if (curr_dma - curr_price >= p * curr_sd)
        {
            if (curr_position - curr_price < -x)
            {
                continue;
            }
            curr_position -= curr_price;
            curr_stocks--;
            write_orders(today, "SELL", "1", curr_price);
        }
        cashflow = -curr_position;
        write_daily_flow(today, cashflow);
    }

    double square_off = curr_stocks * prices.back();
    string p_and_l = to_string(square_off - curr_position);
    pandlfile << "Final Profit/Loss: " + p_and_l + "\n";
}

void dma::run(string infile, string cashflow_file, string order_stats_file, string pandl_file)
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

    calculate_dma();

    simulate_trades();

    file.close();
    statfile.close();
    cashfile.close();
    pandlfile.close();
}