#ifndef ROPE_H
#define ROPE_H

#include <cstddef>
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
    typedef RopeIter<T> iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;

    Rope();

    Rope(const Rope<T>& src);
    
    Rope(const std::span<T>& src);
    
    ~Rope();

    inline void operator=(const Rope<T>& src);

    iterator begin() { return iterator(*this, 0);}
    iterator end()   { return iterator(*this, len);}

    unsigned int insert(unsigned int index, const std::span<T>& new_data);

    unsigned int insert(unsigned int index, const Rope<T>& new_data);

    unsigned int remove(unsigned int start, unsigned int len);

    Rope<T> subrope(unsigned int start, unsigned int end);

    T& at(unsigned int index) const;

    inline unsigned int weight() const;

    inline unsigned int length() const;

    inline T& operator[](unsigned int index) const;

protected:
    friend class RopeIter<T>;

    Rope<T> *L { nullptr };
    Rope<T> *R { nullptr };
    std::span<T> leaf = std::span<T>();
    unsigned int wgt { 0 };
    unsigned int len { 0 };

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
    RopeIter(Rope<T>& ropeArg, int length): myRope( ropeArg ), myPos( length )
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
Rope<T>::Rope()
{}

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
                Rope<T> *left_chunk   = new Rope<T>(leaf.subspan(0, index));
                Rope<T> *right_chunk  = new Rope<T>(leaf.subspan(index, leaf.size() - index));
                Rope<T> *middle_chunk = new Rope<T>(new_data);
                leaf = std::span<T>();
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
    if (!leaf.empty())
    {
        if ((start > 0) && ((start + len) >= leaf.size())) //remove right half/keep left
        {
            leaf = leaf.subspan(0, start);
        }
        else if (start == 0) //remove left/keep right
        {
            if ((start + len) < leaf.size())
            {
                leaf = leaf.subspan(len, leaf.size() - len);
            }
            else
            {
                leaf = std::span<T>();
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
            if (len >= wgt)
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
