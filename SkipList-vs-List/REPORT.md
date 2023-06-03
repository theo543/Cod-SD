### SkipList vs List Project Report

I created a SkipList class that implements the SkipList algorithm.
To make comparing with a linked list easy, I also created a SortedList class that has the same API, so I could use a template function
to run the same tests on both data structures. I tried to match the API of std::set, so the template function could be used with that as well.

I used the _random_ and _chrono_ headers for testing.

### Issues

I had some trouble figuring out how the insert method should work without spending O(n) time figuring
out the links for each level. Eventually I figured out that while searching, each time I take a step down,
I am at the first node greater than the data I'm looking for on the current level, so I can just save that pointer in a vector.
When I reach level 0, I reverse the vector, randomly decide how many levels to keep, and insert
the element before the saved pointer at each level.

To not copy-paste code for benchmarking, I made a TimeScope class that saves the current time when it is created and prints the duration when it is destroyed.
To avoid counting most of the error-checking code, I wrapped the insert/search/delete loops in blocks and did the error checking
after so TimeScope would be destroyed as soon as possible.

### Results

Results saved to 20000.txt, 40000.txt, 80000.txt, 160000.txt

The results match the expected O(log n) time complexity of SkipList insert and search,
and the O(n) time complexity of SortedList insert and search.
Deletion is always near-instant O(1) for SortedList since there's no structure to maintain. SkipList is a bit slower, having to update O(log n) links.

An interesting effect is that after deleting half the elements, the search time gets faster. The linked list
gets extreme speedup, since search is linear.

For SkipList the effect is very small, but it's visible in the biggest test case. The layers end up with half as many
elements, so search speeds up a bit. Not a lot though, since we already skipped most of the elements. (hence the O(log n) time complexity)

Obviously, std::set is much faster since the STL is highly optimized, but it's useful to check that the benchmarking code is working.
