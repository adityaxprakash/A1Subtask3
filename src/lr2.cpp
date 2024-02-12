#include "lr.h"

void swapRows(vector<vector<double>> &matrix, int i, int j)
{
    vector<double> temp = matrix[i];
    matrix[i] = matrix[j];
    matrix[j] = temp;
}

vector<vector<double>> matrix_multiplication(vector<vector<double>> A, vector<vector<double>> B)
{
    vector<vector<double>> result;
    for (int i = 0; i < A.size(); i++)
    {
        vector<double> row;
        for (int j = 0; j < B[0].size(); j++)
        {
            double sum = 0;
            for (int k = 0; k < A[0].size(); k++)
            {
                sum += A[i][k] * B[k][j];
            }
            row.push_back(sum);
        }
        result.push_back(row);
    }
    return result;
}
vector<vector<double>> transpose(vector<vector<double>> A)
{
    vector<vector<double>> result;
    for (int i = 0; i < A[0].size(); i++)
    {
        vector<double> row;
        for (int j = 0; j < A.size(); j++)
        {
            row.push_back(A[j][i]);
        }
        result.push_back(row);
    }
    return result;
}
vector<vector<double>> inverse(vector<vector<double>> &matrix)
{
    int n = matrix.size();

    vector<vector<double>> inverse(n, vector<double>(n, 0));
    // Initialize the identity matrix for inverse
    for (int i = 0; i < n; ++i)
        inverse[i][i] = 1;

    for (int i = 0; i < n; ++i)
    {
        // Find pivot
        int pivot = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (abs(matrix[j][i]) > abs(matrix[pivot][i]))
                pivot = j;
        }

        // Swap pivot row with current row
        swapRows(matrix, i, pivot);
        swapRows(inverse, i, pivot);

        // Make diagonal elements of the current row to 1
        double divisor = matrix[i][i];
        for (int j = 0; j < n; ++j)
        {
            matrix[i][j] /= divisor;
            inverse[i][j] /= divisor;
        }

        // Make other elements of the current column to 0
        for (int j = 0; j < n; ++j)
        {
            if (j != i)
            {
                double multiplier = matrix[j][i];
                for (int k = 0; k < n; ++k)
                {
                    matrix[j][k] -= multiplier * matrix[i][k];
                    inverse[j][k] -= multiplier * inverse[i][k];
                }
            }
        }
    }

    return inverse;
}

void lr::normal_equation(vector<vector<double>> &x_arr, vector<vector<double>> &y_arr)
{
    vector<vector<double>> a = matrix_multiplication(transpose(x_arr), x_arr);
    theta_values = matrix_multiplication(inverse(a), matrix_multiplication(transpose(x_arr), y_arr));
    for(auto weight: theta_values)
    {
        for(auto w: weight)
        {
            cout<<w<<" ";
        }
        cout<<endl;
    
    }
}

void lr::calculatelr(vector<vector<double>> &x_arr, vector<vector<double>> &y_arr, vector<date_entry> &data_entries)
{
    // cout<<"Called"<<" "<<entries.size()<<endl;
    for (int i = 0; i < data_entries.size() - 1; i++)
    {
        vector<double> temp;
        // cout<<entries[i].high<<" "<<endl;
        temp.push_back(1);
        temp.push_back(data_entries[i].high);
        temp.push_back(data_entries[i].low);
        temp.push_back(data_entries[i].open);
        temp.push_back(data_entries[i].close);
        temp.push_back(data_entries[i].vwap);
        temp.push_back(data_entries[i].trades);
        temp.push_back(data_entries[i + 1].open);
        x_arr.push_back(temp);
        y_arr.push_back({data_entries[i + 1].close});
    }
}
double lr::simulate_trade(string cashflow_file, string order_stats_file, string pandl_file, vector<vector<double>> &x_arr, vector<vector<double>> &y_arr)
{
    predictions_values = matrix_multiplication(x_arr, theta_values);
    // cout << "Predictions size: " << predictions_values.size() << " " << predictions_values[0].size() << endl;

    int sim_days = x_arr.size();
    for (int i = 0; i < sim_days; i++)
    {
        string today = test_entries[i+1].date;
        // cout << "Predictions: " << predictions_values[i][0] << " " << y_arr[i][0] << endl;
        if (predictions_values[i][0] - y_arr[i][0] >= p * (y_arr[i][0]) / 100.0 && position < x)
        {
            position++;
            cashflow -= y_arr[i][0];
            write_orders(today, "BUY", "1", y_arr[i][0]);
        }

        else if (y_arr[i][0] - predictions_values[i][0] >= p * y_arr[i][0] / 100.0 && position > -x)
        {
            position--;
            cashflow += y_arr[i][0];
            write_orders(today, "SELL", "1", y_arr[i][0]);
        }
        write_daily_flow(today, cashflow);
    }

    double final_cash = cashflow + position * y_arr[sim_days - 1][0];
    write_pandl(final_cash);
    return final_cash;
}

lr::lr(string start, string end, int x_, int p_, int n_, string cashflow_file, string order_stats_file, string pandl_file, string train_start, string train_end) : strategy(start, end, x_, n_, cashflow_file, order_stats_file, pandl_file)
{
    p = p_;
    train_start_date = train_start;
    train_end_date = train_end;
}

double lr::predict(string train_file, string test_file)
{
    for (auto entry : parser.parse_csv(train_file))
    {
        train_entries.push_back(entry);
    }
    calculatelr(x_train, y_train, train_entries);
    normal_equation(x_train, y_train);

    for (auto entry : parser.parse_csv(test_file))
    {
        test_entries.push_back(entry);
    }
    calculatelr(x_test, y_test, test_entries);
    return simulate_trade("cashflow.csv", "order_stats.csv", "pandl.csv", x_test, y_test);
}

double lr::predict(string filename)
{
    cout << "Shouldn't be here" << endl;
    return 1.0;
}