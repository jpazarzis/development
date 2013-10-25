#include <iostream>
#include <memory>
#include <vector>
using namespace std;
class A
{
    public:
        A() {
            std::cout << "making A " << std::endl;
        }

        ~A(){
            std::cout << "destructing A" << std::endl;
        }

        void speak() {
            std::cout << "hello" << std::endl;
        }

};


void f(const std::unique_ptr<A>& p)
{
    std::unique_ptr<A> junk = std::forward(p);
    p->speak();
}

/*
std::unique_ptr<A> g(const std::unique_ptr<A>& p)
{   
    auto junk = std::move(p);
    return std::move(p);
}
*/



int main(){
      using A_PTR = std::unique_ptr<A> ;
      vector<A_PTR > v;
      
      A_PTR junk (new A());

      A_PTR junk2 = move(junk);



      v.push_back(move(junk));
      v.push_back(A_PTR(new A()));
      v.push_back(A_PTR(new A()));

      for(auto& p: v)
        f(p);
        
       return 0;
}

