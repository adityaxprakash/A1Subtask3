#include "writer.h"

writer::writer(string filename, string header)
{
    file.open(filename);
    if(!file.is_open())
    {
        cout << "Error opening file" << endl;
        return;
    }
    file << header << endl;
}

writer::~writer()
{
    file.close();
}

void writer::write(string line)
{
    if(!file.is_open())
    {
        cout << "Error writing to file" << endl;
        return;
    }
    file << line << endl;
}