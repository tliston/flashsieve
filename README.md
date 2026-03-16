# flashsieve v1.0

A C-language implementation of a high-speed Sieve of Eratosthenes.

I started by mapping the physical CPU hardware cache boundaries, added unrolled loops using Duff's Device, mathematically eradicated all hardware division, implemented asynchronous software prefetching, and built a lockless L1 circular buffer to catch the intra-segment jumps.

This is a bare-metal machine-code engine that comes as close as I can to perfectly exploiting modern superscalar CPU pipelines.

On my Strix Halo Framework Desktop, it finds the 37,607,912,018 primes under 1,000,000,000,000 (1 trillion) in 18.026749 seconds.

## Python code??

I wrote some Python code to create the class of unrolled loops found in unrolled_xoff.c
