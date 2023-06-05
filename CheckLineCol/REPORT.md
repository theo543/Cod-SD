### CheckLineCol Project Report

Unfortunately, I didn't finish this one. It doesn't have a custom balanced binary tree implementation,
and the sweep line algorithm is extremely buggy too. Most of the time it fails to find the intersection.
I think this happens because I manually add one collision at the end by copying the last line and swapping start.x and end.x,
which may trigger some edge case in intersection checking. I also suspect I'm using the RNG wrong somehow, it says online
that 4 random points in a square should have a >10% chance of interesting when turned into segments, but it never happens for me unless I force it.
Unfortunately, it's probably because the intersection checking is buggy, and both algorithms rely on it.

I didn't finish adding edge case handling to the line intersection algorithm, maybe that has something to do with it.

I tried using sanitizers to look for memory corruption, but it looks like the problem is a logic error, not some out-of-bounds thing.

Sorry about this, if I had more time I'm sure I could finish handling the edge cases and finding my mistakes.
