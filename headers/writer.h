#include<bits/stdc++.h>
using namespace std;
#pragma once
class writer
{
    private:
    ofstream file;
   
    public:
    writer(string filename, string header= "");
    ~writer();
    void write(string line);
};