#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "bigNum.h"
#define BASE 1000000000
#define BASE_LENGTH 9
#define MAX_STRING_LENGTH 10
using namespace std;

strNode::strNode(string num)
{
    this->num = num;
}

numStr::~numStr()//將所有數字字串節點刪除
{
    strNode *node = this->end;
    strNode *waitToFree = nullptr;
    while (node != nullptr)
    {
        waitToFree = node;
        node = node->prev;
        delete waitToFree;
        waitToFree = nullptr;
    }
}

void numStr::push(string num)//將num塞入串列尾
{
    strNode *newNode = new strNode(num);
    if (this->end == nullptr && this->begin == nullptr)
    {
        this->begin = newNode;
        this->end = newNode;
    }
    else
    {
        newNode->prev = this->end;
        this->end = newNode;
    }
    return;
}

numNode::numNode(long long int num)
{
    this->num = num;
}

bigNum::bigNum()
{
    numNode *newNode = new numNode(0LL);
    this->begin = newNode;
    this->end = newNode;
}

bigNum::~bigNum()//將所有數組節點刪除
{
    numNode *node = this->begin;
    numNode *waitToFree = nullptr;
    while (node != nullptr)
    {
        waitToFree = node;
        node = node->next;
        delete waitToFree;
        waitToFree = nullptr;
    }
}

void bigNum::push(long long int num)//將數組塞入串列尾
{
    numNode *newNode = new numNode(num);

    if (this->end == nullptr && this->begin == nullptr)
    {
        this->begin = newNode;
        this->end = newNode;
    }
    else
    {
        this->end->next = newNode;
        newNode->prev = this->end;
        this->end = newNode;
    }
    return;
}

void bigNum::addToBegin(long long int num)//將數組塞入串列頭
{
    numNode *newNode = new numNode(num);
    if (this->end == nullptr && this->begin == nullptr)
    {
        this->begin = newNode;
        this->end = newNode;
    }
    else
    {
        this->begin->prev = newNode;
        newNode->next = this->begin;
        this->begin = newNode;
    }
    return;
}

void bigNum::stringToBigNum()//將數字字串串列轉為數組陣列
{
    numNode *node = this->end;
    strNode *str_node = this->str.end;
    int i;
    for (i = 0; str_node->num.length() > 0 && str_node != nullptr; i++)//自字串串列尾部拜訪
    {
        if (node == nullptr)//若數組串列拜訪完，添加新節點至頭
        {
            this->addToBegin(0);
            node = this->begin;
        }
        if (str_node->prev != nullptr && str_node->num.length() <= BASE_LENGTH)//若數字字串未拜訪完且剩餘該字串節點之字元小於BASE_LENGTH位
        {
            str_node->prev->num += str_node->num;//將此節點之字元接入上一個節點字串中
            str_node = str_node->prev;//拜訪上一節點
        }
        if (str_node->num.length() <= BASE_LENGTH)//若字串串列拜訪至頭，且剩餘小於BASE_LENGTH字元
        {
            node->num = (long long int)stoi(str_node->num);//轉換字元並跳出
            break;
        }
        node->num = (long long int)stoi(str_node->num.substr(str_node->num.length() - BASE_LENGTH, BASE_LENGTH));//將字串節點中字串切割輸入數組
        node = node->prev;
        str_node->num.erase(str_node->num.length() - BASE_LENGTH, BASE_LENGTH);//刪除節點中字串BASE_LENGTH位
    }
    return;
}

void bigNum::getStrFromFile(ifstream *file)//將數字從檔案中存至數字字串串列
{
    char input[MAX_STRING_LENGTH + 1];//宣告暫存陣列
    char ch;
    bool check = true;//決定是否繼續迴圈
    while (check)
    {
        for (int i = 0; i < MAX_STRING_LENGTH; i++)
        {
            if (!file->get(ch))//若取至檔案結尾，ch = '\n'
                ch = '\n';
            if (ch == '\n')//若取得換行字元，結束讀取
            {
                if (i != 0)//若尚有剩餘字串
                {
                    input[i] = '\0';
                    string temp(input);
                    this->str.push(temp);//存入剩餘字串
                }
                check = false;
                break;
            }
            else if (ch == '-')//若取得負號
            {
                sign = 0;
                i--;
                continue;
            }
            input[i] = ch;
        }
        if (check)
        {
            input[MAX_STRING_LENGTH] = '\0';
            string temp(input);
            this->str.push(temp);//輸入完整數字字串
        }
    }
    this->stringToBigNum();//將字串串列轉為數組串列
    return;
}

void bigNum::print()//顯示至terminal上
{
    cout << endl;
    if (!sign)
        cout << '-';
    numNode *node = this->begin;
    cout << node->num;
    node = node->next;
    while (node != nullptr)
    {
        cout << setw(BASE_LENGTH) << setfill('0') << node->num;
        node = node->next;
    }
    return;
}

void bigNum::print(ofstream *file)//輸出至檔案
{
    *file << endl;
    if (!sign)
        *file << '-';
    numNode *node = this->begin;
    *file << node->num;
    node = node->next;
    while (node != nullptr)
    {
        *file << setw(BASE_LENGTH) << setfill('0') << node->num;
        node = node->next;
    }
    return;
}

void bigNum::multiply(bigNum *num1, bigNum *num2)//將兩輸入之大數相乘後存於自身
{
    if (num1->sign ^ num2->sign)//確定正負號
        this->sign = 0;
    numNode *num1Node = num1->end;
    numNode *num2Node = num2->end;
    numNode *resultBeginNode = this->end;//該次迴圈��始位置
    numNode *resultCurrentNode = this->end;
    while (num2Node != nullptr)
    {
        while (num1Node != nullptr)
        {
            if (resultCurrentNode == nullptr)//若result拜訪至頭，補0至頭以繼續運算
            {
                this->addToBegin(0);
                resultCurrentNode = this->begin;
            }
            resultCurrentNode->num += num1Node->num * num2Node->num;//相乘
            long long int carry = resultCurrentNode->num / BASE;//取得進位
            if (resultCurrentNode->prev == nullptr && carry != 0LL)//若無上一節點且須進位，則補0節點至頭
            {
                this->addToBegin(0LL);
            }
            if (carry != 0LL)//若須進位
            {
                resultCurrentNode->prev->num += carry;
                resultCurrentNode->num %= BASE;
            }
            num1Node = num1Node->prev;//移至上一項
            resultCurrentNode = resultCurrentNode->prev;
        }
        num1Node = num1->end;//num1從頭，num2移至上一項，result從begin開始
        num2Node = num2Node->prev;
        resultBeginNode = resultBeginNode->prev;
        resultCurrentNode = resultBeginNode;
    }
    return;
}
