#include <iostream>
#include<string>
#include<fstream>
#define tT template <class T>
using namespace std;

tT class Node
{
public:
    Node();
    Node(const T val);
    T data;
    Node<T> *next;
    //~Node();
};

tT class Stack
{
public:
    Stack();
    bool IsEmpty();
    bool push(const T &val);
    bool pop(T &val);
    T Top();
    void print();
    ~Stack();

private:
    Node<T> *top;
};

//template<class T>
class XMLData
{
public:
    int lineNumber;
    bool StartOrEnd;//0 -> start  and  1 -> end
    string tagText;
    XMLData();
    void printXML();
};
