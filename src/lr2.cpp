#include "../headers/lr.h"

vector<double> fr;

vector<vector<double>> training_data;
vector<double> means, std_devs;

vector<vector<double>> x_train;
vector<vector<double>> y_train;


vector<vector<double>> x_test;
vector<vector<double>> y_test;
vector<vector<double>> predictions_values;
vector<vector<double>> theta_values;

double alpha;
int num_iters;

vector<string> dates;



ofstream cashfile;
ofstream statfile;
ofstream pandlfile;

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

vector<vector<double>> inverse(vector<vector<double>> A)
{
    const double EPS = 1E-9;
    int n = A.size();
    std::vector<std::vector<double>> res(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i)
        res[i][i] = 1;

    for (int i = 0; i < n; ++i)
    {
        int maxj = i;
        for (int j = i + 1; j < n; ++j)
            if (abs(A[j][i]) > abs(A[maxj][i]))
                maxj = j;
        if (abs(A[maxj][i]) < EPS)
            throw std::runtime_error("No inverse exists");
        std::swap(A[i], A[maxj]);
        std::swap(res[i], res[maxj]);

        double d = A[i][i];
        for (int j = 0; j < n; ++j)
        {
            A[i][j] /= d;
            res[i][j] /= d;
        }

        for (int j = 0; j < n; ++j)
            if (j != i && abs(A[j][i]) > EPS)
                for (int k = 0; k < n; ++k)
                {
                    A[j][k] -= A[i][k] * A[j][i];
                    res[j][k] -= res[i][k] * A[j][i];
                }
    }
    return res;
}

lr::lr(string start, string end, double x, double p, string train_start, string train_end)
{
    this->start_date = start;
    this->end_date = end;
    this->x = x;
    this->p = p;
    this->train_start_date = train_start;
    this->train_end_date = train_end;
}

void lr::read_file(string train_file, vector<vector<double>> &x, vector<vector<double>> &y)
{
    cout << "Reading file" << endl;
    ifstream file(train_file);
    string line;
    int n = 0;
    int k = 0;
    while (getline(file, line))
    {
        if (k == 0)
        {
            k++;
            continue;
        }
        vector<double> row;
        stringstream ss(line);
        string datee;
        int c = 0;
        while (getline(ss, datee, ','))
        {
            if (c == 0)
            {
                dates.push_back(datee);
                c++;
            }
            else
            {
                row.push_back(stod(datee));
            }
        }
        training_data.push_back(row);
    }
    file.close();

    cout << "Data read" << endl;

    for (int i = 0; i < training_data.size(); i++)
    {
        // training_data[i].push_back(training_data[i][training_data[i].size() - 1]);
        // if (i < training_data.size() - 1)
        //     training_data[i][training_data[i].size() - 1] = training_data[i + 1][training_data[i + 1].size() - 1];
        // else
        //     training_data[i][training_data[i].size() - 2] = 0;
        // double temp = training_data[i].back();
        // training_data[i][training_data[i].size() - 1] = training_data[i][training_data[i].size() - 3];
        // training_data[i][training_data[i].size() - 3] = temp;
        if (i < training_data.size() - 1){
            training_data[i].push_back(training_data[i + 1][training_data[i + 1].size() - 2]);
            training_data[i].push_back(training_data[i + 1][training_data[i + 1].size() - 2]);
        }
        else{
            training_data[i].push_back(0);
            training_data[i].push_back(0);
        }    
    }

    cout << "Data transformed" << endl;
    training_data.erase(training_data.end());

    
    for (int i = 0; i < training_data[0].size(); i++)
    {
        double mean = 0;
        for (int j = 0; j < training_data.size(); j++)
        {
            mean += training_data[j][i];
        }
        mean = mean / training_data.size();
        means.push_back(mean); // Store the mean

        double std_dev = 0;
        for (int j = 0; j < training_data.size(); j++)
        {
            std_dev += (training_data[j][i] - mean) * (training_data[j][i] - mean);
        }
        std_dev = sqrt(std_dev / training_data.size());
        std_devs.push_back(std_dev); // Store the standard deviation

        for (int j = 0; j < training_data.size(); j++)
        {
            training_data[j][i] = (training_data[j][i] - mean) / std_dev;
        }
    }
    cout << "Data normalised" << endl;

    
    for (int i = 0; i < training_data.size(); i++)
    {
        x.push_back(fr);
        x[i].push_back(1);
        for (int j = 0; j < training_data[0].size() - 1; j++)
        {
            
            x[i].push_back(training_data[i][j]);
        }
        y.push_back(fr);
        y[i].push_back(training_data[i][training_data[0].size() - 1]);
        
    }

    cout << "x_train size: " << x.size() << " " << x[0].size() << endl;
    cout << "y_train size: " << y.size() << " " << y[0].size() << endl;
    cout << "Data loaded into matrix" << endl;
    training_data.clear();
}

