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

numStr::~numStr()//�N�Ҧ��Ʀr�r��`�I�R��
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

void numStr::push(string num)//�Nnum��J��C��
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

bigNum::~bigNum()//�N�Ҧ��Ʋո`�I�R��
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

void bigNum::push(long long int num)//�N�Ʋն�J��C��
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

void bigNum::addToBegin(long long int num)//�N�Ʋն�J��C�Y
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

void bigNum::stringToBigNum()//�N�Ʀr�r���C�ର�Ʋհ}�C
{
    numNode *node = this->end;
    strNode *str_node = this->str.end;
    int i;
    for (i = 0; str_node->num.length() > 0 && str_node != nullptr; i++)//�ۦr���C�������X
    {
        if (node == nullptr)//�Y�Ʋզ�C���X���A�K�[�s�`�I���Y
        {
            this->addToBegin(0);
            node = this->begin;
        }
        if (str_node->prev != nullptr && str_node->num.length() <= BASE_LENGTH)//�Y�Ʀr�r�ꥼ���X���B�Ѿl�Ӧr��`�I���r���p��BASE_LENGTH��
        {
            str_node->prev->num += str_node->num;//�N���`�I���r�����J�W�@�Ӹ`�I�r�ꤤ
            str_node = str_node->prev;//���X�W�@�`�I
        }
        if (str_node->num.length() <= BASE_LENGTH)//�Y�r���C���X���Y�A�B�Ѿl�p��BASE_LENGTH�r��
        {
            node->num = (long long int)stoi(str_node->num);//�ഫ�r���ø��X
            break;
        }
        node->num = (long long int)stoi(str_node->num.substr(str_node->num.length() - BASE_LENGTH, BASE_LENGTH));//�N�r��`�I���r����ο�J�Ʋ�
        node = node->prev;
        str_node->num.erase(str_node->num.length() - BASE_LENGTH, BASE_LENGTH);//�R���`�I���r��BASE_LENGTH��
    }
    return;
}

void bigNum::getStrFromFile(ifstream *file)//�N�Ʀr�q�ɮפ��s�ܼƦr�r���C
{
    char input[MAX_STRING_LENGTH + 1];//�ŧi�Ȧs�}�C
    char ch;
    bool check = true;//�M�w�O�_�~��j��
    while (check)
    {
        for (int i = 0; i < MAX_STRING_LENGTH; i++)
        {
            if (!file->get(ch))//�Y�����ɮ׵����Ach = '\n'
                ch = '\n';
            if (ch == '\n')//�Y���o����r���A����Ū��
            {
                if (i != 0)//�Y�|���Ѿl�r��
                {
                    input[i] = '\0';
                    string temp(input);
                    this->str.push(temp);//�s�J�Ѿl�r��
                }
                check = false;
                break;
            }
            else if (ch == '-')//�Y���o�t��
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
            this->str.push(temp);//��J����Ʀr�r��
        }
    }
    this->stringToBigNum();//�N�r���C�ର�Ʋզ�C
    return;
}

void bigNum::print()//��ܦ�terminal�W
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

void bigNum::print(ofstream *file)//��X���ɮ�
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

void bigNum::multiply(bigNum *num1, bigNum *num2)//�N���J���j�Ƭۭ���s��ۨ�
{
    if (num1->sign ^ num2->sign)//�T�w���t��
        this->sign = 0;
    numNode *num1Node = num1->end;
    numNode *num2Node = num2->end;
    numNode *resultBeginNode = this->end;//�Ӧ��j����l��m
    numNode *resultCurrentNode = this->end;
    while (num2Node != nullptr)
    {
        while (num1Node != nullptr)
        {
            if (resultCurrentNode == nullptr)//�Yresult���X���Y�A��0���Y�H�~��B��
            {
                this->addToBegin(0);
                resultCurrentNode = this->begin;
            }
            resultCurrentNode->num += num1Node->num * num2Node->num;//�ۭ�
            long long int carry = resultCurrentNode->num / BASE;//���o�i��
            if (resultCurrentNode->prev == nullptr && carry != 0LL)//�Y�L�W�@�`�I�B���i��A�h��0�`�I���Y
            {
                this->addToBegin(0LL);
            }
            if (carry != 0LL)//�Y���i��
            {
                resultCurrentNode->prev->num += carry;
                resultCurrentNode->num %= BASE;
            }
            num1Node = num1Node->prev;//���ܤW�@��
            resultCurrentNode = resultCurrentNode->prev;
        }
        num1Node = num1->end;//num1�q�Y�Anum2���ܤW�@���Aresult�qbegin�}�l
        num2Node = num2Node->prev;
        resultBeginNode = resultBeginNode->prev;
        resultCurrentNode = resultBeginNode;
    }
    return;
}
