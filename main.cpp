#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const char operators[][6] = {"{CON}","{DIS}","{IMP}","{EQV}"};
const int OperatorsAmount = 4;

struct BinaryTreeNode
{
    char* data;
    BinaryTreeNode* leftNode;
    BinaryTreeNode* rightNode;
};

bool isNumber(char ch)
{
    return (ch>='0'&&ch<='9');
}

bool isLetter(char ch)
{
    return ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'));
}

bool isOperator(char* string)
{
    for(int i = 0; i< OperatorsAmount;i++)
    {
        bool isEqual = true;
        for(int j = 0;j<5;j++)
        {
            if(operators[i][j]!=string[j])
            {
                isEqual = false;
            }
        }
        if(isEqual)
        {
            return true;
        }
    }
    return false;
}

bool hasMorePriority(const char* firstOperator, const char* secondOperator)
{
    for(int i = 0; i < OperatorsAmount;i++)
    {
        if(strcmp(firstOperator,operators[i])==0)
        {
            return true;
        } else if(strcmp(secondOperator,operators[i]) == 0)
        {
            return false;
        }
    }
}

BinaryTreeNode* transformStringToBinaryTree(char** string);

BinaryTreeNode* createOperand(char** string)
{
    BinaryTreeNode* result = new BinaryTreeNode;
    char* operand = new char[20];
    int i = 0;
    bool isNegative = false;
    if((**string)=='!')
    {
        isNegative = true;
        (*string)++;
    }
    if((**string) == '(')
    {
        (*string)++;
        result = transformStringToBinaryTree(string);
        return result;
    }
    while(isNumber((**string))||isLetter((**string)))
    {
        operand[i] = **string;
        i++;
        (*string)++;
    }
    operand[i] = '\0';
    result->data = operand;
    if(isNegative)
    {
        BinaryTreeNode* notOperandNode = new BinaryTreeNode;
        notOperandNode->data = new char[2]{'!','\0'};
        notOperandNode->leftNode = result;
        result = notOperandNode;
    }
    return result;
}

BinaryTreeNode* createOperator(char** string)
{
    BinaryTreeNode* result = new BinaryTreeNode;
    result->data = new char[6];
    /*if((**string) == '!')
    {
        result->data[0] = '!';
        result->data[1] = '\0';
        (*string)++;
    }
    else
    {*/
    for(int i = 0;i<5;i++)
    {
        result->data[i] = (**string);
        (*string)++;
    }
    result->data[5] = '\0';
    //}
    return result;
}

BinaryTreeNode* transformStringToBinaryTree(char** string)
{
    BinaryTreeNode* result = new BinaryTreeNode;
    bool isInit = false;
    while((**string))
    {
        if (!isInit)
        {
            result = createOperand(string);
            isInit = true;
        }
        else if((**string)==')')
        {
            (*string)++;
            return result;
        }
        else if (isOperator((*string)))
        {
            BinaryTreeNode *temp = createOperator(string);
            if (!isOperator(result->data)  || !hasMorePriority(temp->data,result->data))
            {
                temp->leftNode = result;
                result = temp;
            }
            else
            {
                temp->leftNode = result->rightNode;
                temp->rightNode = createOperand(string);
                result->rightNode = temp;
            }
        }
        else
        {
            result->rightNode = createOperand(string);
        }
    }
    return result;
}

int main()
{
    char* str;
    ifstream input;
    input.open("in.txt");
    input.getline(str,100);
    BinaryTreeNode* PolinomialTree;
    PolinomialTree = transformStringToBinaryTree(&str);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
