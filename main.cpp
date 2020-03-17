#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

const char operators[][6] = {"{CON}","{DIS}","{IMP}","{EQV}"};
const int TreePrintSpace = 10;

template <typename T>
struct LinkedListNode
{
    T data;
    LinkedListNode* nextNode;
};

struct BinaryTreeNode
{
    char* data;
    BinaryTreeNode* leftNode;
    BinaryTreeNode* rightNode;
};

bool Contains(LinkedListNode<char*>* list, char* data)
{
    while(list)
    {
        if(strcmp(list->data, data)==0)
        {
            return true;
        }
        list = list->nextNode;
    }
    return false;
}

bool isNumber(char ch)
{
    return (ch>='0'&&ch<='9');
}

bool isLetter(char ch)
{
    return ((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'));
}

bool isOperator(const char* string)
{
    for(auto i : operators)
    {
        bool isEqual = true;
        for(int j = 0;j<5;j++)
        {
            if(i[j]!=string[j])
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
    for(auto i : operators)
    {
        if(strcmp(firstOperator,i)==0)
        {
            return true;
        } else if(strcmp(secondOperator,i) == 0)
        {
            return false;
        }
    }
}

BinaryTreeNode* transformStringToBinaryTree(char** string);

BinaryTreeNode* createOperand(char** string)
{
    auto* result = new BinaryTreeNode;
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
        auto* notOperandNode = new BinaryTreeNode;
        notOperandNode->data = new char[2]{'!','\0'};
        notOperandNode->leftNode = result;
        result = notOperandNode;
    }
    return result;
}

BinaryTreeNode* createOperator(char** string)
{
    auto* result = new BinaryTreeNode;
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
    auto* result = new BinaryTreeNode;
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


void printTree(BinaryTreeNode* root, int spaceAmount = 0)
{
    if(root)
    {
        printTree(root->rightNode,spaceAmount+TreePrintSpace);
        cout<<endl;
        for(int i = 0;i<spaceAmount;i++)
        {
            cout<<" ";
        }
        cout<<root->data<<endl;
        printTree(root->leftNode,spaceAmount+TreePrintSpace);
    }
}

void simplifyTree(BinaryTreeNode* root)
{
    if(root == nullptr || (!isOperator(root->data) && strcmp(root->data,"!")!=0))
    {
        return;
    }
    simplifyTree(root->leftNode);
    simplifyTree(root->rightNode);
    if(!strcmp(root->data,"{CON}"))
    {
        if(root->leftNode->data[0] == '0' || root->rightNode->data[0] == '0')
        {
            delete(root->leftNode);
            delete(root->rightNode);
            root->leftNode = nullptr;
            root->rightNode = nullptr;
            strcpy(root->data,"0");
        }
    }
    else if(!strcmp(root->data,"{DIS}"))
    {
        if(root->leftNode->data[0] == '1' || root->rightNode->data[0] == '1')
        {
            delete(root->leftNode);
            delete(root->rightNode);
            root->leftNode = nullptr;
            root->rightNode = nullptr;
            strcpy(root->data,"1");
        }
    }
    else if(!strcmp(root->data,"{IMP}"))
    {
        if(root->leftNode->data[0] == '0' && root->rightNode->data[0] == '0')
        {
            delete(root->leftNode);
            delete(root->rightNode);
            root->leftNode = nullptr;
            root->rightNode = nullptr;
            strcpy(root->data,"1");
        }
        else if(root->leftNode->data[0] == '1' && root->rightNode->data[0] == '1')
        {
            delete(root->leftNode);
            root->leftNode = nullptr;
            *root = *(root->rightNode);
        }
    }else if(!strcmp(root->data,"!"))
    {
        if(root->leftNode->data[0] == '0')
        {
            delete(root->leftNode);
            root->leftNode = nullptr;
            strcpy(root->data,"1");
        }
        else if(root->leftNode->data[0] == '1')
        {
            delete(root->leftNode);
            root->leftNode = nullptr;
            strcpy(root->data,"0");
        }
        else if(root->leftNode->data[0] == '!')
        {
            root->data = root->leftNode->leftNode->data;
        }
    }
}

LinkedListNode<char*>* detectVariables(BinaryTreeNode* root)
{
    LinkedListNode<char*>* result = nullptr;
    auto* queueTail = new LinkedListNode<BinaryTreeNode*>{root, nullptr};
    LinkedListNode<BinaryTreeNode*>* queueHead = queueTail;
    while(queueTail)
    {
        if(queueTail->data->leftNode)
        {
            auto *temp = new LinkedListNode<BinaryTreeNode *>{queueTail->data->leftNode};
            queueHead->nextNode = temp;
            queueHead = temp;
        }
        if(queueTail->data->rightNode)
        {
            auto *temp = new LinkedListNode<BinaryTreeNode *>{queueTail->data->rightNode};
            queueHead->nextNode = temp;
            queueHead = temp;
        }
        if(!isOperator(queueTail->data->data)&&(strcmp(queueTail->data->data,"!")!=0)&&isLetter((queueTail->data->data[0])))
        {
            if(!Contains(result,queueTail->data->data))
            {
                auto *temp = new LinkedListNode<char *>{queueTail->data->data};
                temp->nextNode = result;
                result = temp;
            }
        }
        queueTail = queueTail->nextNode;
    }
    return result;
}

char* calculateOperator(char* oper, const char* firstOperand, const char* secondOperand)
{
    if(!isOperator(oper)&&(strcmp(oper,"!")!=0))
    {
        return nullptr;
    }
    if(!strcmp(oper,"{CON}"))
    {
        if(firstOperand[0] == '0' || secondOperand[0] == '0')
        {
            return new char[2]{'0','\0'};
        }
        else
        {
            return new char[2]{'1','\0'};
        }
    }
    else if(!strcmp(oper,"{DIS}"))
    {
        if(firstOperand[0] == '1' || secondOperand[0] == '1')
        {
            return new char[2]{'1','\0'};
        }
        else
        {
            return new char[2]{'0','\0'};
        }
    }
    else if(!strcmp(oper,"{EQV}"))
    {
        if(firstOperand[0] == secondOperand[0])
        {
            return new char[2]{'1','\0'};
        }
        else
        {
            return new char[2]{'0','\0'};
        }
    }
    else if(!strcmp(oper,"{IMP}"))
    {
        if(firstOperand[0] == '0')
        {
            return new char[2]{'1','\0'};
        }
        else
        {
            return new char[2]{secondOperand[0],'\0'};
        }
    }
    else if(!strcmp(oper,"!"))
    {
        if(firstOperand[0] == '0')
        {
            return new char[2]{'1','\0'};
        }
        else
        {
            return new char[2]{'0','\0'};
        }
    }
}

char* calculateTree(BinaryTreeNode* root, LinkedListNode<char*>* variables, LinkedListNode<char*>* variableData, bool isOuterRecursion = true)
{
    if(isOuterRecursion)
    {
        simplifyTree(root);
    }
    if(!root)
    {
        return nullptr;
    }
    else if(isOperator(root->data) || strcmp(root->data,"!")==0)
    {
        return calculateOperator(root->data
                ,calculateTree(root->leftNode,variables,variableData, false)
                ,calculateTree(root->rightNode,variables,variableData, false));
    }
    else if(isLetter(root->data[0]))
    {
        while(variables)
        {
            if(!strcmp(variables->data,root->data))
            {
                return variableData->data;
            }
            variableData = variableData->nextNode;
            variables = variables->nextNode;
        }
        return nullptr;
    }
    else
    {
        return root->data;
    }
}

int main()
{
    char* str = new char[100];
    ifstream input;
    input.open("in.txt");
    input.getline(str,100);
    BinaryTreeNode* PolinomialTree;
    PolinomialTree = transformStringToBinaryTree(&str);
    cout<<"Unsimplified tree:"<<endl;
    printTree(PolinomialTree);
    simplifyTree(PolinomialTree);
    cout<<endl<<"Simplified tree:"<<endl;
    printTree(PolinomialTree);
    cout<<endl;
    LinkedListNode<char*>* variables = detectVariables(PolinomialTree);
    LinkedListNode<char*>* variablesCpy = variables;
    LinkedListNode<char*>* variablesData = nullptr;
    cout<<"To caclucate the expression enter values of the following variables:"<<endl;
    while(variablesCpy)
    {
        cout<<variablesCpy->data<<": ";
        auto* temp = new LinkedListNode<char*>{new char [2]};
        cin>>temp->data;
        cout<<endl;
        temp->nextNode = variablesData;
        variablesData = temp;
        variablesCpy = variablesCpy->nextNode;
    }
    cout<<"The result is: "<<calculateTree(PolinomialTree,variables,variablesData);
    return 0;
}
