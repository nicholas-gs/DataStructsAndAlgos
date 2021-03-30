add_library(
    wtl_datastructures_lib
    include/Array.h
    include/BiMap.h
    include/BinarySearchTree.h
    include/ClosedMap.h
    include/DoublyLinkedList.h
    include/FenwickTree.h
    include/Heap.h
    include/IndexedPriorityQueue.h 
    include/PriorityQueue_LL.h
    include/PriorityQueue.h
    include/Queue.h 
    include/SinglyLinkedList.h
    include/SparseTable.h 
    include/Stack_LL.h 
    include/Stack.h 
    include/UnionFind.h 
    include/UnorderedMap.h 
    include/UnorderedSet.h 
    include/Vector.h
)

set(CMAKE_CXX_STANDARD 17)

target_include_directories(wtl_datastructures_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(wtl_datastructures_lib PROPERTIES LINKER_LANGUAGE CXX)
