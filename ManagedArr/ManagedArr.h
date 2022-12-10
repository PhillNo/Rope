#ifndef MANAGED_ARR_H
#define MANAGED_ARR_H

#include <memory>
#include <stdexcept>

namespace phillno
{

enum class AllocationType {STATIC, DYNAMIC, ENUM_SIZE};

template <class T>
class ManagedArr
{
public:
    ManagedArr():length(0), offset(0){}

    ManagedArr(T *data, unsigned int len, AllocationType allocation): length(len), offset(0)
    {
        /*
        Source allocation of STATIC will not give the pointer to source char array to a shared pointer
        This will ensure no side effect of delete[]-ing the static source.

        Source allocation of DYNAMIC will give the pointer to the original char array to a shared pointer.
        Side effect: When all of the shared pointers go out of scope, they will delete[] the source.
        */

        //TODO: SourceAllocation::STATIC implementation

        if (data)
        {
            //this->length = length;

            if (allocation == AllocationType::DYNAMIC)
            {
                shared_buff = std::shared_ptr<T>(data);
                ptr_get = &phillno::ManagedArr<T>::get_shared_buff;
            }
            else if (allocation == AllocationType::STATIC)
            {
                static_buff = data;
                ptr_get = &phillno::ManagedArr<T>::get_static_buff;
            }
        }
        else
        {
            throw std::invalid_argument("nullptr not accepted in ManagedArr constructor.");
        }
    }

    ManagedArr(const ManagedArr& source, unsigned int start, unsigned int len): offset(start), length(len)
    {   
        /*
        Constructed ManagedArr will use the same head.
        Offset becomes src offset + offset. Len is provided len.
        
        All ManagedArr from the same shared_bugg share pointer to head of buff.
        New offsets are relative to source offset, not source shared_buff.
        */
    
        if (offset > source.length)
        {
            throw std::out_of_range("offset & length args OOR of source ManagedArr");
        }
        else
        {
            this->ptr_get     = source.ptr_get;
            this->static_buff = source.static_buff;
            this->shared_buff = source.shared_buff;
            //this->offset      = source.offset + offset;
            //this->length      = length;
        }
    }

    ~ManagedArr(){}

    unsigned int len() const
    {
        return length;
    }

    T get(unsigned int index) const
    {
        return (this->*ptr_get)(index);
    }

    T get_static_buff(unsigned int index) const
    {
        if (index < length)
        {
            return static_buff[offset + index];
        }
        else
        {
            throw std::out_of_range("requested index out of range. Index, offset, length.");
        }
    }

    T get_shared_buff(unsigned int index) const
    {
        if (index < length)
        {
            return shared_buff.get()[offset + index];
        }
        else
        {
            throw std::out_of_range("requested index out of range. Index, offset, length.");
        }
    }
    
    inline T operator[](unsigned int index) const
    {
	    return get(index);  
    }

    T (ManagedArr::*ptr_get) (unsigned int) const = nullptr;

protected:
    const unsigned int length;
    const unsigned int offset;
    T *static_buff = nullptr;
    std::shared_ptr<T> shared_buff;

};

}

#endif
