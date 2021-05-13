#include <ostream>

class Complex
{
private:
    int _a,_b;
public:
    Complex(int a, int b) : _a(a) , _b(b) {}

    friend std::ostream& operator<<(std::ostream &os, const Complex &complex);

    Complex& operator+=(const Complex c)
    {
        _a += c._a;
        _b += c._b;
        return *this;
    }

    bool operator==(const Complex &rhs) const
    {
        return _a == rhs._a && _b == rhs._b;
    }

    Complex operator+(const Complex c) const
    {
        Complex res (_a + c._a, _b + c._b);
        return res;
    }

    Complex operator!() const
    {
        Complex conj (_a, -1 * _b);
        return conj;
    }

    Complex& operator++()
    {
        _a++;
        _b++;
        return *this;
    }

    Complex operator++(int)
    {
        Complex temp (_a, _b);
        _a++;
        _b++;
        return temp;
    }

};

std::ostream& operator<<(std::ostream &os, const Complex &complex)
{
    if (complex._b >= 0)
    {
        os << complex._a << "+i" << complex._b << std::endl;
    }
    else
    {
        os << complex._a << "-i" << -complex._b << std::endl;
    }
    return os;
}