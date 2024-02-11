#include "dma.h"
#include "dma_imp.h"
#include "rsi.h"
#include "macd.h"
#include "adx.h"
#include "basic.h"
#include "lr.h"
// #include <omp.h>

string cashflow_name = "data/daily_cashflow.csv";
string order_name = "data/order_statistics.csv";
string pandl_name = "data/final_pnl.txt";

void get_stock_data(string symbol, int n, string start_date, string end_date, string filename)
{
    string command = "python3 src/fetch_data.py " + symbol + " " + start_date + " " + end_date + " " + to_string(n) + " " + filename;
    system(command.c_str());
}

string prev_year(string date)
{
    string year = date.substr(6, 4);
    string day = date.substr(0, 2);
    string month = date.substr(3, 2);
    int y = stoi(year);
    y--;
    if (day == "29" && month == "02")
    {
        return "28/02/" + to_string(y);
    }

    return date.substr(0, 6) + to_string(y);
}

void parallel(string start_date, string end_date, string cashflow_name, string order_name, string pandl_name, string symbol)
{
    int x = 5;
    vector<double> results(7, 0);
    vector<string> names = {"basic", "dma", "dma_imp", "rsi", "macd", "adx", "lr"};
    double max_profit = -1e10;
#pragma omp parallel sections
    {
#pragma omp section
        {
            string infile_name = "data/" + names[0] + ".csv";
            get_stock_data(symbol, 7, start_date, end_date, infile_name);
            basic t1(start_date, end_date, x, 7, cashflow_name, order_name, pandl_name);
            results[0] = t1.predict(infile_name);
            max_profit = max(max_profit, results[0]);
        }

#pragma omp section
        {
            string infile_name = "data/" + names[1] + ".csv";
            get_stock_data(symbol, 50, start_date, end_date, infile_name);
            dma t2(start_date, end_date, x, 50, 2, cashflow_name, order_name, pandl_name);
            results[1] = t2.predict(infile_name);
            max_profit = max(max_profit, results[1]);
        }

#pragma omp section
        {
            string infile_name = "data/" + names[2] + ".csv";
            get_stock_data(symbol, 14, start_date, end_date, infile_name);
            dma_imp t3(start_date, end_date, x, 14, 5, 28, 2, 0.2, cashflow_name, order_name, pandl_name);
            results[2] = t3.predict(infile_name);
            max_profit = max(max_profit, results[2]);
        }
#pragma omp section
        {
            string infile_name = "data/" + names[3] + ".csv";
            get_stock_data(symbol, 14, start_date, end_date, infile_name);
            rsi t4(start_date, end_date, x, 14, 30, 70, cashflow_name, order_name, pandl_name);
            results[3] = t4.predict(infile_name);
            max_profit = max(max_profit, results[3]);
        }

#pragma omp section
        {
            string infile_name = "data/" + names[4] + ".csv";
            get_stock_data(symbol, 0, start_date, end_date, infile_name);
            macd t5(start_date, end_date, x, cashflow_name, order_name, pandl_name);
            results[4] = t5.predict(infile_name);
            max_profit = max(max_profit, results[4]);
        }

#pragma omp section
        {
            string infile_name = "data/" + names[5] + ".csv";
            get_stock_data(symbol, 14, start_date, end_date, infile_name);
            adx t6(start_date, end_date, x, 14, 25, cashflow_name, order_name, pandl_name);
            results[5] = t6.predict(infile_name);
            max_profit = max(max_profit, results[5]);
        }

#pragma omp section
        {
            string trainfile = "data/" + symbol + "_train.csv";
            string testfile = "data/" + symbol + "_test.csv";
            get_stock_data(symbol, 1, prev_year(start_date), prev_year(end_date), trainfile);
            get_stock_data(symbol, 1, start_date, end_date, testfile);
            lr tool(start_date, end_date, x, 2, 1, cashflow_name, order_name, pandl_name, prev_year(start_date), prev_year(end_date));
            results[6] = tool.predict(trainfile, testfile);
            max_profit = max(max_profit, results[6]);
        }
    }
    std::cout << "Results: \n";
    std::cout << "Basic: " << results[0] << "\n";
    std::cout << "DMA: " << results[1] << "\n";
    std::cout << "DMA++: " << results[2] << "\n";
    std::cout << "RSI: " << results[3] << "\n";
    std::cout << "MACD: " << results[4] << "\n";
    std::cout << "ADX: " << results[5] << "\n";
    std::cout << "LR: " << results[6] << "\n";
    if (max_profit == results[0])
    {
        string infile_name = "data/" + symbol + ".csv";
        get_stock_data(symbol, 7, start_date, end_date, infile_name);
        basic t1(start_date, end_date, x, 7, cashflow_name, order_name, pandl_name);
        t1.predict(infile_name);
    }
    else if (max_profit == results[1])
    {
        string infile_name = "data/" + symbol + ".csv";
        get_stock_data(symbol, 50, start_date, end_date, infile_name);
        dma t2(start_date, end_date, x, 50, 2, cashflow_name, order_name, pandl_name);
        t2.predict(infile_name);
    }
    else if (max_profit == results[2])
    {
        string infile_name = "data/" + symbol + ".csv";
        get_stock_data(symbol, 14, start_date, end_date, infile_name);
        dma_imp t3(start_date, end_date, x, 14, 5, 28, 2, 0.2, cashflow_name, order_name, pandl_name);
        t3.predict(infile_name);
    }
    else if (max_profit == results[3])
    {
        string infile_name = "data/" + symbol + ".csv";
        get_stock_data(symbol, 14, start_date, end_date, infile_name);
        rsi t4(start_date, end_date, x, 14, 30, 70, cashflow_name, order_name, pandl_name);
        t4.predict(infile_name);
    }
    else if (max_profit == results[4])
    {
        string infile_name = "data/" + symbol + ".csv";
        get_stock_data(symbol, 0, start_date, end_date, infile_name);
        macd t5(start_date, end_date, x, cashflow_name, order_name, pandl_name);
        t5.predict(infile_name);
    }
    else if (max_profit == results[5])
    {
        string infile_name = "data/" + symbol + ".csv";
        get_stock_data(symbol, 14, start_date, end_date, infile_name);
        adx t6(start_date, end_date, x, 14, 25, cashflow_name, order_name, pandl_name);
        t6.predict(infile_name);
    }
    else if (max_profit == results[6])
    {
        string trainfile = "data/" + symbol + "_train.csv";
        string testfile = "data/" + symbol + "_test.csv";
        get_stock_data(symbol, 1, start_date, end_date, trainfile);
        get_stock_data(symbol, 1, start_date, end_date, testfile);
        lr tool(start_date, end_date, x, 2, 1, cashflow_name, order_name, pandl_name, trainfile, testfile);
        tool.predict(trainfile, testfile);
    }
}

