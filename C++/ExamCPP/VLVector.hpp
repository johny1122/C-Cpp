/**
 * @file VLVector.hpp
 * @author  Jonathan Birnbaum
 * @date 04/08/2020
 *
 * @brief VLVector class implementation
 */

#include <algorithm>
#include <stdexcept>


#define STATIC_CAPACITY_DEFAULT (16)
#define SIZE_ZERO (0)
#define DYNAMIC_CAPACITY ((3 * (_current_size + 1)) / 2)
#define DEFAULT_ISSTATICCAPACITY (-1)

/**
 * @brief exception messages
 */
#define OUT_OF_RANGE_MSG "Error: index is out of range"



/**
 * @brief VLVector class
 * @tparam T: generic element type
 * @tparam StaticCapacity: maximum static capacity of elements in VLVector
 */
template <class T, size_t StaticCapacity = STATIC_CAPACITY_DEFAULT>
class VLVector
{
private:
    size_t _static_capacity;
    size_t _capacity;
    size_t _current_size;
    bool _isStatic;
    T _vectorStatic[StaticCapacity];
    T* _vectorDynamic;

    /**
     * @brief Iterator class implementation
     */
    class Iterator
    {
    private:
        T* _cur;

    public:
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        // --------------- CONSTRUCTORS ---------------
        Iterator() : _cur(nullptr){}

        Iterator(pointer start) : _cur(start) {}

        Iterator(const Iterator &rhs) : _cur(rhs._cur) {}

        // --------------- READ & WRITE ---------------
        reference operator*() const {return *_cur; }

        // --------------- ITERATION ---------------
        Iterator& operator++() {++_cur; return *this; } // (Iterator++ prefix)

        Iterator operator++(int) {Iterator temp = *this; ++_cur; return temp; } // (Iterator++ postfix)

        Iterator& operator--() {--_cur; return *this; } // (Iterator-- prefix)

        Iterator operator--(int) {Iterator temp = *this; --_cur; return temp; } // (Iterator-- postfix)


        Iterator operator+(const difference_type& rhs) const {return Iterator(_cur + rhs); } // (Iterator + int)

        //(int + Iterator)
        friend Iterator operator+(const difference_type lhs, const Iterator& rhs) {return Iterator(rhs._cur + lhs); }


        difference_type operator-(const Iterator& rhs) const {return (_cur - rhs._cur); } //(Iterator - Iterator)

        Iterator operator-(const difference_type& rhs) const {return Iterator(_cur - rhs); } // (Iterator - int)

        //(int - Iterator)
        friend Iterator operator-(const difference_type lhs, const Iterator& rhs) {return Iterator(rhs._cur - lhs); }


        Iterator& operator+=(const difference_type& rhs) {_cur += rhs; return *this; } // (Iterator += int)

        Iterator& operator-=(const difference_type& rhs) {_cur -= rhs; return *this; } // (Iterator -= int)


        // --------------- COMPARISON ---------------
        bool operator==(const Iterator& rhs) const {return (_cur == rhs._cur); }

        bool operator!=(const Iterator& rhs) const {return (_cur != rhs._cur); }

        bool operator>(const Iterator& rhs) const {return (_cur > rhs._cur); }

        bool operator<(const Iterator& rhs) const {return (_cur < rhs._cur); }

        bool operator>=(const Iterator& rhs) const {return (_cur >= rhs._cur); }

        bool operator<=(const Iterator& rhs) const {return (_cur <= rhs._cur); }
    };

    /**
     * @brief ConstIterator class implementation
     */
    class ConstIterator
    {
    private:
        const T* _cur;

    public:
        typedef T value_type;
        typedef const T& reference;
        typedef const T* pointer;
        typedef int difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        // --------------- CONSTRUCTORS ---------------
        ConstIterator() : _cur(nullptr){}

        ConstIterator(pointer start) : _cur(start) {}

        ConstIterator(const ConstIterator& rhs) : _cur(rhs._cur) {}

        // --------------- READ & WRITE ---------------
         value_type operator*() const {return *_cur; }

        // --------------- ITERATION ---------------
        ConstIterator& operator++() {_cur++; return *this; } // (Iterator++ prefix)

        ConstIterator operator++(int) {ConstIterator temp = *this; _cur++; return temp; } // (Iterator++ postfix)

        ConstIterator& operator--() {_cur--; return *this; } // (Iterator-- prefix)

        ConstIterator operator--(int) {ConstIterator temp = *this; _cur--; return temp; } // (Iterator-- postfix)


