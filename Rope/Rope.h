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

    Rope(const Rope<T>& src)
    {
        weight = src.weight;
        length = src.length;

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
    
    Rope(const ManagedArr<T> source_arr)
    {
        leaf   = new ManagedArr<T>(source_arr);
        weight = source_arr.len();
        length = weight;
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

    unsigned int insert(unsigned int index, const ManagedArr<T> new_data)
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
                    R->insert(index - weight, new_data);
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
                    R = new Rope<T>();
                    R->L = middle_chunk;
                    R->R = right_chunk;
                }
            }
        }
        calc_length();
        return length;
    }

    unsigned int remove(unsigned int start, unsigned int len)
    {
        if (leaf)
        {
            if ((start > 0) && ((start + len) >= leaf->len()))
            {
                ManagedArr<T> *new_leaf = new ManagedArr<T>(*leaf, 0, start);
                delete leaf;
                leaf = new_leaf;
            }
            else if ((start == 0) && ( (start + len) <= leaf->len()) )
            {
                ManagedArr<T> *new_leaf = new ManagedArr<T>(*leaf, len, leaf->len() - len);
                delete leaf;
                leaf = new_leaf;
            }
            else if ( (start > 0) && ((start + len) < leaf->len()) )
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
                if (len >= weight)
                {
                    delete L;
                    L = R;
                    R = nullptr;
                    L->remove(0, len - weight);
                }
                else
                {
                    L->remove(0, len);
                }
            }
            if (start > 0)
            {
                if (start >= weight)
                {
                    if (R)
                    {
                        R->remove(start - weight, len);
                    }
                    else
                    {
                        throw std::out_of_range("start index greater than rope weight and no R subrope.");
                    }
                }
                else
                {
                    if ((start + len) <= weight)
                    {
                        L->remove(start, len);
                    }
                    else
                    {
                        L->remove(start, weight - start);
                        R->remove(0, len - (weight - start));
                    }
                }
            }

        }
        
        calc_length();
        return length;
    }

    T at(unsigned int index) const
    {
        if (index >= length)
        {
            throw std::out_of_range("Rope.at index OOR");
        }
        else
        {

            if (index >= weight)
            {
                return R->at(index - weight);
            }
            if (index < weight)
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
	    return at(index);  
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
