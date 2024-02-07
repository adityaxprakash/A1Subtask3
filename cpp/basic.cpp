#include <bits/stdc++.h>

using namespace std;

class basic
{
    int n;
    int x;
    string start;
    string end;

public:
    basic(int n, int x, string start, string end)
    {
        this->n = n;
        this->x = x;
        this->start = start;
        this->end = end;
    }
    void run(string filename, string cashname, string statname)
    {
        int date_counter = -1;
        int position = 0;
        std::ifstream file(filename);
        std::string line;
        int lastdec = 0;
        int lastinc = 0;
        double lastday = 0;
        double accumul = 0;
        string closval;
        int date;
        int netstocks = 0;
        std::ofstream cashfile(cashname);
        std::ofstream statfile(statname);
        cashfile << "Date,Cashflow\n";
        statfile << "Date,Action,Quantity,Price,Value\n";
        double profit = 0;
        while (std::getline(file, line))
        {
            if (date_counter == -1)
            {
                date_counter++;
                continue;
            }
            
            std::stringstream ss(line);
            std::string date, closval;
            if (getline(ss, date, ',') && getline(ss, closval, ','))
            {
                // cashfile << date << ","<<closval<<"\n";
                if (date_counter == 0)
                {
                    lastday = std::stod(closval);
                }
                else if (date_counter > 0)
                {
                    if (std::stod(closval) > lastday)
                    {
                        lastinc = date_counter;
                    }
                    if (std::stod(closval) < lastday)
                    {
                        lastdec = date_counter;
                    }
                    lastday = std::stod(closval);
                }

                if (date_counter > n - 1)
                {
                    if (date_counter - lastinc > n-1)
                    {
                        if (accumul + stod(closval) < x)
                        {
                            netstocks -= 1;
                            accumul += std::stod(closval);
                            statfile << date << ","
                                     << "SELL"
                                     << "," << 1 << "," << closval << "\n";
                        }
                    }
                    else if (date_counter - lastdec > n-1)
                    {
                        if (accumul - stod(closval) >-x)
                        {
                            netstocks += 1;
                            accumul -= std::stod(closval);
                            statfile << date << ","
                                     << "BUY"
                                     << "," << 1 << "," << closval << "\n";
                        }
                    }
                    cashfile << date << "," << accumul << "\n";
                    if (file.peek()==EOF){
                        profit = netstocks * std::stod(closval) + accumul;
                    }
                }
            }
            date_counter++;
        }
        ofstream finalfile("final_pl.csv");
        finalfile << "Basic"<<","<<profit<<","<<"\n";
    }
};
