/*

Rope.h
Rope class
RopeIter class
Supporting functions

Author: PhillNo

Date: Spring 2023

Description:
A Rope is a template container for variable sized collections of data. It's purpose is to store
data in a non-contiguous memory allocation. The underlying implementation is a binary tree.

Leaf nodes of the tree contain a std::span where instances of T are stored. Multiple std::span objects can
point to the same array of T. This allows fast reuse of substrings and inherent compression.

Insertion, deletion, peek take place in O(logn) time complexity.

*/

#ifndef ROPE_H
#define ROPE_H

#include <iostream>
#include <iterator>
#include <span>
#include <stdexcept>

namespace phillno
{

template <class T> class RopeIter;

template <class T>
class Rope
{
public:
    // typedefs necessary to some STL functions
    typedef RopeIter<T> iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;

    Rope();

    Rope(const Rope<T>& src);
    
    Rope(const std::span<T>& src);

    //Rope(T);
    
    ~Rope();

    // Returned iterator objects used by 
    // STL and implicitly by range-based loops
    iterator begin() { return iterator(*this, 0);}
    iterator end()   { return iterator(*this, len);}

    // returns size of Rope instance after modification
    unsigned int insert(unsigned int index, const std::span<T>& new_data);

    unsigned int insert(unsigned int index, const Rope<T>& new_data);

    //unsigned int insert(unsigned int index, T val);

    //returns size of Rope instance after modification
    unsigned int remove(unsigned int start, unsigned int len);

    // Generates a new Rope that contains element spanned by start & end
    Rope<T> subrope(unsigned int start, unsigned int end);

    // Returns reference to element at indez
    T& at(unsigned int index) const;

    // Returns the number of elements to the left of root
    // in the tree structure implemented by Rope
    inline unsigned int weight() const;

    // Returns the total number of elements below root
    // in the tree structure implemented by Rope
    inline unsigned int length() const;

    // Assignment operator used by copy constructor
    inline void operator=(const Rope<T>& src);

    // Shorthand for Rope::at
    inline T& operator[](unsigned int index) const;

protected:
    // Rope elements must be exposed to RopeIter
    friend class RopeIter<T>;

    // Left & right branches of tree structure
    Rope<T> *L { nullptr };
    Rope<T> *R { nullptr };

    // leaf contains the data elements in the Rope collection
    // If the leaf is non-empty then L & R MUST BE NULL
    std::span<T> leaf = std::span<T>();

    // Elements to left of root
    unsigned int wgt { 0 };

    // Elements to right of root
    unsigned int len { 0 };

    // calc_length must be called any time a Rope is modified
    // or if changes to a subrope must propagate up
    unsigned int calc_length();

};

// Feed T elements into a std::ostream
// May not be compatible with all T
template<class T>
std::ostream& operator<<(std::ostream& os, const Rope<T>& iterand)
{
    for (unsigned int i = 0; i < iterand.length(); ++i)
    {
        os << iterand[i];
    }

    return os;
}

template <class T>
class RopeIter
{
public:
    RopeIter(Rope<T>& rope_target, int length): iterand( rope_target ), pos( length )
    {}

    bool operator==(RopeIter<T>& comparand)
    {
        return (&iterand == &(comparand.iterand)) && (pos == comparand.pos);
    }

    bool operator!=(RopeIter<T>& comparand)
    {
        return !operator==(comparand);
    }

    T& operator*() { return iterand[pos]; }

    RopeIter<T>& operator++() { pos++; return *this; }
    
    RopeIter<T>& operator--() { pos++; return *this; }

    //post increment operator clones "this" in order to return unmodified version
    RopeIter<T> operator++( int ) {RopeIter<T> clone(*this); ++pos; return clone;}

