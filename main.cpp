#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

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

//TODO: remove or rework after realisation of binary operators
bool isOperator(char ch)
{
    return (ch=='+'||ch=='-'||ch=='*'||ch=='/');
}

BinaryTreeNode* transformStringToBinaryTree(char** string);

BinaryTreeNode* createOperand(char** string)
{
    BinaryTreeNode* result = new BinaryTreeNode;
    char* operand = new char[20];
    int i = 0;
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
    return result;
}

BinaryTreeNode* createOperator(char** string)
{
    //TODO: edit this part of code to work with binary operators
    BinaryTreeNode* result = new BinaryTreeNode;
    result->data = new char;
    result->data[0] = (**string);
    (*string)++;
    return result;
}

BinaryTreeNode* transformStringToBinaryTree(char** string)
{
    BinaryTreeNode* result = new BinaryTreeNode;
    bool isInit = false;
    while((**string))
    {
        if((**string)==')')
        {
            (*string)++;
            return result;
        }
        if (!isInit)
        {
            result = createOperand(string);
            isInit = true;
        }
        else if (isOperator((**string)))
        {
            BinaryTreeNode *temp = createOperator(string);
            if (temp->data[0] == '+' || temp->data[0] == '-' || !isOperator(result->data[0]))
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
