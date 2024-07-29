
# o1.shm

A minimal shared memory library, to simplify both:
1. allocation + mapping
2. existing shared memory are mapping

## Consuming the library

* `#include <o1shm.h>` and link with `-lo1shm` to use as a library.

* `#include <o1shm-static.h>` if linking to a library is an issue.
