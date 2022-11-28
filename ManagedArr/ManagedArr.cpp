//#include "ManagedArr.h"
//
//template <class T>
//phillno::ManagedArr<T>::ManagedArr()
//{
//    std::cout << "Hello world" << std::endl;
//}
//
//template <class T>
//phillno::ManagedArr<T>::ManagedArr(T const *data, unsigned int len, SourceAllocation allocation)
//{
//    /*
//    Source allocation of STATIC will not give the pointer to source char array to a shared pointer
//    
//    This will ensure no side effect of delete[]-ing the static source.
//
//    Source allocation of DYNAMIC will give the pointer to the original char array to a shared pointer.
//
//    Side effect: When all of the shared pointers go out of scope, they will delete[] the source.
//    */
//
//    if (data != nullptr)
//    {
//        length = len;
//        buff   = data;
//        
//        if (allocation == SourceAllocation::DYNAMIC)
//        {
//            shared_buff = std::shared_ptr<T const[]>(buff);
//        }
//    }
//}
//
//template <class T>
//phillno::ManagedArr<T>::ManagedArr(const ManagedArr<T>& source, unsigned int offset, unsigned int length)
//{   /*
//    Creates a new shared string by using the same data buffer at a provided
//    offset/
//    */
//    
//    if ((offset + length) > source.length)
//    {
//        throw "Index out of bounds exception";
//    }
//    else
//    {
//        this->offset      = offset;
//        this->length      = length;
//        this->buff        = source.buff;
//        this->shared_buff = source.shared_buff;
//    }
//}
//
//template <class T>
//T const* phillno::ManagedArr<T>::get_buff()
//{
//    return buff;
//}
//
//template <class T>
//unsigned int phillno::ManagedArr<T>::get_length()
//{
//    return length;
//}
//
//template <class T>
//T phillno::ManagedArr<T>::operator[](unsigned int index)
//{
//    if (index < length)
//    {
//        return buff[offset + index];
//    }
//    else
//    {
//        throw "Index out of bounds exception";
//    }
//}
