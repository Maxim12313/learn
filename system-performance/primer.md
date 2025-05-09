# Performance vs Scalability
Performance is right now
Scalability is potential, how performance scales with more workload and more resources

# Latency vs Throughput
Latency is the speed from event -> result
Throughput is the actions / time, think of batches


# Availability vs Consistency
Can only support 2 of CAP: consistency, availability, partition tolerance
consistency: every read is up to date or an error
availability: every read will get a response, but might be stale

consistency is better if we're making a critical decision. We would rather abort or wait for a more prime oppurtunity than act on stale information
availability is good if we believe that user responsiveness is better, for example in a monitor or log system, seeing a general trend
might just mark as outdated as well

# Considerations
Want to minimize IO, make fewest rounds of IO and network trips
Every IO incurrs immedaite cost of context switch to kernel space for sys call
Consider buffering IO operations and doing together as a batch

# Security
Encrypt in transit + at rest
Sanitize input
Parameterize where possible to avoid injection
Least privilige

# Numbers
2^3 bits per byte
2^10 = thousand = 1KB
2^20 = million = 1MB
2^30 = billion = 1GB
2^40 = trillion = 1TB

cycle    = 0.25ns
l1 cache = 1ns
l2 cache = 5ns
l3 cache = 10ns
main mem = 100ns
lan      = 100,000ns   = 0.1ms
country  = 100,000,000ns = 100ms
diskread ssd = 10,000 ns = 0.01ms =  10 micro sec

branch mispredict = 3ns
mutex lock/unlock = 17ns
