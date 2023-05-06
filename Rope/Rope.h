#ifndef ROPE_H
#define ROPE_H

#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include "ManagedArr.h"

namespace phillno
{

template <class T> class RopeIter;

template <class T>
class Rope
{
public:
    typedef RopeIter<T> iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;

    Rope();

    Rope(const Rope<T>& src);
    
    Rope(const ManagedArr<T>& source_arr);
    
    ~Rope();

    inline Rope<T>& operator=(const Rope<T>& src);

    iterator begin() { return iterator(*this, 0);}
    iterator end()   { return iterator(*this, len);}

    unsigned int insert(unsigned int index, const ManagedArr<T>& new_data);

    unsigned int insert(unsigned int index, const Rope<T>& new_data);

    unsigned int remove(unsigned int start, unsigned int len);

    T& at(unsigned int index) const;

    inline unsigned int weight() const;

    inline unsigned int length() const;

    inline T& operator[](unsigned int index) const;

protected:
    friend class RopeIter<T>;

    //friend std::ostream& operator<<(std::ostream& os, const Rope<T>& myRope);

    Rope<T> *L = nullptr;
    Rope<T> *R = nullptr;
    ManagedArr<T>* leaf = nullptr;
    unsigned int wgt = 0;
    unsigned int len = 0;

    unsigned int calc_length();

};

template<class T>
std::ostream& operator<<(std::ostream& os, const Rope<T>& myRope)
{
    for (unsigned int i = 0; i < myRope.length(); ++i)
    {
        os << myRope[i];
    }

    return os;
}

template <class T>
class RopeIter
{
public:
    RopeIter(Rope<T>& ropeArg, int size): myRope( ropeArg ), myPos( size )
    {}

    bool operator==(RopeIter<T>& comparee)
    {
        return (&myRope == &(comparee.myRope)) && (myPos == comparee.myPos);
    }

    bool operator!=(RopeIter<T>& comparee)
    {
        return !operator==(comparee);
    }

    T& operator*() { return myRope[myPos]; }

    RopeIter<T>& operator++() { myPos++; return *this; }
    
    RopeIter<T>& operator--() { myPos++; return *this; }

    RopeIter<T> operator++( int ) {RopeIter<T> clone(*this); ++myPos; return clone;}

    RopeIter<T> operator--( int ) {RopeIter<T> clone(*this); --myPos; return clone;}



protected:
    Rope<T>& myRope;
    unsigned int myPos;
};

template <class T>
Rope<T>::Rope(){}

template <class T>
Rope<T>::Rope(const Rope<T>& src)
{
    wgt = src.wgt;
    len = src.len;

    if (src.L)
    {
        L = new Rope<T>(*src.L);
    }

    if (src.R)
    {
        L = new Rope<T>(*src.L);
    }

    if (src.leaf)
    {
        leaf = new ManagedArr<T>(*src.leaf);
    }

}

template <class T>
Rope<T>::Rope(const ManagedArr<T>& source_arr)
{
    leaf   = new ManagedArr<T>(source_arr);
    wgt = source_arr.len();
    len = wgt;
}

template <class T>
Rope<T>::~Rope()
{
    if (!leaf)
    {
        delete leaf;
    }
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
inline Rope<T>& Rope<T>::operator=(const Rope<T> &src)
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
        if (leaf)
        {
            delete leaf;
            leaf = nullptr;
        }

        len = src.len;
        wgt = src.wgt;

        if (src.L)
        {
            L = new Rope<T>(*src.L);
        }
        if (src.R)
        {
            R = new Rope<T>(*src.R);
        }
        if (src.leaf)
        {
            leaf = new ManagedArr<T>(*src.leaf);
        }
    }

    return *this;
}

template <class T>
unsigned int Rope<T>::insert(unsigned int index, const ManagedArr<T>& new_data)
{
    if (index > len)
    {
        throw std::out_of_range("OOR: max index for insertion is Rope.len");
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
                Rope<T> *left_chunk = new Rope<T>(*(new ManagedArr<T>(*leaf, 0, index)));
                Rope<T> *right_chunk = new Rope<T>(*(new ManagedArr<T>(*leaf, index, leaf->len() - index)));
                Rope<T> *middle_chunk = new Rope<T>(*(new ManagedArr<T>(new_data)));
                delete leaf;
                leaf = nullptr;

                L = new Rope<T>();
                L->L = left_chunk;
                L->R = middle_chunk;
                R = new Rope<T>();
                R->L = right_chunk;
            }
        }
    }
    calc_length();
    return len;
}

template <class T>
unsigned int Rope<T>::insert(unsigned int index, const Rope<T>& new_data)
{
    if (index > len)
    {
        throw std::out_of_range("OOR: max index for insertion is Rope.len");
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
                Rope<T> *left_chunk   = new Rope<T>(*(new ManagedArr<T>(*leaf, 0, index)));
                Rope<T> *right_chunk  = new Rope<T>(*(new ManagedArr<T>(*leaf, index, leaf->len() - index)));
                Rope<T> *middle_chunk = new Rope<T>(new_data);
                delete leaf;
                leaf = nullptr;
                L = new Rope<T>();
                L->L = left_chunk;
                L->R = middle_chunk;
                R = new Rope<T>();
                R->L = right_chunk;
            }
        }
    }
    calc_length();
    return len;
}

template <class T>
unsigned int Rope<T>::remove(unsigned int start, unsigned int len)
{
    if (leaf)
    {
        if ((start > 0) && ((start + len) >= leaf->len()))
        {
            ManagedArr<T> *new_leaf = new ManagedArr<T>(*leaf, 0, start);
            delete leaf;
            leaf = new_leaf;
        }
        else if ((start == 0) && ((start + len) <= leaf->len()))
        {
            ManagedArr<T> *new_leaf = new ManagedArr<T>(*leaf, len, leaf->len() - len);
            delete leaf;
            leaf = new_leaf;
        }
        else if ((start > 0) && ((start + len) < leaf->len()))
        {
            Rope<T> *new_L = new Rope<T>();

            ManagedArr<T> *left_chunk = new ManagedArr<T>(*leaf, 0, start);
            ManagedArr<T> *right_chunk = new ManagedArr<T>(*leaf, start + len, leaf->len() - (start + len));

            new_L->L = new Rope<T>(*left_chunk);
            new_L->R = new Rope<T>(*right_chunk);
            L = new_L;
            delete leaf;
            leaf = nullptr;
        }
    }

    else
    {
        if (start == 0)
        {
            if (len >= wgt)
            {
                delete L;
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
            if (start >= wgt)
            {
                if (R)
                {
                    R->remove(start - wgt, len);
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
            else if (leaf)
            {
                return leaf->at(index);
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
    if (leaf)
    {
        wgt = leaf->len();
        len = wgt;
        if (R)
        {
            len += R->calc_length();
        }
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
