#include <fstream>
#include <string>
using namespace std;

class strNode
{
public:
    strNode(string num);
    string num;
    strNode *prev = nullptr;
};

class numStr
{
public:
    ~numStr();
    strNode *begin = nullptr;
    strNode *end = nullptr;

    void push(string num);
};

class numNode
{
public:
    numNode(long long int num);
    long long int num = 0;
    numNode *prev = nullptr;
    numNode *next = nullptr;
};

class bigNum
{
public:
    bigNum();
    ~bigNum();
    void getStrFromFile(ifstream *file);
    void print();
    void print(ofstream *file);
    void multiply(bigNum *num1, bigNum *num2);

protected:
    bool sign = 1;
    numNode *begin = nullptr;
    numNode *end = nullptr;

private:
    numStr str;

    void stringToBigNum();
    void addToBegin(long long int num);
    void push(long long int num);
};