        // (Iterator + int)
        ConstIterator operator+(const difference_type& rhs) const {return ConstIterator(_cur + rhs); }
        //(int + Iterator)
        friend ConstIterator operator+(const difference_type& lhs, const ConstIterator& rhs)
        {return ConstIterator(lhs + rhs._cur); }


        // (Iterator - Iterator)
        difference_type operator-(const ConstIterator& rhs) const {return (_cur - rhs._cur); }
        // (Iterator - int)
        ConstIterator operator-(const difference_type& rhs) const {return ConstIterator(_cur - rhs); }
        //(int - Iterator)
        friend ConstIterator operator-(const difference_type& lhs, const ConstIterator& rhs)
        {return ConstIterator(lhs - rhs._cur); }


        ConstIterator& operator+=(const difference_type& rhs) {_cur += rhs; return *this; } // (Iterator += int)

        ConstIterator& operator-=(const difference_type& rhs) {_cur -= rhs; return *this; } // (Iterator -= int)


        // --------------- COMPARISON ---------------
        bool operator==(const ConstIterator& rhs) const {return (_cur == rhs._cur); }

        bool operator!=(const ConstIterator& rhs) const {return (_cur != rhs._cur); }

        bool operator>(const ConstIterator& rhs) const {return (_cur > rhs._cur); }

        bool operator<(const ConstIterator& rhs) const {return (_cur < rhs._cur); }

        bool operator>=(const ConstIterator& rhs) const {return (_cur >= rhs._cur); }

        bool operator<=(const ConstIterator& rhs) const {return (_cur <= rhs._cur); }
    };


     /**
     * @brief checks if static capacity is needed or not
      * @param size: size_t to check with
     * @return: true if needed and false if not
      */
    bool _isStaticCapacity(size_t size = DEFAULT_ISSTATICCAPACITY)
    {
        if (size == (size_t) DEFAULT_ISSTATICCAPACITY)
        {
            return (_current_size <= _static_capacity);
        }
        return (size <= _static_capacity);
    }

    /**
     * @brief change the private methods to static allocation status
     */
    void _changeToStatic()
    {
        _capacity = _static_capacity;
        _isStatic = true;
    }


public:

    // ------------------------ CONSTRUCTORS & DESTRUCTOR ------------------------
    /**
     * @brief default constructor
     */
    VLVector() : _static_capacity{StaticCapacity}, _capacity{StaticCapacity}, _current_size{SIZE_ZERO},
                 _isStatic{true}, _vectorStatic{}, _vectorDynamic{nullptr} {}


    /**
     * @brief constructor 1
     */
    template<class InputIterator>
    VLVector(InputIterator first, InputIterator last) : _static_capacity{StaticCapacity}, _capacity{},
                                                          _current_size{}, _isStatic{}, _vectorStatic{},
                                                          _vectorDynamic{}
    {
        _current_size = std::distance(first, last);

        if (_isStaticCapacity())
        {
            _isStatic = true;
            _capacity = _static_capacity;
            std::copy(first, last, this->begin());
        }
        else
        {
            _isStatic = false;
            _capacity = DYNAMIC_CAPACITY;
            _vectorDynamic = new T [_capacity];
            std::copy(first, last, this->begin());
        }
    }


    /**
     * @brief copy constructor
     */
    VLVector(const VLVector<T>& rhs) : _static_capacity{rhs._static_capacity}, _capacity{rhs.capacity()},
                                    _current_size{rhs.size()}, _isStatic{rhs._isStatic}, _vectorStatic{},
                                    _vectorDynamic{}
    {
        if (!_isStatic)
        {
            _vectorDynamic = new T [_capacity];
        }
        std::copy(rhs.begin(), rhs.end(), this->begin());

    }


    /**
     * @brief destructor
     */
    ~VLVector()
    {
        delete [] _vectorDynamic;
    }

    // ------------------------ METHODS ------------------------

    /**
     * @brief getter size
     * @return current size of the vector
     */
    size_t size() const {return _current_size; }

    /**
     * @brief getter capacity
     * @return the capacity according to the CAPC
     */
    size_t capacity() const {return _capacity; }

    /**
     * @brief checks if the vector is empty
     * @return true if empty, else if not
     */
    bool empty() const {return (_current_size == SIZE_ZERO); }

    /**
     * @brief return the element at the given index. if index is out of range, throw exception
     * @param index: given index in the vector
     * @return element at the index
     */
    T& at(const size_t index)
    {
        if ((index < SIZE_ZERO) || (index >= _current_size))
        {
            throw std::out_of_range(OUT_OF_RANGE_MSG);
        }
        if (_isStatic)
        {
            return _vectorStatic[index];
        }
        return _vectorDynamic[index];
    }


