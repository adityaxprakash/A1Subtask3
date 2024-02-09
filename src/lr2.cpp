#include "../headers/lr.h"

vector<vector<double>> training_data;
vector<double> means, std_devs;

MatrixXd X_train;
VectorXd Y_train;
VectorXd theta;
double alpha;
int num_iters;

vector<string> dates;

MatrixXd X_test;
VectorXd Y_test;

VectorXd predictions;

ofstream cashfile;
ofstream statfile;
ofstream pandlfile;

lr::lr(string start, string end, double x, double p, string train_start, string train_end)
{
    this->start_date = start;
    this->end_date = end;
    this->x = x;
    this->p = p;
    this->train_start_date = train_start;
    this->train_end_date = train_end;
}

void lr::read_file(string train_file, MatrixXd &X, VectorXd &Y)
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
        // if (ss>>datee){
        //     dates.push_back(datee);
        //     continue;
        // }
        // row.push_back(1);
        // while (ss >> value)
        // {
        //     row.push_back(value);
        //     if (ss.peek() == ',')
        //         ss.ignore();
        // }

        // training_data.push_back(row);
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
    training_data.erase(training_data.begin());
    cout << "File read" << endl;
    
    int wow = 0;
    cout << training_data.size() << endl;
    cout << training_data[0].size() << endl;
    for (int i = 0; i < training_data.size(); i++)
    {
        if (training_data[i].size() != 6)
            cout << training_data[i].size() << " " << i << endl;
        wow++;
    }

    cout << "Data read" << endl;
    for (int i = 0; i < training_data.size(); i++)
    {
        training_data[i].push_back(training_data[i][training_data[i].size() - 1]);
        if (i < training_data.size() - 1)
            training_data[i][training_data[i].size() - 1] = training_data[i + 1][training_data[i + 1].size() - 1];
        else
            training_data[i][training_data[i].size() - 2] = 0;
        double temp = training_data[i].back();
        training_data[i][training_data[i].size() - 1] = training_data[i][training_data[i].size() - 3];
        training_data[i][training_data[i].size() - 3] = temp;
    }
    cout << "Data transformed" << endl;
    training_data.erase(training_data.end());

    // normalise the data and convert
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
    // loading into matrix
    X = MatrixXd(training_data.size(), training_data[0].size() - 1);
    Y = VectorXd(training_data.size());
    for (int i = 0; i < training_data.size(); i++)
    {
        for (int j = 0; j < training_data[0].size() - 1; j++)
        {
            X(i, j) = training_data[i][j];
        }

        Y(i) = training_data[i][training_data[0].size() - 1];
    }
    cout << "Data loaded into matrix" << endl;
    training_data.clear();
}

void normal_equation()
{
    theta = Eigen::VectorXd::Zero(X_train.cols());
    theta = (X_train.transpose() * X_train).inverse() * X_train.transpose() * Y_train;
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

void lr::simulate_trade(string cashflow_file, string order_stats_file, string pandl_file)
{
    ofstream cashfile(cashflow_file);
    ofstream statfile(order_stats_file);
    ofstream pandlfile(pandl_file);
    cashfile << "Date, Cash\n";
    predictions = X_test * theta;
    for (int i = 0; i < predictions.size(); i++)
    {
        predictions[i] = predictions[i] * std_devs[std_devs.size() - 1] + means[means.size() - 1];
    }

    int siumation_days = X_test.rows();
    double cashflow = 0;
    int current_position = 0;
    double current_price = 0;
    double current_cash = 0;
    for (int i = 0; i < siumation_days; i++)
    {
        current_price = Y_test(i);
        if (predictions(i) > (100 + p) * (Y_test(i)) / 100)
        {
            if (current_position == x)
            {
                continue;
            }
            else
            {
                current_position++;
                current_cash += Y_test(i);
                write_orders(dates[i], "BUY", "1", Y_test(i));
            }
        }
        else if (predictions(i) < (100 - p) * (Y_test(i)) / 100)
        {
            if (current_position == -x)
            {
                continue;
            }
            else
            {
                current_position--;
                current_cash -= Y_test(i);
                write_orders(dates[i], "SELL", "1", Y_test(i));
            }
        }
        cashflow = -current_cash;
        // string to_write = dates[i] + "," + to_string(cashflow) + "\n";
        // cashfile << to_write;
        cout<<dates[i]<<" "<<cashflow<<endl;
        write_daily_flow(dates[i], cashflow);
    }
    double final_cash = -current_cash + current_position * current_price;
    string pandl = to_string(final_cash);
    pandlfile << pandl + "\n"
              << endl;
}

void lr::run(string infile, string cashflow_file, string train_file, string order_stats_file, string pandl_file)
{
    cout << "Running" << endl;
    read_file(train_file, X_train, Y_train);
    cout << "Training data read" << endl;
    read_file(infile, X_test, Y_test);
    cout << "Test data read" << endl;
    alpha = 0.01;
    num_iters = 1000;

    normal_equation();
    cout << "Normal equation done" << endl;
    simulate_trade(cashflow_file, order_stats_file, pandl_file);
    cout << "Simulation done" << endl;
}

// input format for the training file
//  date, some 7 value, closing price
// input format for the test file is same as the one for the training file

int main()
{
    lr l("2019-01-01", "2019-12-31", 1000, 1.0, "2019-01-01", "2019-12-31");
    l.run("test.csv", "ca.csv", "train.csv", "order_stats.csv", "pandl.csv");
    return 0;
}