* A key feature of pipelining is that it **increases the throughput** of the system (i.e., the number of customers served per unit time), but it may also slightly increase the latency (i.e., the time required to service an individual customer).

# 4.4.1 Computational Pipelines
## Unpipelined computation
hardware
![[Pasted image 20240918161817.png|400]]
* A simple nonpipelined hardware system: 
	* consists of some **logic** that performs a computation, followed by a **register** to hold the results of this computation. 
	* A **clock signal** controls the loading of the register at some regular time interval.
	* The computational block is implemented as **combinational logic**, meaning that the signals will pass through a series of logic gates, with the outputs becoming some function of the inputs after some time delay.
* Pipeline diagram:
	* Time flows from left to right. 
	* A series of instructions (here named I1, I2, and I3) are written from top to bottom. 
	* The solid rectangles indicate the times during which these instructions are executed.
	* In this implementation, we must complete one instruction before beginning the next. Hence, the boxes do not overlap one another vertically.
* Some computations of the performance:
	* Assumptions:
		* The combinational logic requires 300 ps.
		* The loading of the register requires 20 ps.
	* Formulas: $Throughput = \frac{1~instruction} {(20+300)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 3.12~GIPS$
		* GIPS - billions of instructions per second.
	* The total time required to perform a single instruction from beginning to end is known as the **latency**.
	*  In this system, the latency is 320 ps.

## Three-stage pipelined computation hardware
![[Pasted image 20240918163705.png|500]]





























