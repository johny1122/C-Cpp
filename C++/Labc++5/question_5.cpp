#include <iostream>

using std::iterator;

template <class ForwardIterator, class OutputIterator, class T>

OutputIterator replace_copy_my_version (ForwardIterator start, ForwardIterator end, OutputIterator result, const T&
old_value, const T& new_value)
{
    for (auto iter = start; iter != end; iter++,result++)
    {
        *result = (*iter == old_value) ? new_value : *iter;
    }
    return result;
}
