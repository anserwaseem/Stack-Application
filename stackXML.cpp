#include "stackXML.h"

tT Node<T>::Node()
{
    //this->next=new Node();
}

tT Node<T>::Node(const T val)
{
    //this->next=new Node();
    this->data=val;
}

// tT Node<T>::~Node()
// {
    
// }

tT Stack<T>::Stack()
{
    //this->top=new Node<T>;
    top=nullptr;
}

tT bool Stack<T>::IsEmpty()
{
    if(!top)
    {
        cout<<"Stack is empty\n";
        return true;
    }
    return false;
}

tT bool Stack<T>::pop(T &val)
{
    if(!IsEmpty())
    {
        val=top->data;

        Node<T>* aux = top;
        top=top->next;
        aux->next=nullptr;
        aux=nullptr;
        return true;
    }
    return false;
}

tT bool Stack<T>::push(const T &val)
{
    Node<T>* aux=new Node<T>(val);
    aux->next=top;
    top=aux;
    return true;
}

tT void Stack<T>::print()
{
    Node<T>* aux=top;
    while(aux!=nullptr)
    {
        cout<<aux->data<<"\t";

        if(aux->next==nullptr)
            break;

        aux=aux->next;
    }
}

tT T Stack<T>::Top()
{
    return top->data;
}

tT Stack<T>::~Stack()
{

}

XMLData::XMLData()
{
    lineNumber=0;
    StartOrEnd=0;
}

void XMLData::printXML()
{
    cout<<"Tag name"<<tagText<<"\tLine Number"<<lineNumber<<"\t";
    if(StartOrEnd)
        cout<<"Starting tag\n";
    else
        cout<<"Ending tag\n";
}

void checkXML(string filename)
{
    Stack<XMLData> St;
    int lineCounter = 1;

    ifstream fin;
    fin.open(filename);

    while(!fin.eof())
    {
        XMLData xml;
        string line;
        getline(fin, line, '\n');//reads whole line so that line number can be tracked

        int OpeningAngularBracket = line.find('<');
        int ClosingAngularBracket = line.find('>');

        //NOTE: XML prolog is optional. If it exists, it must come first in the document.
        if(line[OpeningAngularBracket+1] == '?')//if true, it's a prolog
        {
            xml.lineNumber=lineCounter;
            xml.StartOrEnd=0;//starting tag is found

            line = line.substr(OpeningAngularBracket+2, line.size());//ignore s[0]='<' and s[1]='?'
            int FirstSpaceAfterTag = line.find(' ');

            string Tag = line.substr(0, FirstSpaceAfterTag);
            xml.tagText=Tag;

            St.push(xml);
            cout<<"Pushed: "; xml.printXML();
        }
        else if(line[ClosingAngularBracket-1] == '?')//if true, it means it's end of prolog
        {
            St.pop(xml);
            cout<<"Popped: "; xml.printXML();
        }

        //NOTE: Two dashes in the middle of a comment are not allowed
        else if(line[OpeningAngularBracket+1] == '!' && line[OpeningAngularBracket+2] == '-' && line[OpeningAngularBracket+3] == '-')//if true, it's start of a comment
        {
            xml.lineNumber=lineCounter;
            xml.StartOrEnd=0;
            xml.tagText="Comment";

            St.push(xml);
            cout<<"Pushed: "; xml.printXML();
        }
        else if(line[ClosingAngularBracket-1] == '-' && line[ClosingAngularBracket-2] == '-')//if true, it means it's  end of a comment
        {
            St.pop(xml);
            cout<<"Popped: "; xml.printXML();
        }


        else if(  (line[OpeningAngularBracket+1] >= 65 && line[OpeningAngularBracket+1] <= 90) || (line[OpeningAngularBracket+1] >=97 && line[OpeningAngularBracket+1] <= 102)  )//if true, it's start of a simple tag
        {
            xml.lineNumber=lineCounter;
            xml.StartOrEnd=0;

            line = line.substr(OpeningAngularBracket+1, line.size());
            int FirstSpaceAfterTag = line.find(' ');

            string Tag = line.substr(0, FirstSpaceAfterTag);
            xml.tagText=Tag;

            St.push(xml);
            cout<<"Pushed: "; xml.printXML();
        }
        else if(  (line[ClosingAngularBracket-1] >= 65 && line[ClosingAngularBracket-1] <= 90) || (line[ClosingAngularBracket-1] >=97 && line[ClosingAngularBracket-1] <= 102)  )//if true, it's end of a simple tag
        {
            St.pop(xml);
            cout<<"Popped: "; xml.printXML();
        }

        lineCounter++;
    }
}

int main()
{
    // Stack<int> s;
    // cout<<s.IsEmpty();
    // cout<<s.push(2);
    // cout<<s.push(3);
    // cout<<s.push(4);
    // s.print();
    // cout<<s.push(5);
    // cout<<s.push(6);
    // int g; cout<<s.pop(g)<<g<<endl;
    // cout<<s.pop(g)<<"\t"<<g;
    // g=s.Top();cout<<endl<<g;
    // s.print();
    // cout<<s.IsEmpty();


    // Stack<XMLData> S1;
    // XMLData f;
    checkXML("xml.txt");
    
    return 0;
}