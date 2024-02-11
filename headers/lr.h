#include "strategy.h"

class lr: public strategy
{
private:
    vector<vector<double>> x_train;
    vector<vector<double>> y_train;
    vector<vector<double>> x_test;
    vector<vector<double>> y_test;
    vector<date_entry> test_entries;
    vector<date_entry> train_entries;
    vector<vector<double>> theta_values;
    vector<vector<double>> predictions_values;
    string train_start_date;
    string train_end_date;
    int p;
    void calculatelr(vector<vector<double>> &x_arr, vector<vector<double>> &y_arr, vector<date_entry> &data_entries);
    void normal_equation(vector<vector<double>> &x_arr, vector<vector<double>> &y_arr); 
    double simulate_trade(string cashflow_file, string order_stats_file, string pandl_file,vector<vector<double>> &x_arr, vector<vector<double>> &y_arr);
public:
    lr(string start, string end, int x_, int p_,int n_ , string cashflow_file, string order_stats_file, string pandl_file,string train_start, string train_end);
    double predict(string train_file,string test_file);
    double predict(string filename) override;
};