void normal_equation()
{
    theta_values = matrix_multiplication(inverse(matrix_multiplication(transpose(x_train), x_train)), matrix_multiplication(transpose(x_train), y_train));
    cout << "Theta values: " << theta_values.size() << " " << theta_values[0].size() << endl;
}

void write_daily_flow(string date, double cashflow)
{
    // cout<<date<<" "<<cashflow<<endl;
    // cashfile << "Date, Cashflow\n";
    string to_write = date + "," + to_string(cashflow) + "\n";
    cashfile << to_write;
}

void write_orders(string date, string action, string quantity, double price)
{
    string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
    statfile << to_write;
}

double lr::simulate_trade(string cashflow_file, string order_stats_file, string pandl_file)
{
    ofstream cashfile(cashflow_file);
    ofstream statfile(order_stats_file);
    ofstream pandlfile(pandl_file);
    cashfile << "Date, Cash\n";
    predictions_values = matrix_multiplication(x_test, theta_values);
    cout << "Predictions size: " << predictions_values.size() << " " << predictions_values[0].size() << endl;

    for (int i = 0; i < predictions_values.size(); i++)
    {
        predictions_values[i][0] = predictions_values[i][0] * std_devs[std_devs.size() - 1] + means[means.size() - 1];
    }
    for (int i = 0; i < y_test.size(); i++)
    {
        y_test[i][0] = y_test[i][0] * std_devs[std_devs.size() - 1] + means[means.size() - 1];
    }

    int siumation_days = x_test.size();
    double cashflow = 0;
    int current_position = 0;
    double current_price = 0;
    double current_cash = 0;
    for (int i = 0; i < siumation_days; i++)
    {
        cout<<"Predictions: "<<predictions_values[i][0]<<" "<<y_test[i][0]<<endl;
        if (predictions_values[i][0] > (100 + p) * (y_test[i][0]) / 100)
        {
            if (current_position == x)
            {
                continue;
            }
            else
            {
                current_position++;
                current_cash -= y_test[i][0];
                statfile << dates[i] << ",BUY,1," << y_test[i][0] << endl;
                
            }
        }
       
        else if (predictions_values[i][0] < (100 - p) * (y_test[i][0]) / 100)
        {
            if (current_position == -x)
            {
                continue;
            }
            else
            {
                current_position--;
                current_cash += y_test[i][0];
                statfile << dates[i] << ",SELL,1," << y_test[i][0] << endl;
            }
        }
        cashflow = current_cash;
        string to_write = dates[i] + "," + to_string(cashflow) + "\n";
        cashfile << to_write;
    }
    double final_cash = current_cash + current_position * y_test[siumation_days - 1][0];
    
    return final_cash;
}

double lr::run(string infile, string cashflow_file, string train_file, string order_stats_file, string pandl_file)
{
    cout << "Running" << endl;
    read_file(train_file, x_train, y_train);
    cout << "Training data read" << endl;
    read_file(infile, x_test, y_test);
    cout << "Test data read" << endl;
    normal_equation();
    cout << "Normal equation done" << endl;
    return simulate_trade(cashflow_file, order_stats_file, pandl_file);
    cout << "Simulation done" << endl;
}