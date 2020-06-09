#include "stackXML.h"

tT Node<T>::Node() {}

tT Node<T>::Node(const T val)
{
    this->data=val;
}

tT Stack<T>::Stack()
{
    top=nullptr;
}

tT bool Stack<T>::IsEmpty()
{
    if(!top)
        return true;
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
    bool attributeError = false;

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

            if(OpeningAngularBracketIndex == -1 && ClosingAngularBracketIndex == -1)//if true, it means the current line contain no tag
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
            //check if there's a proper end of prolog
            else if(  (OpeningAngularBracketIndex==-1 || OpeningAngularBracketIndex>ClosingAngularBracketIndex) && line[ClosingAngularBracketIndex-1]=='?' )
            {
                if(lineCounter==1)
                {
                    cout<<"---- ERROR ---- \tThere's no start of prolog but its end exists.\n";
                    foundError = true;
                    break;
                }
                else
                {
                    cout<<"---- ERROR ---- \tThere's no start of prolog but its end exists. It's at line "<<lineCounter<<" (prolog must be at line 1)\n";
                    foundError = true;
                    break;
                }
                
            }

            //check if it's end of a comment
            else if(  (line[ClosingAngularBracketIndex-1] == '-') && (line[ClosingAngularBracketIndex-2] == '-') && (line.find("-->")==line.find("--"))  )
            {
                xml=St.Top();
                if(xml.tagText == "Comment")
                {
                    St.pop(xml);
                    //cout<<"Popped: "; xml.printXML();
                }
                else
                {
                    cout<<"---- ERROR ---- \tfound closing tag of Comment at line "<<lineCounter<<", but there's no starting tag for it.";
                    foundError = true;
                    break;
                }
                
                line = line.substr(ClosingAngularBracketIndex+1, line.size());
                CurrentLineLength=line.length();
            }

            //check if it's start of a simple tag with or without attribute.
            else if( (line[OpeningAngularBracketIndex]=='<')  &&  ((line[OpeningAngularBracketIndex+1] >= 65 && line[OpeningAngularBracketIndex+1] <= 90) || (line[OpeningAngularBracketIndex+1] >=97 && line[OpeningAngularBracketIndex+1] <= 122))  /*&&  (OpeningAngularBracketIndex < ClosingAngularBracketIndex)*/  )
            {
                xml.lineNumber=lineCounter;
                xml.StartOrEnd=0;

                string Tag;
                int FirstSpaceAfterTag = line.find(' ');

                //check if there's an attribute
                if(FirstSpaceAfterTag != -1 && (FirstSpaceAfterTag < ClosingAngularBracketIndex || ClosingAngularBracketIndex==-1))
                {
                    Tag = line.substr(1, FirstSpaceAfterTag-1);

                    line = line.substr(FirstSpaceAfterTag+1, line.size());//delete tag name from current line

                    int firstQuoteS = line.find("'");//find first single quote
                    int firstQuoteD = line.find('"');//find first double quote

                    //if first "double quote" exists and there's a charachter after that double quote (i.e., name of attribute)
                    if(firstQuoteD<firstQuoteS && firstQuoteD!=-1 && ((line[firstQuoteD+1] >= 65 && line[firstQuoteD+1] <= 90) || (line[firstQuoteD+1] >=97 && line[firstQuoteD+1] <= 122)))
                    {
                        line=line.substr(firstQuoteD+1, line.size());//delete line from start upto first double quote(icluding)

                        int secondQuoteD = line.find('"');//find second double quote
                        ClosingAngularBracketIndex = line.find('>');//update index of '>'
                        
                        //checking if current tag is properly closed; both when there's only one tag or multiple tags in a single line.
                        if(   (ClosingAngularBracketIndex==-1 || (secondQuoteD!=-1 && secondQuoteD<ClosingAngularBracketIndex)) && (  (line.find("\">")==-1) || (line.find("\">")!=-1 && secondQuoteD<line.find("\">"))  )   )
                        {
                            cout<<"---- ERROR ---- \tNo proper closing of tag: <"<<Tag<<"> at line "<<lineCounter;
                            foundError = true;
                            break;
                        }
                        //checking if current attribute is quoted; both when there's only one attribute or multiple attributes in a single line.
                        else if(ClosingAngularBracketIndex!=-1 && (secondQuoteD>ClosingAngularBracketIndex || secondQuoteD==-1))
                        {
                            cout<<"---- ERROR ---- \tThe attribute is not properly quoted at line "<<lineCounter;
                            attributeError = true;
                            break;
                        }
                    }

                    //if first 'single quote' exists and there's a charachter after that single quote (i.e., name of attribute)
                    else if(firstQuoteS!=-1 && ((line[firstQuoteS+1] >= 65 && line[firstQuoteS+1] <= 90) || (line[firstQuoteS+1] >=97 && line[firstQuoteS+1] <= 122)))
                    {
                        line=line.substr(firstQuoteS+1, line.size());//delete line from start upto first single quote(icluding)

                        int secondQuoteS = line.find("'");//find second 'single quote'
                        ClosingAngularBracketIndex = line.find('>');//update index of '>'
                        
                        //checking if current tag is properly closed; both when there's only one tag or multiple tags in a single line.
                        if(   (ClosingAngularBracketIndex==-1 || (secondQuoteS!=-1 && secondQuoteS<ClosingAngularBracketIndex)) && (  (line.find("'>")==-1) || (line.find("'>")!=-1 && secondQuoteS<line.find("'>"))  )   )
                        {
                            cout<<"---- ERROR ---- \tNo proper closing of tag: <"<<Tag<<"> at line "<<lineCounter;
                            foundError = true;
                            break;
                        }
                        //checking if current attribute is quoted; both when there's only one attribute or multiple attributes in a single line.
                        else if(ClosingAngularBracketIndex!=-1 && (secondQuoteS>ClosingAngularBracketIndex || secondQuoteS==-1))
                        {
                            cout<<"---- ERROR ---- \tThe attribute is not properly quoted at line "<<lineCounter;
                            attributeError = true;
                            break;
                        }
                    }
                }

                else//if there's no attribute
                    Tag = line.substr(1, ClosingAngularBracketIndex-1);

                xml.tagText=Tag;

                St.push(xml);
                //cout<<"Pushed: "; xml.printXML();
                
                ClosingAngularBracketIndex=line.find('>');
                line = line.substr( ClosingAngularBracketIndex+1, line.size());
                CurrentLineLength=line.length();
            }
            //icheck if it's end of a simple tag with or without attribute.
            else if(  (line[OpeningAngularBracketIndex]=='<')  &&  (line[OpeningAngularBracketIndex+1]=='/')  &&  ((line[OpeningAngularBracketIndex+2] >= 65 && line[OpeningAngularBracketIndex+2] <= 90) || (line[OpeningAngularBracketIndex+2] >=97 && line[OpeningAngularBracketIndex+2] <= 122))  )
            {
                line = line.substr(OpeningAngularBracketIndex+2, line.size());
                int TagLength = line.find('>');
                string Tag = line.substr(0, TagLength);

                if(!St.IsEmpty())//check if there's a starting tag against incoming ending tag
                    xml = St.Top();
                else
                {
                    cout<<"---- ERROR ---- \tfound closing tag: </"<<Tag<<"> at line "<<lineCounter<<", but there's no starting tag for it.";
                    foundError = true;
                    break;
                }
                

                int x=xml.tagText.compare(Tag);
                if(x==0)
                {
                    St.pop(xml);
                    //cout<<"Popped: "; xml.printXML();
                }
                else
                {
                    cout<<"---- ERROR ---- \tThere MUST be a closing tag: </"<<xml.tagText<<"> before encountered closing tag i.e., </"<<Tag<<"> at line "<<lineCounter;
                    foundError = true;
                    break;
                }

                line = line.substr(TagLength+1, line.size());
                CurrentLineLength=line.length();
            }


            //check if it's start of a comment
            else if(line[OpeningAngularBracketIndex+1] == '!' && line[OpeningAngularBracketIndex+2] == '-' && line[OpeningAngularBracketIndex+3] == '-')
            {
                line = line.substr(OpeningAngularBracketIndex+4, line.size());// <!-- these 4 charachters are deleted from string
                CurrentLineLength=line.length();

                xml.lineNumber=lineCounter;
                xml.StartOrEnd=0;
                xml.tagText="Comment";

                St.push(xml);
                //cout<<"Pushed: "; xml.printXML();
            }
            
            //check if there are double dashes in the comment
            else if( line.find("--") != line.find("-->") )
            {
                if(OpeningAngularBracketIndex==-1 && ClosingAngularBracketIndex==-1)
                {
                    CurrentLineLength=0;
                }
                else
                {
                    cout<<"---- ERROR ---- \tAt line "<<lineCounter<<", Two dashes in the middle of a comment are not allowed.";
                    foundError = true;
                    break;
                }
                
            }
        }

        if(foundError || attributeError)
            break;

        lineCounter++;
    }

    //show error if stack is not empty
    if(!St.IsEmpty() && !attributeError && !foundError)
    {
        cout<<"---- ERROR ---- \tFound ";

        if(St.Top().StartOrEnd==0)
            cout<<"Starting";
        else
            cout<<"Ending";

        cout<<" tag \""<<St.Top().tagText<<"\" at line "<<St.Top().lineNumber<<" but it has no ";

        if(St.Top().StartOrEnd==0)
            cout<<"Ending";
        else
            cout<<"Starting";

        cout<<" tag\n";
    }
    else if(!foundError && !attributeError)
        cout<<"No Error found in this xml file.\n";

    fin.close();
}

int main()
{
    checkXML("xml.txt");
    
    return 0;
}