int main(int argc, char *argv[])
{
    string strat = argv[1];
    string symbol = argv[2];
    int n = stoi(argv[3]);
    int x = stoi(argv[4]);
    string start_date = argv[5];
    string end_date = argv[6];
    int p = stoi(argv[7]);
    int max_hold = stoi(argv[8]);
    double c1 = stod(argv[9]);
    double c2 = stod(argv[10]);
    double oversold = stod(argv[11]);
    double overbought = stod(argv[12]);
    double adx_thresh = stod(argv[13]);
    string train_start = argv[14];
    string train_end = argv[15];

    string infile_name = "data/" + symbol + ".csv";
    if (strat == "LINEAR_REGRESSION")
    {
        string trainfile = "data/" + symbol + "_train.csv";
        string testfile = "data/" + symbol + "_test.csv";
        get_stock_data(symbol, 1, train_start, train_end, trainfile);
        get_stock_data(symbol, 1, start_date, end_date, testfile);
        lr tool(start_date, end_date, x, p, 1, cashflow_name, order_name, pandl_name, train_start, train_end);
        tool.predict(trainfile, testfile);
        return 0;
    }

    string filename = "data/" + symbol + ".csv";
    get_stock_data(symbol, n, start_date, end_date, filename);

    if (strat == "BASIC")
    {
        basic tool(start_date, end_date, x, n, cashflow_name, order_name, pandl_name);
        tool.predict(infile_name);
    }
    else if (strat == "DMA")
    {
        dma tool(start_date, end_date, x, n, p, cashflow_name, order_name, pandl_name);
        tool.predict(infile_name);
    }
    else if (strat == "DMA++")
    {
        dma_imp tool(start_date, end_date, x, n, p, max_hold, c1, c2, cashflow_name, order_name, pandl_name);
        tool.predict(infile_name);
    }
    else if (strat == "MACD")
    {
        macd tool(start_date, end_date, x, cashflow_name, order_name, pandl_name);
        tool.predict(infile_name);
    }
    else if (strat == "RSI")
    {
        rsi tool(start_date, end_date, x, n, oversold, overbought, cashflow_name, order_name, pandl_name);
        tool.predict(infile_name);
    }
    else if (strat == "ADX")
    {
        adx tool(start_date, end_date, x, n, adx_thresh, cashflow_name, order_name, pandl_name);
        tool.predict(infile_name);
    }
    else if (strat == "BEST_OF_ALL")
    {
        parallel(start_date, end_date, cashflow_name, order_name, pandl_name, symbol);
    }
    else
    {
        std::cout << "Invalid strategy\n";
    }
}