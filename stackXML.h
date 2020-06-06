#include<iostream>

using namespace std;

template<class T>
class Node 
{
public:
    Node()
    T data;
    Node<T> * next;
};

template<class T>
class Stack 
{ 
public:
    Stack(); 
    ~Stack();
    bool IsEmpty();
    bool push(const T & val);
    bool pop(T & val);
    T top(); 
    void print(); 
private:
    Node<T> * top;
};

//template<class T>
class XMLData
{


};
