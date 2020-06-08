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
    cout<<"Tag name = "<<tagText<<",  Line Number = "<<lineNumber<<", ";
    if(!StartOrEnd)
        cout<<"Starting tag\n";
    else
        cout<<"Ending tag\n";
}

void checkXML(string filename)
{
    Stack<XMLData> St;
    int lineCounter = 1;
    bool foundError = false;

    ifstream fin;
    fin.open(filename);

    while(!fin.eof())
    {
        XMLData xml;
        string line;
        getline(fin, line, '\n');//reads whole line so that line number can be tracked

        int CurrentLineLength=line.length();
        while( CurrentLineLength>0 )
        {
            int OpeningAngularBracketIndex = line.find('<');
            int ClosingAngularBracketIndex = line.find('>');

            if(OpeningAngularBracketIndex == -1 && ClosingAngularBracketIndex == -1)
                CurrentLineLength = 0;

            //NOTE: XML prolog is optional. If it exists, it must come first in the document.
            if(line[OpeningAngularBracketIndex+1] == '?')//if true, it's a prolog
            {
                if(lineCounter==1)
                {
                    line = line.substr(OpeningAngularBracketIndex+2, line.size());//ignore s[0]='<' and s[1]='?'
                    CurrentLineLength=line.length();

                    int endOfProlog = line.find("?>");
                    if(endOfProlog==-1)//if false, it means prolog is complete
                    {
                        cout<<"---- ERROR ---- \tThe prolog is not complete.\n";
                        foundError = true;
                        break;
                    }
                    else//if prolog is complete, then check if there is another tag
                    {
                        int secondAngularBracketIndex = line.find('<');
                        if (secondAngularBracketIndex==-1)//if there is no other tag besides prolog, then clear the current line
                        {    
                            CurrentLineLength=0;
                        }
                        else//else, delete the contents of line before another tag (i.e., tag after prolog in the current line)
                        {
                            line = line.substr(secondAngularBracketIndex, line.size());
                            CurrentLineLength=line.length();
                        }
                        
                    }
                }
                else//if prolog is not in the first line; show error
                {
                    cout<<"---- ERROR ---- \tProlog must be at the start of xml file.\n";
                    foundError = true;
                    break;
                }
                
            }
            //else if(line[ClosingAngularBracketIndex-1] == '?')//if true, it means it's end of prolog
            //else if(!line.empty() && ClosingAngularBracketIndex==-1)
            //{
                // xml=St.Top();
                // if(xml.tagText == "xml")//prolog will always contain "xml" as a tag text
                // {
                    // St.pop(xml);
                    // cout<<"Popped: "; xml.printXML();
                // }
                // else
                // {
                //     cout<<"---- ERROR ---- \tno closing tag found but opening tag exists as follows;\n";
                //     xml.printXML();
                //     foundError = true;
                //     break;
                // }
                

            //     line = line.substr(ClosingAngularBracketIndex+1, line.size());
            //     CurrentLineLength=line.length();

            // }

            //NOTE: Two dashes in the middle of a comment are not allowed
            else if(line[OpeningAngularBracketIndex+1] == '!' && line[OpeningAngularBracketIndex+2] == '-' && line[OpeningAngularBracketIndex+3] == '-')//if true, it's start of a comment
            {
                line = line.substr(OpeningAngularBracketIndex+4, line.size());// <!-- these 4 charachters are deleted from string
                CurrentLineLength=line.length();

                xml.lineNumber=lineCounter;
                xml.StartOrEnd=0;
                xml.tagText="Comment";

                St.push(xml);
                cout<<"Pushed: "; xml.printXML();
            }
            else if(line[ClosingAngularBracketIndex-1] == '-' && line[ClosingAngularBracketIndex-2] == '-')//if true, it means it's end of a comment
            {
                xml=St.Top();
                if(xml.tagText == "Comment")
                {
                    St.pop(xml);
                    cout<<"Popped: "; xml.printXML();
                }
                else
                {
                    cout<<"---- ERROR ---- \tno closing tag found but opening tag exists as follows;\n";
                    xml.printXML();
                    foundError = true;
                    break;
                }
                
                line = line.substr(ClosingAngularBracketIndex+1, line.size());
                CurrentLineLength=line.length();
            }

            
            else if(  (line[OpeningAngularBracketIndex]=='<')  &&  ((line[OpeningAngularBracketIndex+1] >= 65 && line[OpeningAngularBracketIndex+1] <= 90) || (line[OpeningAngularBracketIndex+1] >=97 && line[OpeningAngularBracketIndex+1] <= 122))  &&  (line[ClosingAngularBracketIndex]=='>')  )//if true, it's start of a simple tag
            {
                xml.lineNumber=lineCounter;
                xml.StartOrEnd=0;

                string Tag;
                int FirstSpaceAfterTag = line.find(' ');

                if(FirstSpaceAfterTag != -1 && FirstSpaceAfterTag < ClosingAngularBracketIndex)
                    Tag = line.substr(1, FirstSpaceAfterTag-1);
                else
                    Tag = line.substr(1, ClosingAngularBracketIndex-1);

                xml.tagText=Tag;

                St.push(xml);
                cout<<"Pushed: "; xml.printXML();

                line = line.substr(ClosingAngularBracketIndex+1, line.size());
                CurrentLineLength=line.length();
            }
            else if(  (line[OpeningAngularBracketIndex]=='<')  &&  (line[OpeningAngularBracketIndex+1]=='/')  &&  ((line[OpeningAngularBracketIndex+2] >= 65 && line[OpeningAngularBracketIndex+2] <= 90) || (line[OpeningAngularBracketIndex+2] >=97 && line[OpeningAngularBracketIndex+2] <= 122))  )//if true, it's end of a simple tag
            {
                line = line.substr(OpeningAngularBracketIndex+2, line.size());
                int TagLength = line.find('>');
                string Tag = line.substr(0, TagLength);
                xml = St.Top();

                int x=xml.tagText.compare(Tag);
                if(x==0)
                {
                    St.pop(xml);
                    cout<<"Popped: "; xml.printXML();
                }
                else
                {
                    cout<<"---- ERROR ---- \tno closing tag found but opening tag exists as follows;\n";
                    xml.printXML();
                    foundError = true;
                    break;
                }

                line = line.substr(TagLength+1, line.size());
                CurrentLineLength=line.length();

                
            }
        }

        if(foundError)
            break;

        lineCounter++;
    }

    if(!foundError)
        cout<<"No Error found in this xml file.\n";

    fin.close();
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