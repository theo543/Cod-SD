### CheckLineCol Project Report

Unfortunately, I didn't finish this one. It doesn't have a custom balanced binary tree implementation,
and the sweep line algorithm is extremely buggy too. Most of the time it fails to find the intersection.
I think this happens because I manually add one collision at the end by copying the last line and swapping start.x and end.x,
which may trigger some edge case in intersection checking.

I didn't finish adding edge case handling to the line intersection algorithm, maybe that has something to do with it.

