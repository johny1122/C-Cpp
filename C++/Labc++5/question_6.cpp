#include <iostream>

template <class T, int size>

class Vector
{
private:
    T _v[size];

public:
    Vector() : Vector (0) {};

    Vector(const T& value) { for (int i = 0; i < size; i++) _v[i] = value; };

    T operator[](int index) const { return _v[index]; }

    T& operator[](int index) { return _v[index]; }

    T operator*(const Vector<T,size>& rhs) const
    {
        T dot_product = 0;
        for (int index = 0; index < size; index++) dot_product += (_v[index] * rhs[index]);
        return dot_product;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector<T,size>& rhs)
    {
        os << "{";
        for (int index = 0; index < size; index++) os << " " << rhs._v[index];
        os << " }";
        return os;
    }
};
