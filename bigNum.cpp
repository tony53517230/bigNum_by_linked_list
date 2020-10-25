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

numStr::~numStr()//±N©Ò¦³¼Æ¦r¦r¦ê¸`ÂI§R°£
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

void numStr::push(string num)//±Nnum¶ë¤J¦ê¦C§À
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

bigNum::~bigNum()//±N©Ò¦³¼Æ²Õ¸`ÂI§R°£
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

void bigNum::push(long long int num)//±N¼Æ²Õ¶ë¤J¦ê¦C§À
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

void bigNum::addToBegin(long long int num)//±N¼Æ²Õ¶ë¤J¦ê¦CÀY
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

void bigNum::stringToBigNum()//±N¼Æ¦r¦r¦ê¦ê¦CÂà¬°¼Æ²Õ°}¦C
{
    numNode *node = this->end;
    strNode *str_node = this->str.end;
    int i;
    for (i = 0; str_node->num.length() > 0 && str_node != nullptr; i++)//¦Û¦r¦ê¦ê¦C§À³¡«ô³X
    {
        if (node == nullptr)//­Y¼Æ²Õ¦ê¦C«ô³X§¹¡A²K¥[·s¸`ÂI¦ÜÀY
        {
            this->addToBegin(0);
            node = this->begin;
        }
        if (str_node->prev != nullptr && str_node->num.length() <= BASE_LENGTH)//­Y¼Æ¦r¦r¦ê¥¼«ô³X§¹¥B³Ñ¾l¸Ó¦r¦ê¸`ÂI¤§¦r¤¸¤p©óBASE_LENGTH¦ì
        {
            str_node->prev->num += str_node->num;//±N¦¹¸`ÂI¤§¦r¤¸±µ¤J¤W¤@­Ó¸`ÂI¦r¦ê¤¤
            str_node = str_node->prev;//«ô³X¤W¤@¸`ÂI
        }
        if (str_node->num.length() <= BASE_LENGTH)//­Y¦r¦ê¦ê¦C«ô³X¦ÜÀY¡A¥B³Ñ¾l¤p©óBASE_LENGTH¦r¤¸
        {
            node->num = (long long int)stoi(str_node->num);//Âà´«¦r¤¸¨Ã¸õ¥X
            break;
        }
        node->num = (long long int)stoi(str_node->num.substr(str_node->num.length() - BASE_LENGTH, BASE_LENGTH));//±N¦r¦ê¸`ÂI¤¤¦r¦ê¤Á³Î¿é¤J¼Æ²Õ
        node = node->prev;
        str_node->num.erase(str_node->num.length() - BASE_LENGTH, BASE_LENGTH);//§R°£¸`ÂI¤¤¦r¦êBASE_LENGTH¦ì
    }
    return;
}

void bigNum::getStrFromFile(ifstream *file)//±N¼Æ¦r±qÀÉ®×¤¤¦s¦Ü¼Æ¦r¦r¦ê¦ê¦C
{
    char input[MAX_STRING_LENGTH + 1];//«Å§i¼È¦s°}¦C
    char ch;
    bool check = true;//¨M©w¬O§_Ä~Äò°j°é
    while (check)
    {
        for (int i = 0; i < MAX_STRING_LENGTH; i++)
        {
            if (!file->get(ch))//­Y¨ú¦ÜÀÉ®×µ²§À¡Ach = '\n'
                ch = '\n';
            if (ch == '\n')//­Y¨ú±o´«¦æ¦r¤¸¡Aµ²§ôÅª¨ú
            {
                if (i != 0)//­Y©|¦³³Ñ¾l¦r¦ê
                {
                    input[i] = '\0';
                    string temp(input);
                    this->str.push(temp);//¦s¤J³Ñ¾l¦r¦ê
                }
                check = false;
                break;
            }
            else if (ch == '-')//­Y¨ú±o­t¸¹
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
            this->str.push(temp);//¿é¤J§¹¾ã¼Æ¦r¦r¦ê
        }
    }
    this->stringToBigNum();//±N¦r¦ê¦ê¦CÂà¬°¼Æ²Õ¦ê¦C
    return;
}

void bigNum::print()//Åã¥Ü¦Üterminal¤W
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

void bigNum::print(ofstream *file)//¿é¥X¦ÜÀÉ®×
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

void bigNum::multiply(bigNum *num1, bigNum *num2)//±N¨â¿é¤J¤§¤j¼Æ¬Û­¼«á¦s©ó¦Û¨­
{
    if (num1->sign ^ num2->sign)//½T©w¥¿­t¸¹
        this->sign = 0;
    numNode *num1Node = num1->end;
    numNode *num2Node = num2->end;
    numNode *resultBeginNode = this->end;//¸Ó¦¸°j°éþ©l¦ì¸m
    numNode *resultCurrentNode = this->end;
    while (num2Node != nullptr)
    {
        while (num1Node != nullptr)
        {
            if (resultCurrentNode == nullptr)//­Yresult«ô³X¦ÜÀY¡A¸É0¦ÜÀY¥HÄ~Äò¹Bºâ
            {
                this->addToBegin(0);
                resultCurrentNode = this->begin;
            }
            resultCurrentNode->num += num1Node->num * num2Node->num;//¬Û­¼
            long long int carry = resultCurrentNode->num / BASE;//¨ú±o¶i¦ì
            if (resultCurrentNode->prev == nullptr && carry != 0LL)//­YµL¤W¤@¸`ÂI¥B¶·¶i¦ì¡A«h¸É0¸`ÂI¦ÜÀY
            {
                this->addToBegin(0LL);
            }
            if (carry != 0LL)//­Y¶·¶i¦ì
            {
                resultCurrentNode->prev->num += carry;
                resultCurrentNode->num %= BASE;
            }
            num1Node = num1Node->prev;//²¾¦Ü¤W¤@¶µ
            resultCurrentNode = resultCurrentNode->prev;
        }
        num1Node = num1->end;//num1±qÀY¡Anum2²¾¦Ü¤W¤@¶µ¡Aresult±qbegin¶}©l
        num2Node = num2Node->prev;
        resultBeginNode = resultBeginNode->prev;
        resultCurrentNode = resultBeginNode;
    }
    return;
}
