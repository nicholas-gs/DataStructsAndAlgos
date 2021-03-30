add_library(
    wtl_sort_lib
    include/BubbleSort.h 
    include/BucketSort.h
    include/CountingSort.h 
    include/HeapSort.h
    include/InsertionSort.h 
    include/MergeSort.h
    include/QuickSort.h 
    include/RadixSort.h 
    include/SelectionSort.h 
    include/TreeSort.h
)

set(CMAKE_CXX_STANDARD 17)

target_include_directories(wtl_sort_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_target_properties(wtl_sort_lib PROPERTIES LINKER_LANGUAGE CXX)