    /**
     * @brief gets an element and adds it to the vector. if size == capacity: does new dynamic allocation
     * @param input: element to add to the vector
     */
    void push_back(const T& input)
    {
        if (_current_size == _capacity) // if size == capacity : new alloc is for sure dynamic
        {
            _capacity = DYNAMIC_CAPACITY;
            T* temp = new T [_capacity];
            std::copy(this->begin(), this->end(), Iterator(temp));
            delete [] _vectorDynamic;
            _vectorDynamic = temp;
            _isStatic = false;
            _vectorDynamic[_current_size] = input;
            _current_size++;
        }
        else
        {
            (_isStatic) ? _vectorStatic[_current_size] = input : _vectorDynamic[_current_size] = input;
            _current_size++;
        }
    }


    /**
     * @brief insert (1)
     *        insert an element before the position
     * @param position: iterator to add the new element before
     * @param val: value of the new element from variable T
     * @return iterator of the new added element
     */
    Iterator insert(Iterator position, const T& val)
    {
        T* temp;
        int elements_before = std::distance(this->begin(), position); // number of elements before position

        if (_current_size == _capacity) // if size == capacity : new alloc is for sure dynamic
        {
            _capacity = DYNAMIC_CAPACITY;
            temp = new T [_capacity]; // create new dynamic vector

            // copy all element before position to temp
            std::copy(this->begin(), position, Iterator(temp));
            //copy all elements from position to end into temp[position + 1]
            std::copy(position, this->end(), Iterator(temp + elements_before + 1));
            *position = val;
            delete [] _vectorDynamic;
            _vectorDynamic = temp;
            _isStatic = false;
        }
        else
        {
            std::move_backward(position, this->end(), this->end() + 1);
            *position = val;
        }
        _current_size++;
        return (this->begin() + elements_before);
    }



    /**
     * @brief insert (2)
     *        insert a range of elements before the given position
     * @tparam InputIterator: Iterator of elements
     * @param position: Iterator of the element to insert before
     * @param first: Iterator of the beginning of the input range elements
     * @param last: Iterator of the end of the input range elements
     * @return Iterator of the first element that was added
     */
    template<class InputIterator>
    Iterator insert(Iterator position, InputIterator first, InputIterator last)
    {
        int input_size = std::distance(first, last);
        int elements_before = std::distance(this->begin(), position); // number of elements before position
        _current_size += input_size;
        T* temp;
        if (_current_size >= _capacity) // need new allocation: for sure dynamic
        {
            _capacity = DYNAMIC_CAPACITY;
            _isStatic = false;
            temp = new T [_capacity]; // create new dynamic vector

            // copy all element before position to temp
            std::copy(this->begin(), position, Iterator(temp));
            //copy all elements from position to end into temp
            std::copy(position, this->end(), Iterator(temp + elements_before + input_size));
            std::copy(first, last, Iterator(temp + elements_before));

            delete [] _vectorDynamic;
            _vectorDynamic = temp;
        }
        else // no need for new allocation (same capacity)
        {
            std::move_backward(position, this->end(), (this->end() + input_size));
            std::copy(first, last, position);
        }
        return (this->begin() + elements_before);
    }


    /**
     * @brief removes the last element in the vector.
     * changes the vector to static memory if needed.
     * if size is zero do nothing.
     */
    void pop_back()
    {
        if (_current_size == SIZE_ZERO) // no element to pop
        {
            return;
        }

        if (_isStaticCapacity(_current_size - 1)) // if vector is dynamic and now needs to be static
        {
            for (size_t index = 0; index < (_current_size - 1); index++)
            {
                _vectorStatic[index] = _vectorDynamic[index];
            }
            _changeToStatic();
        }
        _current_size--;
    }





    /**
     * @brief erase (2)
     *        removes all the elements in the range of first to last.
     *        if needed, change allocation and capacity to static.
     * @param first: Iterator of the first element to remove from range
     * @param last: Iterator of the last element to remove from range
     * @return Iterator to the element after the last iterator
     */
    Iterator erase(Iterator first, Iterator last)
    {
        int number_elements_before = std::distance(this->begin(), first);
        int number_elements_delete = std::distance(first, last);

        // if vector is dynamic and now needs to be static
        if (!_isStatic && _isStaticCapacity(_current_size - number_elements_delete))
        {
            std::copy(this->begin(), first, Iterator(_vectorStatic));
            std::copy(last, this->end(), Iterator(_vectorStatic + number_elements_before));
            _changeToStatic();
        }
        else // vector is dynamic/static and still dynamic/static
        {
            std::move(last, this->end(), first);

        }
        _current_size -= number_elements_delete;
        return (this->begin() + number_elements_before);
    }

