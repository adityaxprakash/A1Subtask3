#include "csv_parser.h"

vector<date_entry> csv_parser::parse_csv(string filename)
{
    vector<date_entry> entries;
    file.open(filename);
    if(!file.is_open())
    {
        cout << "Error opening file" << endl;
        return {};
    }
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        date_entry temp;
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        temp.date = token;

        getline(ss, token, ',');
        temp.close = stod(token);

        getline(ss, token, ',');
        temp.open = stod(token);

        getline(ss, token, ',');
        temp.high = stod(token);

        getline(ss, token, ',');
        temp.low = stod(token);

        getline(ss, token, ',');
        temp.trades = stod(token);

        getline(ss, token, ',');
        temp.prev_close = stod(token);

        getline(ss, token, ',');
        temp.vwap = stod(token);
        
        entries.push_back(temp);
    }
    return entries;
}