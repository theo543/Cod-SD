### Heaps Project Report

For this project, I created a Heap class that provides a FindMaxAndSwap method and a GetData friend function to unwrap
the vector once sorting is done. The Heap implements the Heapify algorithm to maintain its heap property while sorting.

I used the _random_ header's std::shuffle method to generate the permutations.  
I also used the _chrono_ header to time the sorting.  
To get more accurate results, I ran each sort 10 times and averaged the results.

### Results

```
./heaps 25000 10
Heap sort: 2.2ms
Selection sort: 111.3ms
Standard sort: 1.3ms

./heaps 50000 10
Heap sort: 4.4ms
Selection sort: 429.6ms
Standard sort: 2.6ms

./heaps 100000 10
Heap sort: 9.7ms
Selection sort: 1740.4ms
Standard sort: 5.6ms
```

These results match the expected O(n log n) time complexity of heap sort and std::sort,
and the O(n^2) time complexity of selection sort.
