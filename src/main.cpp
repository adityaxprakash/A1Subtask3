#include "mrp.h"
#include "mrpsl.h"
#include "best_of_all.h"

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
    string symbol1 = argv[16];
    string symbol2 = argv[17];
    double threshold = stod(argv[18]);
    double stop_loss_threshold = stod(argv[19]);

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
    else if (strat == "PAIRS")
    {

        string infile_name1 = "data/" + symbol1 + ".csv";
        string infile_name2 = "data/" + symbol2 + ".csv";

        get_stock_data(symbol1, n, start_date, end_date, infile_name1);
        get_stock_data(symbol2, n, start_date, end_date, infile_name2);
        if (stop_loss_threshold < 0)
        {
            mrp tool(start_date, end_date, x, n, threshold, cashflow_name, pandl_name, order_stat1, order_stat2);
            tool.predict(infile_name1, infile_name2);
        }
        else
        {
            mrpsl tool(start_date, end_date, x, n, threshold, cashflow_name, pandl_name, order_stat1, order_stat2, stop_loss_threshold);
            tool.predict(infile_name1, infile_name2);
        }
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