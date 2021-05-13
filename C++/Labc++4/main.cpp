#include <iostream>
#include <map>
#include <string>


using std::map;
using std::string;

class AntonymFinder
{
private:
    std::map<string ,string> _dictionary;

public:
    void readDictionary (std::istream& is)
    {
        string firstWord;
        string secondWord;
        while (is >> firstWord >> secondWord)
        {
            _dictionary[firstWord] = secondWord;
            _dictionary[secondWord] = firstWord;
        }
    }

    string findAntonym (const string& word) const
    {
        if (_dictionary.find(word) == _dictionary.end())
        {
            return "Antonym not found for word: " + word;
        }
        return _dictionary.at(word);
    }
};

