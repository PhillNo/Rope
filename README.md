# Rope

A Rope data structure which will serve as the underlying implementation of a new string-like class.

The benefits of a Rope are:

1. Substrings of data can be reused, making the Rope inherently compressed when leafs are reused.

2. Insertions into the Rope will not require reallocation and copying. Instead, new leafs are inserted in O(logn) time. Only original leafs are allocated. This can reduce peaks in memory use during the copying of an old String to a new String instance if a concatenated String is larger than its parent.

3. A Rope requires less contiguous memory allocation since it is comprised of pointers to other Ropes (and std::span at the leafs).



Todos:

Replace recursion with a loop and stack

Allow construction and insertion of type T

Implement more Iterator traits.

Implement more std::string.

Implement typedef for Rope<char> as a std::string replacement