    RopeIter<T> operator--( int ) {RopeIter<T> clone(*this); --pos; return clone;}

protected:
    Rope<T>& iterand; // Rope which will be iterated over
    unsigned int pos; // position in Rope iteration
};

template <class T>
Rope<T>::Rope()
{}

template <class T>
Rope<T>::Rope(const Rope<T>& src)
{
    // Copying a Rope from a Rope creates new independent tree

    wgt = src.wgt;
    len = src.len;

    if (src.L)
    {
        L = new Rope<T>(*src.L);
    }

    if (src.R)
    {
        R = new Rope<T>(*src.R);
    }

    leaf = src.leaf;

}

template <class T>
Rope<T>::Rope(const std::span<T>& source_arr)
{
    leaf = source_arr;
    wgt = source_arr.size();
    len = wgt;
}

template <class T>
Rope<T>::~Rope()
{
    if (!L)
    {
        delete L;
    }
    if (!R)
    {
        delete R;
    }
}

template <class T>
inline void Rope<T>::operator=(const Rope<T>& src)
{
    if (this != &src)
    {
        if (L)
        {
            delete L;
            L = nullptr;
        }
        if (R)
        {
            delete R;
            R = nullptr;
        }
        
        len = src.len;
        wgt = src.wgt;
        leaf = src.leaf;

        if (src.L)
        {
            L = new Rope<T>(*src.L);
        }
        if (src.R)
        {
            R = new Rope<T>(*src.R);
        }
        
    }
}

template <class T>
unsigned int Rope<T>::insert(unsigned int index, const std::span<T>& new_data)
{
    Rope<T> new_rope(new_data);

    return insert(index, new_rope);
}

template <class T>
unsigned int Rope<T>::insert(unsigned int index, const Rope<T>& new_data)
{
    // Always call calc_length when modifying Rope
    // A Rope with a leaf should never have L or R branches

    if (index >= len)
    {
        if (index == len)
        {
            if (R)
            {
                R->insert(index - wgt, new_data);
            }
            else
            {
                if (L)
                {
                    R = new Rope<T>(new_data);
                }
                else
                {
                    L = new Rope<T>(leaf);
                    R = new Rope<T>(new_data);
                    leaf = std::span<T>();
                }
            }
        }
        else
        {
            throw std::out_of_range("OOR: max index for insertion is Rope.len");
        }
    }
    else
    {
        if (index >= wgt)
        {
            if (R)
            {
                R->insert(index - wgt, new_data);
            }
            else
            {
                R = new Rope<T>(new_data);
            }
        }
        else
        {
            if (L)
            {
                L->insert(index, new_data);
            }
            else
            {
                Rope<T> *left_chunk   = new Rope<T>(leaf.subspan(0, index));
                Rope<T> *right_chunk  = new Rope<T>(leaf.subspan(index, leaf.size() - index));
                Rope<T> *middle_chunk = new Rope<T>(new_data);
                leaf = std::span<T>();
                
                L = new Rope<T>();
                L->L = left_chunk;
                L->R = middle_chunk;
                R = right_chunk;
            }
        }
    }

    calc_length();
    return len;
}

template <class T>
unsigned int Rope<T>::remove(unsigned int start, unsigned int len)
{
    // Always call calc_length when modifying Rope
    // A Rope with a leaf should never have L or R branches

    if (!leaf.empty())
    {
        if ((start > 0) && ((start + len) >= leaf.size())) //remove right half/keep left
        {
            leaf = leaf.subspan(0, start);
        }
        else if (start == 0) //remove left half/keep right right
        {
            if ((start + len) < leaf.size())
            {
                leaf = leaf.subspan(len, leaf.size() - len);
            }
            else
            {
                leaf = std::span<T>(); //Remove whole leaf. (Parent should delete this Rope.)
            }
        }
        else if ((start > 0) && ((start + len) < leaf.size())) //remove middle
        {
            std::span<T> left_chunk  = leaf.subspan(0, start);
            std::span<T> right_chunk = leaf.subspan(start + len, leaf.size() - (start + len));

            L = new Rope<T>(left_chunk);
            R = new Rope<T>(right_chunk);
            leaf = std::span<T>();
        }
    }

    else
    {
        if (start == 0)
        {
            if (len >= this->len)
            {
                throw std::out_of_range("Indexing beyond (sub)Rope length.");
            }
            else if (len >= wgt)
            {
                if (L)
                {
                    delete L;
                }
                L = R;
                R = nullptr;
                L->remove(0, len - wgt);
            }
            else
            {
                L->remove(0, len);
            }
        }
        if (start > 0)
        {
            if (start >= wgt) //and/or len in body
            {
                if (R)
                {
                    if (len >= R->length()) //efficiency gain to prevent empty Rope. Not critical.
                    {
                        delete R;
                        R = nullptr;
                    }
                    else
                    {
                        R->remove(start - wgt, len);
                    }
                }
                else
                {
                    throw std::out_of_range("start index greater than rope wgt and no R subrope.");
                }
            }
            else
            {
                if ((start + len) <= wgt)
                {
                    L->remove(start, len);
                }
                else
                {
                    L->remove(start, wgt - start);
                    R->remove(0, len - (wgt - start));
                }
            }
        }
    }

    calc_length();
    return len;
}

template <class T>
T& Rope<T>::at(unsigned int index) const
{    
    if (index >= len)
    {
        throw std::out_of_range("Rope.at index OOR");
    }
    else
    {

        if (index >= wgt)
        {
            return R->at(index - wgt);
        }
        if (index < wgt)
        {
            if (L)
            {
                return L->at(index);
            }
            else if (!leaf.empty())
            {
                return leaf[index];
            }
        }
    }
    throw std::logic_error("error in Rope.at() implementation.");
}

template <class T>
inline unsigned int Rope<T>::weight() const
{
    return wgt;
}

template <class T>
inline unsigned int Rope<T>::length() const
{
    return len;
}

template <class T>
inline T& Rope<T>::operator[](unsigned int index) const
{
    return at(index);
}

template <class T>
unsigned int Rope<T>::calc_length()
{
    // calculates weight and length
    // returns len since insert & remove
    // don't need weight of subtrees
    
    wgt = 0;
    len = 0;

    if (leaf.size() != 0)
    {
        wgt = leaf.size();
        len = wgt;
    }
    else if (L)
    {
        wgt = L->calc_length();
        len = wgt;
        if (R)
        {
            len += R->calc_length();
        }
    }

    return len;
}

}

#endif
