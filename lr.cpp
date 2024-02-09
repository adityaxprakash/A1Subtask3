// #include "lr.h"

// vector<vector<double>> training_data;
// MatrixXd X;
// MatrixXd X_bias;
// VectorXd Y;
// VectorXd theta;
// MatrixXd X_input;
// VectorXd predictions;
// vector<vector<double>> testing;
// MatrixXd testing_data;
// MatrixXd testing_input;
// double alpha;
// int num_iters;
// vector<double> means, std_devs;
// vector<double> actual_prices;
// ofstream cashfile;
// ofstream statfile;
// ofstream pandlfile;

// lr::lr(string start, string end, double x, double p, string train_start, string train_end)
// {
//     this->start_date = start;
//     this->end_date = end;
//     this->x = x;
//     this->p = p;
//     this->train_start_date = train_start;
//     this->train_end_date = train_end;
// }

// void lr::read_file(string train_file)
// {
//     ifstream file(train_file);
//     string line;
//     while (getline(file, line))
//     {
//         vector<double> row;
//         stringstream ss(line);
//         double value;
//         while (ss >> value)
//         {
//             row.push_back(value);
//             if (ss.peek() == ',')
//                 ss.ignore();
//         }
//         training_data.push_back(row);
//     }
//     file.close();

//     // normalise the data and convert
//     for (int i = 0; i < training_data[0].size(); i++)
//     {
//         double mean = 0;
//         for (int j = 0; j < training_data.size(); j++)
//         {
//             mean += training_data[j][i];
//         }
//         mean = mean / training_data.size();
//         means.push_back(mean); // Store the mean

//         double std_dev = 0;
//         for (int j = 0; j < training_data.size(); j++)
//         {
//             std_dev += (training_data[j][i] - mean) * (training_data[j][i] - mean);
//         }
//         std_dev = sqrt(std_dev / training_data.size());
//         std_devs.push_back(std_dev); // Store the standard deviation

//         for (int j = 0; j < training_data.size(); j++)
//         {
//             training_data[j][i] = (training_data[j][i] - mean) / std_dev;
//         }
//     }

//     X = MatrixXd(training_data.size(), training_data[0].size() - 1);
//     Y = VectorXd(training_data.size());
//     for (int i = 0; i < training_data.size(); i++)
//     {
//         for (int j = 0; j < training_data[0].size() - 1; j++)
//         {
//             X(i, j) = training_data[i][j];
//         }

//         Y(i) = training_data[i][training_data[0].size() - 1];
//     }
//     X_bias = MatrixXd::Ones(X.rows(), X.cols() + 1);
//     X_bias.block(0, 1, X.rows(), X.cols()) = X;
//     theta = Eigen::VectorXd::Zero(X_bias.cols());

//     alpha = 0.01;
//     num_iters = 1000;
// }

// double lr::compute_cost(string line)
// {
//     int m = X_bias.rows();
//     Eigen::VectorXd error = X_bias * theta - Y;
//     double cost = error.transpose() * error;
//     cost = cost / (2 * m);
//     return cost;
//     // given the input parameters calculate the expected answer
// }

// // void lr::gradient_descent()
// // {
// //     int m = X_bias.rows();
// //     for (int i = 0; i < 1; i++)
// //     {
// //         Eigen::VectorXd error = X_bias * theta - Y;
// //         Eigen::VectorXd temp = X_bias.transpose() * error;
// //         theta = theta - alpha * temp / m;
// //     }
// // }
// void normal_equation()
// {
//     theta = (X_bias.transpose() * X_bias).inverse() * X_bias.transpose() * Y;
// }

// void lr::simulate_trade(string cashflowfile, string order_stats_file, string pandl_file)   
// {
//     cashfile.open(cashflowfile);
//     statfile.open(order_stats_file);
//     pandlfile.open(pandl_file);
//     // for (int i = 0; i < training_data[0].size(); i++)
//     // {
//     //     double mean = means[i];
//     //     double std_dev = std_devs[i];
//     //     for (int j = 0; j < training_data.size(); j++)
//     //     {
//     //         training_data[j][i] = training_data[j][i] * std_dev + mean;
//     //     }
//     // }
//     predictions = testing_input * theta;
//     for (int i = 0; i < predictions.size(); i++)
//     {
//         predictions[i] = predictions[i] * std_devs[std_devs.size() - 1] + means[means.size() - 1];
//     }

//     int siumation_days = testing.size();

//     double cash = x;

    
// }
// void write_daily_flow(string date, double cashflow)
// {
//     string to_write = date + " " + to_string(cashflow) + "\n";
//     cashfile << to_write;
// }

// void write_orders(string date, string action, string quantity, double price)
// {
//     string to_write = date + "," + action + "," + quantity + "," + to_string(price) + "\n";
//     statfile << to_write;
// }

// void lr::read_test_file(string test_file)
// {
//     ifstream file(test_file);
//     string line;
//     while (getline(file, line))
//     {
//         vector<double> row;
//         stringstream ss(line);
//         double value;
//         while (ss >> value)
//         {
//             row.push_back(value);
//             if (ss.peek() == ',')
//                 ss.ignore();
//         }
//         testing.insert(testing.end(), row.begin(), row.end() - 1);
//         actual_prices.push_back(row[row.size() - 1]);
//         row.pop_back();        
//     }
//     for (int i = 0; i < testing.size(); i++)
//     {
//         testing[i] = (testing[i] - means[means.size() - 1]) / std_devs[std_devs.size() - 1];
//     }
//     file.close();
//     for (int i = 0; i < testing.size(); i++)
//     {
//         testing[i] = (testing[i] - means[means.size() - 1]) / std_devs[std_devs.size() - 1];
//     }
//     testing_data = MatrixXd(testing.size(), training_data[0].size() - 1);
//     testing_input = MatrixXd::Ones(testing_data.rows(), testing_data.cols() + 1);
//     testing_input.block(0, 1, testing_data.rows(), testing_data.cols()) = testing_data;
    
// }



// void lr::run(string infile, string cashflow_file, string train_file, string order_stats_file, string pandl_file, string test_file)
// {

    
//     read_file(train_file);
//     read_test_file(infile);
//     normal_equation();
//     simulate_trade(cashflow_file, order_stats_file, pandl_file);
// }
// int main()
// {
//     lr l("2018-01-01", "2018-12-31", 100000, 0.01, "2018-01-01", "2018-12-31");
//     l.run("infile", "cashflow_file", "SBINtrainee.csv", "order_stats_file", "pandl_file");
//     return 0;
// }