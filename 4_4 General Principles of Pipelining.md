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
* Three-stage pipelined computation hardware structure:
	* Divide the computation performed by our system into 3 stages, A, B, and C, where each requires 100 ps.
	* Put **pipeline registers** between the stages so that each instruction moves through the system in 3 steps, requiring three complete clock cycles from beginning to end.
* Pipeline diagram:
	* Allow `I2` to enter stage A as soon as `I1` moves from A to B, and so on.
	* In steady state, all 3 stages would be active, with one instruction leaving and a new one entering the system every clock cycle.
	* During the 3rd clock cycle in the pipeline diagram where `I1` is in stage C, `I2` is in stage B, and `I3` is in stage A.
* Some computations of the performance:
	* The computation is split into stages A, B, and C. On each 120 ps cycle, each instruction progresses through one stage.
	* Cycle the clocks every 100 + 20 = 120 picoseconds (皮秒), giving a throughput of around 8.33 GIPS ($Throughput = \frac{1~instruction} {(20+100)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 8.33~GIPS$).
	* Since processing a single instruction requires 3 clock cycles, the latency of this pipeline is 3 × 120 = 360 ps.
* Performance inprovements - increase the **throughput** of the system by a factor of 8.33/3.12 = 2.67 at the expense of some added hardware and a slight increase in the **latency** (360/320 = 1.12).
	* The increased latency is due to the time overhead of the added pipeline registers.

# 4.4.2 A Detailed Look at Pipeline Operation
## Three-stage pipeline timing
![[Pasted image 20240919151150.png|300]]
* The transfer of the instructions between pipeline stages is controlled by a **clock signal**.
* The rising edge of the clock signal controls the movement of instructions from one pipeline stage to the next.
* Every 120 ps, this signal rises from 0 to 1, initiating the next set of pipeline stage evaluations.

## One clock cycle of pipeline operation
![[Pasted image 20240919152328.png|400]]
* Between times 240 and 360:
	* The instruction `I1` propagates through stage C;
	* The instruction `I2` propagates through stage B;
	* The instruction `I3` propagates through stage A.
![[Pasted image 20240919153557.png|400]]
* Point 1 (239 ps) - just before the rising clock at time 240,
	* The values computed in stage A for instruction `I2` (blue) have reached the input of the first pipeline register, but its state and output remain set to those computed during stage A for instruction `I1` (dark gray).
		* The values computed in stage B for instruction `I1` (dark gray) have reached the input of the second pipeline register.
![[Pasted image 20240919153903.png|400]]
* Point 2 (241 ps) - the clock rises from point 1.
	* As the clock rises, all the inputs are loaded into the pipeline registers, becoming the register outputs.
	* The input to stage A is set to initiate the computation of instruction `I3` (light gray).
![[Pasted image 20240919155013.png|400]]
* Point 3 (300 ps) - the signals propagate through the combinational logic for the different stages.
![[Pasted image 20240919155315.png|400]]
* Point 4 (359 ps) - before time 360, the result values reach the inputs of the pipeline registers.
* When the clock rises at time 360, each of the instructions will have progressed through one pipeline stage.
























