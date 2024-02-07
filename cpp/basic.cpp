#include <bits/stdc++.h>
#include "../headers/basic.h"

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
        n = n;
        x = x;
        start = start;
        end = end;
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
        string closval = 0;
        int date;
        int netstocks = 0;
        std::ofstream cashfile(cashname);
        std::ofstream statfile(statname);
        cashfile << "Date,Cashflow\n";
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
                if (date_counter == 0)
                {
                    lastday = std::stod(closval);
                }
                if (date_counter > 0)
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
            }
            if (date_counter > n)
            {
                if (date_counter - lastinc > n)
                {
                    if (accumul > -x)
                    {
                        netstocks += 1;
                        accumul += std::stod(closval);
                        statfile << date << ","
                                 << "BUY"
                                 << "," << 1 << "," << closval << "," << accumul << "\n";
                    }
                    cashfile << date << "," << accumul << "\n";
                }
                else if (date_counter - lastdec > n)
                {
                    if (accumul < x)
                    {
                        netstocks -= 1;
                        accumul -= std::stod(closval);
                        statfile << date << ","
                                 << "SELL"
                                 << "," << 1 << "," << closval << "," << accumul << "\n";
                    }
                    cashfile << date << "," << accumul << "\n";
                }
                else
                {
                    cashfile << date << "," << accumul << "\n";
                }
            }
            date_counter++;
        }
    }
};
int main()
{
    basic b(10, 100, "2020-01-01", "2020-12-31");
    b.run("../SBIN.csv", "../cash.csv", "../stat.csv");
    return 0;
}