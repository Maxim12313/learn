# Misc
web response time = dns latency, tcp connection latency, tcp data transmission time
request latency, etc modify latency to specify
in network transfer time: hops, rate of drops, retransmissions, complicated
cpu cycle: 0.3ns
if cpu cycle=1
good fast cheap, performant on-time inexpensive
quick wins
contention for a resource degrading throughput, knee point
fast degradation vs slow degradation
utiliization time based or capacity based, often device usage
cache performance slop is expo by hit rate
cache warmth, how relevant curr data
resource anallysis utilization vs workload analsysi latency, request <-> response
observational trying to spot the difference by comparison vs experimental tryying to counteract the diffeerence and see if it fixes
negative test: deliberately hurting performance to see the direction of an issue
USE, for eevery resource, check utilization, saturation, and errors
saturatino=pressure
for these resources like main memory, utilizaiton has to do with capacity
a failed request might have low latency but might lead to additional work afterwards to clean up
error, then saturation, then utilization
saturation as a weight queue length "average queue length of 4"
CPUs: Sockets, cores, hardware threads (virtual CPUs)
* Main memory: DRAM
* Network interfaces: Ethernet ports, Infiniband
* Storage devices: Disks, storage adapters
* Accelerators: GPUs, TPUs, FPGAs, etc., if in use
* Controllers: Storage, network
* Interconnects: CPU, memory, I/O

RED, for every service, check request rate, errors, and duration
workload characterization, who why what how

# Concrete
Performance mantras
1. Don’t do it: Eliminate unnecessary work.
2. Do it, but don’t do it again: Caching.
3. Do it less: Tune refreshes, polling, or updates to be less frequent.
4. Do it later: Write-back caching.
5. Do it when they’re not looking: Schedule work to run during off-peak hours.
6. Do it concurrently: Switch from single-threaded to multi-threaded.
7. Do it more cheaply: Buy faster hardware.

Contention: Some components of the architecture are shared and can be used only
serially, and contention for these shared resources begins to reduce the effectiveness of
scaling.
Coherence: The tax to maintain data coherency including propagation of changes begins
to outweigh the benefits of scaling.

Queueing systems can be categorized by three factors:
* Arrival process: This describes the inter-arrival time for requests to the queueing system,
which may be random, fixed-time, or a process such as Poisson (which uses an exponen-
tial distribution for arrival time).
* Service time distribution: This describes the service times for the service center. They
may be fixed (deterministic), exponential, or of another distribution type.
* Number of service centers: One or many.
These factors can be written in Kendall’s notati

For each metric, estimate the performance gain from resollving the issue
Example:
* Observed: Application request takes 10 ms.
* Observed: Of that, 9 ms is disk I/O.
* Suggestion: Configure the application to cache I/O in memory, with expected DRAM latency around ~10 μs.
* Estimated gain: 10 ms → 1.01 ms (10 ms - 9 ms + 10 μs) = ~9x gain

# Considerations
Everything is a tradeoff!

