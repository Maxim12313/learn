sentinel value
flags

CPU bound work vs IO bound work

polling vs blocking

CSP communicating sequential process, where threads separate

oversubscription: too many threads that context switch outweights performance gain

# mesi
mesi cache coherence protocol
modified, exclusive, shared, invalid
writer: shared -> modified
readers: shared -> invalid
modified like dirty bit for write backjk

# ping pong
per thread cache. if read and write over protected data in same step, cache is invalidated every time
high contention, processors waiting for each other
cache ping pong

# false sharing
when cache line is shared even though no data really is, this is false sharing
causes repeated cache invalidation on thread actions that might have entirely separate data
restructure memory to group data by what each running thread accesses
similarly, distance data for different threads
want closer also so each thread invalidates less cache lines and has to load fewer

if more threads than cores, os might end up moving thread to different cores,
this requires that the l1/l2 cache be rewarmed up each time

# oversubscription
when ready threads > available threads. lose parallel excecution for excess and starts agressive context switching
this will exacerbate false sharing also because more often switching

# pointer data structure
bad for cache on access but because of this, can be better for plain traversal of pointers
even if value modified, can just look at pointer
```
struct Node {
    Data *data;
    Node *next;
};

```
# exception safety
use futures and packaged_task, will propogate up exception if fail
async also works


# performance
performance gain: $\frac{1}{f_s+\frac{1-f_s}{n}}$, where $f_n$ gives concurrent duratioin and $n$ is num threads

scalability is about reducing the time it takes to perform an action or increasing
the amount of data that can be processed in a given time as more processors are added

throughput or latency
you can hide waiting by giving idle threadss work to do

if less threads than cores, blocking thread means wasted potential.
if anticipate much blocking, can add additional thread to take its place
or might have work queue and blocking threads can instead start doing work off the queue

# responsiveness
separate main thread with tasks, gui

