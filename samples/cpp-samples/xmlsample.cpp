#include "xmldocument.h"
#include <iostream>
using namespace std;

int main()
{
    XmlDocument doc("bookstore.xml");
    cout << doc.to_string() << endl;
    cout << " ==" << endl;
    for(auto p : doc)
    {
        cout << p->name() << ": "<< p->value()<< endl;
        for (auto p1 : *p)
        {
            cout << p1->name() << ": "<< p1->value()<< endl;
        }
    }
    cout << " ==" << endl;
  return 0;
}
