#include "dma_imp.h"

dma_imp::dma_imp(string start, string end, int x_, int n_, int p_, int max_hold, double c1_, double c2_, string cashflow_file, string order_stats_file, string pandl_file) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{
    p = p_;
    max_hold_days = max_hold;
    c1 = c1_;
    c2 = c2_;
}

void dma_imp::calculate_ama()
{
    int num_days = entries.size();
    vector<double> change_in_prices(num_days, 0);
    sf.resize(num_days - n, 0);
    ama.resize(num_days - n, 0);

    for (int i = 1; i < num_days; i++)
    {
        change_in_prices[i] = change_in_prices[i - 1] + fabs(entries[i].close - entries[i - 1].close);
    }

    sf[1] = sf0;
    ama[1] = entries[n + 1].close;
    // cout<<entries[n+1].close<<" "<<entries[1+n].date<<" "<<ama[1]<<" "<<sf[1]<<endl;
    for (int i = 2; i < num_days - n; i++)
    {
        double denom = change_in_prices[i + n] - change_in_prices[i];
        if (abs(denom) < 1e-6)
        {
            sf[i] = sf[i - 1];
            ama[i]= ama[i-1];
        }
        else
        {
            double er = (entries[i + n].close - entries[i].close) / denom;
            double aux = (2 * er) / (1 + c2);
            sf[i] = sf[i - 1] + c1 * (((aux - 1) / (aux + 1)) - sf[i - 1]);
            ama[i] = ama[i - 1] + sf[i] * (entries[i + n].close - ama[i - 1]);
        }
        // cout<<entries[i+n].close<<" "<<entries[i+n].date<<" "<<ama[i]<<" "<<sf[i]<<endl;
    }
}

double dma_imp::simulate_trades()
{
    int sim_period = ama.size();

    for (int i = 1; i < sim_period; i++)
    {
        double curr_ama = ama[i];
        double curr_price = entries[i + n].close;
        string today = entries[i + n].date;
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
        // cout<<entries[last_sold+n].date<<" "<<entries[last_bought+n].date<<" "<<today<<endl;
        // cout<<curr_price<<" "<<curr_ama<<" "<<today<<endl;
        if (curr_price - curr_ama >= p*curr_ama/100.0 )
        {
            // cout<<"here_"<<endl;
            if (i - last_bought >= max_hold_days)
            {
                bought_date.pop_front();
                bought_date.push_back(i);
            }
            else if(position<x)
            {
                if (last_sold < i)
                {
                    sold_date.pop_front();
                }
                else 
                {
                    bought_date.push_back(i);
                }
                cashflow -= curr_price;
                position++;
                write_orders(today, "BUY", "1", curr_price);
            }
        }
        else if (curr_ama - curr_price >= p*curr_ama/100.0 )
        {
            // cout<<"here"<<endl;
            if (i - last_sold >= max_hold_days)
            {
                sold_date.pop_front();
                sold_date.push_back(i);
                continue;
            }
            else if(position>-x)
            {
                if (last_bought < i)
                {
                    bought_date.pop_front();
                }
                else 
                {
                    sold_date.push_back(i);
                }
                cashflow += curr_price;
                position--;
                write_orders(today, "SELL", "1", curr_price);
            }
        }
        else
        {
                // cout<<"here2"<<endl;
            if(i-last_bought>=max_hold_days && position>-x)
            {
                bought_date.pop_front();
                // sold_date.push_back(i);
                cashflow += curr_price;
                position--;
                write_orders(today, "SELL", "1", curr_price);
            }
            else if(i-last_sold>=max_hold_days && position<x)
            {
                sold_date.pop_front();
                // bought_date.push_back(i);
                cashflow -= curr_price;
                position++;
                write_orders(today, "BUY", "1", curr_price);
            }
            // if (last_bought < last_sold)
            // {
            //     if (position == -x)
            //     {
            //         continue;
            //     }
            //     if (last_bought < i)
            //     {
            //         bought_date.pop_front();
            //     }
            //     sold_date.push_back(i);
            //     cashflow += curr_price;
            //     position--;
            //     write_orders(today, "SELL", "1", curr_price);
            // }
            // else if (last_sold < last_bought)
            // {
            //     if (position == x)
            //     {
            //         continue;
            //     }
            //     if (last_sold < i)
            //     {
            //         sold_date.pop_front();
            //     }
            //     bought_date.push_back(i);
            //     cashflow -= curr_price;
            //     position++;
            //     write_orders(today, "BUY", "1", curr_price);
            // }
        }
        write_daily_flow(today, cashflow);
    }

    double square_off = position * entries.back().close;

    write_pandl(square_off + cashflow);
    return square_off + cashflow;
}

double dma_imp::predict(string infile)
{
    for (auto entry : parser.parse_csv(infile))
    {
        entries.push_back(entry);
    }
    calculate_ama();
    return simulate_trades();
}
