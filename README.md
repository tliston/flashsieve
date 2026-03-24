# flashsieve v1.1

A C-language implementation of a high-speed Sieve of Eratosthenes.

I started by mapping the physical CPU hardware cache boundaries, added unrolled loops using Duff's Device, mathematically eradicated all hardware division, implemented asynchronous software prefetching, and built a lockless L1 circular buffer to catch the intra-segment jumps.

This is a bare-metal machine-code engine that comes as close as I can to perfectly exploiting modern superscalar CPU pipelines.

On my Strix Halo Framework Desktop, it finds 37,607,912,018 primes under 1,000,000,000,000 (1 trillion) in 9.699310 seconds (using the L2 cache; see below).

## Python code??

I wrote some Python code to create the C code for the class of unrolled loops found in unrolled_xoff.c - it's all basically a big ol' template.

## Change log
v1.0 -> v1.1: Added the ability to use a segment size chosen to fit the L2 cache with a command-line parameter.

It turns out that the Strix Halo architecture has an L2 cache that is fast enough that using a larger segment size (to take advantage of hyper-optimized unrolled loops for MUCH longer) actually more than makes up for the L1/L2 speed difference. 1 trillion in less than 10 seconds! Using a smaller segment size that fit within the L1 cache took 18.026749 seconds...
