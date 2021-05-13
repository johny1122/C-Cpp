#include <iostream>
#include <string>

class hangers
{
private:
int _numCoats;
std::string* _closet;
public:

    hangers(int numCoats) : _numCoats(numCoats), _closet{}
    {
        _closet = new std::string [numCoats];
    }

    hangers(hangers &rhs)
    {
        _numCoats = rhs._numCoats;
        _closet = new std::string [_numCoats];
        for (int index = 0; index < _numCoats; index++)
        {
            _closet[index] = rhs._closet[index];
        }
    }

    ~hangers()
    {
        delete [] _closet;
    }

    std::string operator[] (int index) const
    {
        std::string temp = _closet[index % _numCoats];
        return temp;
    }

    std::string& operator[] (int index)
    {
        return _closet[index % _numCoats];
    }

    explicit operator bool() const
    {
        for (int index = 0; index < _numCoats; index++)
        {
            if (_closet[index].empty())
            {
                return false;
            }
        }
        return true;
    }

    friend std::ostream& operator<< (std::ostream& stream, hangers &rhs);

};

std::ostream& operator<< (std::ostream& stream, hangers &rhs)
{
    for (int index = 0; index < rhs._numCoats; index++)
    {
        stream << index << ": " << rhs._closet[index] << std::endl;
    }
    return stream;
}