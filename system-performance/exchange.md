# considerations
scale
fairness
reliability
durability
robustness to bad client

prioritize the aggresive order

multicast to send data to multiple ports simultaneously
is udp

design it to be an fsm where behavior determined purely by state
allows replay, where on loss can be rebuilt from history
should be rebuildable

retransmitter holds history of everything
majority voting with redundancy for behavior
some passive that imitates same behavior of matching engine

need for fast
everyone gets every message
one in flight -> latency determines throughput
only 1 interaction per flight
want latency to be microseconds
roll back is simpler

recovery requires replay so speed necessary

deterministic

allocate very little in the critical path
want to be prepared upfront with all memory you would ever need
sys calls introduce unpredictable latency

# parallelism
could go on symbols, fine but etf match making or other that go over different symbols becomes inconvenient

prefetching to improve cache hits

if parallel, order that orders are processed is no longer deterministic. can't replay

publisher: sending a transaction
matching engine: just sends same transactino back out with multicast, everyone sees it

do not want double action
active passive consistency

# conclusion
speed keeps things simple
state machine replication is great
deterministic is great, can be understood. if regulator wants to reason about transaction, we know why

# priorities
consistency, determinism -> recoverability

# fault tolerant
idempotency, retry should not modify
write ahead log, log before processing it, as soon as it starts so you can replay if it doesn't complete
graceful degradation, if something breaks, degrade functionality instead of failing completely. ex leave gui read only, but still available
retry with backoff, prevents large retry storm, wait more each time 100ms->200ms->400ms..
deterministic
heartbeat passive second engine
health checks,
delayed acknowledgement only when everything is complete, prevent false positives


use active passive because input must be applied in global context to avoid double process
active active is better if more interested in behavior and server is largely stateless but higher throughput
