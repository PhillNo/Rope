#ifndef MANAGED_ARR_H
#define MANAGED_ARR_H

#include <memory>
#include <stdexcept>

namespace phillno
{

enum class SourceAllocation {STATIC, DYNAMIC, ENUM_SIZE};

template <class T>
class ManagedArr
{
public:
    ManagedArr(){}

    ManagedArr(T *data, unsigned int length, SourceAllocation allocation)
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
            this->length = length;

            if (allocation == SourceAllocation::DYNAMIC)
            {
                shared_buff = std::shared_ptr<T>(data);
                get_ptr = &phillno::ManagedArr<T>::get_shared_buff;
            }
            else if (allocation == SourceAllocation::STATIC)
            {
                raw_buff = data;
                get_ptr = &phillno::ManagedArr<T>::get_raw_buff;
            }
        }
    }

    ManagedArr(const ManagedArr& source, unsigned int offset, unsigned int length)
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
            this->get_ptr     = source.get_ptr;
            this->raw_buff    = source.raw_buff;
            this->shared_buff = source.shared_buff;
            this->offset      = source.offset + offset;
            this->length      = length;
        }
    }

    ManagedArr(const ManagedArr& source)
    {
        this->get_ptr     = source.get_ptr;
        this->raw_buff    = source.raw_buff;
        this->shared_buff = source.shared_buff;
	    this->offset = source.offset;
	    this->length = source.length;
    }
    
    ~ManagedArr(){}

    unsigned int len() const
    {
        return length;
    }

    T get(unsigned int index) const
    {
        return (this->*get_ptr)(index);
    }

    T get_raw_buff(unsigned int index) const
    {
        if (index < length)
        {
            return raw_buff[offset + index];
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

    T (ManagedArr::*get_ptr) (unsigned int) const = nullptr;

protected:
    unsigned int length = 0;
    unsigned int offset = 0;
    T *raw_buff = nullptr;
    std::shared_ptr<T> shared_buff;


};

}

#endif
