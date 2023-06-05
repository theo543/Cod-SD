### Josephus Project Report

I created a LoopedList class to store nodes in a circular linked list.
It functions like a regular linked list, but the last node points to the first node.
It was a bit annoying figuring out the edge cases for maintaining the loop, but it works.

To solve the problem I read a number from the command line, then put that many nodes in a LoopedList,
then I deleted every other node until there is only one left.

To make sure it's working, I also output the formula answer for comparison.

### Results

```
Enter non-unsigned int value to exit
1
Looped List solution is: 1
Formula solution is: 1
10
Looped List solution is: 5
Formula solution is: 5
100
Looped List solution is: 73
Formula solution is: 73
1000
Looped List solution is: 977
Formula solution is: 977
10000
Looped List solution is: 3617
Formula solution is: 3617
100000
Looped List solution is: 68929
Formula solution is: 68929
1000000
Looped List solution is: 951425
Formula solution is: 951425
10000000
Looped List solution is: 3222785
Formula solution is: 3222785
100000000
Looped List solution is: 65782273
Formula solution is: 65782273
```
