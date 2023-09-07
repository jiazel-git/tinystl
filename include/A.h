#ifndef __A
#define __A
#include <iostream>
using namespace std;
namespace jz
{
class A
{
public:
    A() = default;
    A(double rate, string currency) :
        m_rate(rate), m_currency(currency) {}
    // A(const A& otr) { cout << "copy constructor" << endl; }
    // A(const A&&) {cout << "move constructor" << endl; }
    double get_rate() { return m_rate; }
    string get_currency() { return m_currency; }
    friend ostream& operator<<(ostream& os, A& a) {
        os << a.m_currency << " " << a.m_rate << endl;
        return os;
    }
    friend ostream& operator<<(ostream& os, A* a) {
        os << a->m_currency << " " << a->m_rate << endl;
        return os;
    }

private:
    double m_rate;
    string m_currency;
};

} // namespace jz
#endif