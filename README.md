# Rope

A Rope data structure which will serve as the underlying implementation of a string class.

The benefits of a Rope will be that

1. Substrings of data can be reused, making the Rope inherently compressed when leafs are reused. The leaf nodes of the Rope structure will point to a ManagedArr object, which can be referenced by many leafs. The ManagedArr is an immutable pointer to an array, containing offset and length attributes. The ManagedArr is initialized with an "Allocation" in order to delete[] any dynamically allocated arrays when the reference count goes to 0 (through a SharedPointer), or to leave alone any statically allocated arrays the ManagedArr may be built upon. Additionally, this immutable object will use a function pointer set at initialization to return elements at() from either a shared pointer or a pointer to a statically allocated array, without having to make such a determination every time at() is called.

2. Insertions into the Rope will not require reallocation and copying. Instead, new leafs are inserted in O(logn) time. Only the new leaf is allocated. This can reduces peaks in memory use during the copying of an old String to a new String instance if a concatenated String is larger than its parent.

3. A rope is less sensitive to memory fragmentation because it does need to not occupy a single contiguous array.

