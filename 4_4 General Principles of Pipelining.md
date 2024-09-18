* A key feature of pipelining is that it **increases the throughput** of the system (i.e., the number of customers served per unit time), but it may also slightly increase the latency (i.e., the time required to service an individual customer).

# 4.4.1 Computational Pipelines
## a simple nonpipelined hardware system
![[Pasted image 20240918151225.png|400]]
* A simple nonpipelined hardware system consists of some **logic** that performs a computation, followed by a **register** to hold the results of this computation. 
* A **clock signal** controls the loading of the register at some regular time interval.
* The computational block is implemented as **combinational logic**, meaning that the signals will pass through a series of logic gates, with the outputs becoming some function of the inputs after some time delay.
* Some computations of the performance:
	* Assumptions:
		* The combinational logic requires 300 ps.
		* The loading of the register requires 20 ps.





























