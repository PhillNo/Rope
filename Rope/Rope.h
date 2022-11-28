#ifndef ROPE_H
#define ROPE_H

#include <iostream>
#include <stdexcept>
#include "ManagedArr.h"

namespace phillno
{
template <class T>
class Rope
{
public:
    Rope(){}
    
    Rope(const ManagedArr<T>& sourceArr)
    {
        leaf   = new ManagedArr<T>(sourceArr);
        weight = sourceArr.len();
        length = weight;
    }

    Rope(const ManagedArr<T>* sourceArr)
    {
        leaf   = new ManagedArr<T>(sourceArr);
        weight = sourceArr->len();
        length = weight;
    }

    Rope(const Rope<T>& src)
    {
        if (src.leaf) leaf = new ManagedArr<T>(*src.leaf);        
        if (src.L) L       = new Rope(*src.L);
        if (src.R) R       = new Rope(*src.R);
        weight = src.weight;
        length = src.length;
    }

    Rope(const Rope<T>* src)
    {
        if (src->leaf) leaf = new ManagedArr<T>(*src.leaf);        
        if (src->L) L       = new Rope(*src.L);
        if (src->R) R       = new Rope(*src.R);
        weight = src->weight;
        length = src->length;
    }
    
    ~Rope()
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

    unsigned int insert(unsigned int index, const ManagedArr<T> newData)
    {
        if (index > length)
        {
            throw std::out_of_range("OOR: max index for insertion is Rope.length");
        }
        else
        {
            if (index >= weight)
            {
                if (R)
                {
                    R->insert(index - weight, newData);
                }
                else
                {
                    R = new Rope<T>(newData);
                }
            }
            else
            {
                if (L)
                {
                    L->insert(index, newData);
                }
                else
                {
                    ManagedArr<T> fc = ManagedArr<T>(*leaf, 0, index);
                    ManagedArr<T> sc = ManagedArr<T>(*leaf, index, leaf->len() - index);//index, leaf->get_length() - index);
                    delete leaf;
                    leaf = nullptr;

                    L = new Rope<T>(fc);
                    L->R = new Rope<T>(newData);
                    L->R->R = new Rope<T>(sc);
                }
            }
        }
        calc_length();
        return length;
    }

    unsigned int remove(unsigned int start, unsigned int end);
    
    T get(unsigned int index) const
    {
        if (index >= length)
        {
            throw std::out_of_range("Rope peek index OOR");
        }
        else
        {

            if (index >= weight)
            {
                return R->get(index - weight);
            }
            if (index < weight)
            {
                if (L)
                {
                    return L->get(index);
                }
                else if (leaf)
                {
                    return leaf->get(index);
                }
            }
        }
        throw std::logic_error("error in Rope.peek() implementation.");
    }

    unsigned int wgt() const
    {
        return weight;
    }

    unsigned int len() const
    {
        return length;
    }

    inline T operator[](unsigned int index) const
    {
	    return get(index);  
    }

protected:
    Rope<T> *L = nullptr;
    Rope<T> *R = nullptr;
    ManagedArr<T>* leaf = nullptr;
    unsigned int weight = 0;
    unsigned int length = 0;

    unsigned int calc_length()
    {
        if (leaf)
        {
            weight = leaf->len();
            length = weight;
            if (R)
            {
                length += R->calc_length();
            }
        }
        else if (L)
        {
            weight = L->calc_length();
            length = weight;
            if (R)
            {
                length += R->calc_length();
            }
        }

        return length;
    }

};

}

#endif