    /**
     * @brief erase (1)
     *        removes an element from the vector in the given position
     * @param position: Iterator of the element to remove
     * @return Iterator to the element after position
     */
    Iterator erase(Iterator position) {return erase(position, position + 1); }


    /**
     * @brief clear all the vector from elements
     */
    void clear()
    {
        if (!_isStatic) // if vector is dynamic
        {
            _changeToStatic();
        }
        _current_size = 0;
    }


    /**
     * @brief returns a direct pointer to the memory array
     * @return a pointer to the first element in the array
     */
    T* data() {return (_isStatic ? _vectorStatic : _vectorDynamic); }

    /**
     * @brief returns a direct pointer to the memory array
     * @return a pointer to the first element in the array
     */
    T* data() const {return (_isStatic ? _vectorStatic : _vectorDynamic); }


// ------------------------ ITERATOR ------------------------

    /**
     * @brief iterator of the beginning of the vector
     * @return an iterator
     */
    Iterator begin() {return (_isStatic) ? Iterator(_vectorStatic) : Iterator(_vectorDynamic); }

    /**
     * @brief iterator of the end of the vector (nullptr)
     * @return an iterator
     */
    Iterator end()
    {
        return (_isStatic) ? Iterator(_vectorStatic + _current_size) : Iterator(_vectorDynamic + _current_size);
    }

    /**
     * @brief const iterator of the beginning of the vector
     * @return a const iterator
     */
    ConstIterator begin() const {return (_isStatic) ? ConstIterator(_vectorStatic) : ConstIterator(_vectorDynamic); }

    /**
     * @brief const iterator of the end of the vector (nullptr)
     * @return a const iterator
     */
    ConstIterator end() const
    {
        return (_isStatic) ? ConstIterator(_vectorStatic + _current_size) : ConstIterator (_vectorDynamic +
        _current_size);
    }

    /**
     * @brief const iterator of the beginning of the vector
     * @return a const iterator
     */
    ConstIterator cbegin() const {return (_isStatic) ? ConstIterator(_vectorStatic) : ConstIterator(_vectorDynamic); }

    /**
     * @brief const iterator of the end of the vector (nullptr)
     * @return a const iterator
     */
    ConstIterator cend() const
    {
        return (_isStatic) ? ConstIterator(_vectorStatic + _current_size) : ConstIterator
                (_vectorDynamic + _current_size);
    }


    // ------------------------ OPERATORS ------------------------

    /**
     * @brief operator = (rhs VLVector)
     * @return *this
     */
    VLVector& operator=(const VLVector& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        _current_size = rhs.size();
        _isStatic = rhs._isStatic;
        _capacity = rhs.capacity();
        if (!rhs._isStatic)
        {
            delete [] _vectorDynamic;
            _vectorDynamic = new T [_capacity];

        }
        std::copy(rhs.begin(), rhs.end(), this->begin());
        return *this;
    }


    /**
     * @brief returns a reference to the element in the index position
     * @param index: position to return element from
     * @return reference of T
     */
    T& operator[](size_t index) noexcept {return (_isStatic) ? _vectorStatic[index] : _vectorDynamic[index]; }

    /**
     * @brief returns a reference to the element in the index position
     * @param index: position to return element from
     * @return reference of T
     */
    T operator[](size_t index) const noexcept {return (_isStatic) ? _vectorStatic[index] : _vectorDynamic[index]; }


    /**
     * @brief operator ==
     * @param rhs: VLVector to compare with
     * @return true if they have the same elements and false otherwise
     */
    bool operator==(const VLVector& rhs) const
    {
        if (_current_size != rhs.size())
        {
            return false;
        }
        if (_isStatic)
        {
            for (size_t i = 0; i < _current_size; i++)
            {
                if (_vectorStatic[i] != rhs._vectorStatic[i])
                {
                    return false;
                }
            }
        }
        else
        {
            for (size_t i = 0; i < _current_size; i++)
            {
                if (_vectorDynamic[i] != rhs._vectorDynamic[i])
                {
                    return false;
                }
            }
        }
        return true;
    }


    /**
     * @brief operator !=
     * @param rhs: VLVector to compare with
     * @return false if they have the same elements and true otherwise
     */
    bool operator!=(const VLVector& rhs) const {return (!(this == &rhs)); }

};

