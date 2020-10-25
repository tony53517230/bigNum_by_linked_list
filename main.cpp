#include <iostream>
#include <fstream>
#include "bigNum.h"

using namespace std;

int main()
{
    ifstream *ifile = new ifstream();
    bigNum num1, num2, result;
    ifile->open("num.txt", ios::in);
    num1.getStrFromFile(ifile);
    num2.getStrFromFile(ifile);
    ifile->close();
    ofstream *ofile = new ofstream();
    ofile->open("num.txt", ios::out | ios::app);
    result.multiply(&num1, &num2);
    cout << "caculate complete";
    result.print(ofile);
    ofile->close();
    return 0;
}
