#include "mrpsl.h"

mrpsl::mrpsl(string start, string end, double x_, int n_, double threshhold, string cashflow_file, string order_stats_file, string pandl_file, string order_stats1, string order_stats2, double stop_loss_threshhold) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{

    mrp_threshold = threshhold;
    order_stats_file1 = order_stats1;
    order_stats_file2 = order_stats2;
    stop_loss_threshold = stop_loss_threshhold;
}

void mrpsl::calculate_mrp()
{
    for (int i = 1; i < entries.size(); i++)
    {
        if (i <= (entries.size()) / 2)
            stock1_prices.push_back(entries[i].close);
        else
        {
            stock2_prices.push_back(entries[i].close);
        }
    }
    for (int i = 1; i < stock1_prices.size(); i++)
    {
        spread.push_back(stock1_prices[i] - stock2_prices[i]);
    }
    for (int i = 1; i < spread.size(); i++)
    {
        if (i <= n)
        {
            rolling_mean.push_back(0);
            rolling_std.push_back(0);
            sum.push_back(sum[i - 1] + spread[i]);
            square_sum.push_back(square_sum[i - 1] + spread[i] * spread[i]);
        }
        else
        {
            sum.push_back(sum[i - 1] + spread[i]);
            square_sum.push_back(square_sum[i - 1] + spread[i] * spread[i]);
            rolling_mean.push_back((sum[i] - sum[i - n]) / n);
            rolling_std.push_back(sqrt((square_sum[i] - square_sum[i - n]) / n - (rolling_mean[i] * rolling_mean[i])));
            z_score.push_back((spread[i] - rolling_mean[i]) / rolling_std[i]);
        }
    }
}

double mrpsl::simulate_trades()
{
    ofstream ofile1(order_stats_file1);
    ofstream ofile2(order_stats_file2);
    for (int i = 1; i < spread.size(); i++)
    {
        if (i <= n)
        {
            continue;
        }
        string today = entries[i].date;
        set<int> to_remove;
        string tday = "";
        double currscore = z_score[i - n];
        int last_position = i;
        if (!openpositions.empty())
            last_position = *openpositions.begin();
        int mode = 0;
        // int delta_pos=0;
        int counter = 0;
        if (openpositions.size() > 0)
        {
            if (z_score[*openpositions.begin()-n] > 0)
            {
                mode = 1;
            }
            else
            {
                mode = -1;
            }
            for (auto ele : openpositions)
            {
                if (abs(spread[i] - rolling_mean[ele]) > stop_loss_threshold * rolling_std[ele])
                {
                    counter += mode;
                    cashflow -= mode*spread[i];
                    to_remove.insert(ele);
                }
            }
            for (auto ele : to_remove)
            {
                openpositions.erase(ele);
            }
            position+=counter;
            counter=0;
        }

        if (currscore > mrp_threshold && currscore<stop_loss_threshold&&  position>-x)
        {
            tday = "SELL";
            // if (mode == 1 && position + to_remove.size() > -x)
            // {
            //     position--;
            // }
            // else if (mode == -1)
            // {
            //     to_remove.insert(openpositions[1]);
            // }
            if(mode==0 || mode==1 || (mode==-1 && openpositions.empty()))
            {
                openpositions.insert(i);
            }
            if(mode==-1 && openpositions.find(last_position)!=openpositions.end())
            {
                openpositions.erase(last_position);
            }   
            
            counter--;
            cashflow+=spread[i];
        }
        else if (currscore < -mrp_threshold &&currscore>-stop_loss_threshold&& position<x)
        {
            tday = "BUY";
            // if (mode == -1 && position - to_remove.size() < x)
            // {
            //     position++;
            // }
            // else if (mode == 1)
            // {
            //     to_remove.insert(openpositions[1]);
            if(mode==0 || mode==-1 || (mode==1 && openpositions.empty()))
            {
                openpositions.insert(i);
            }
            if(mode==1 && openpositions.find(last_position)!=openpositions.end())
            {
                openpositions.erase(last_position);
            }   
            counter++;
            cashflow-=spread[i];
        }
        // for (auto x : to_remove)
        // {
        //     openpositions.erase(remove(openpositions.begin(), openpositions.end(), x), openpositions.end());
        //     if (mode == 1)
        //     {
        //         cashflow += spread[i] - spread[x];
        //         counter++;
        //         position++;
        //     }
        //     else
        //     {
        //         cashflow += spread[x] - spread[i];
        //         counter--;
        //         position--;
        //     }
        // }
        if (counter)
        {
            position += counter;
            tday=counter>0?"BUY":"SELL";
            ofile1 << today << "," << tday << "," << counter << "," << stock1_prices[i] << endl;
            ofile2 << today << "," << (tday == "BUY" ? "SELL" : "BUY") << "," << counter << "," << stock2_prices[i] << endl;
        }
        write_daily_flow(today, cashflow);
    }
    cout<<position<<endl;
    write_pandl(cashflow + position * spread[spread.size() - 1]);
    return cashflow + position * spread[spread.size() - 1];
}

double mrpsl::predict(string filename, string filename2)
{
    for (auto entry : parser.parse_csv(filename))
    {
        entries.push_back(entry);
    }
    for (auto entry : parser.parse_csv(filename2))
    {
        entries.push_back(entry);
    }
    calculate_mrp();
    return simulate_trades();
}
double mrpsl::predict(string filename)
{
    return 0;
}