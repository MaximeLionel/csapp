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

* Tips:
	* Slowing down the clock would not change the pipeline behavior.
	* If the clock were run too fast, the values would not have time to propagate through the combinational logic.
	* The simple mechanism of having clocked registers between blocks of combinational logic suffices to control the flow of instructions in the pipeline.

# 4.4.3 Limitations of Pipelining

## Nonuniform Partitioning
![[Pasted image 20240920150626.png|500]]
* The system divides the computation into 3 stages, but the delays through the stages range from 50 to 150 ps. 
* The sum of the delays through all of the stages remains 300 ps.
* Nonuniform stage delays:
	* Stage A will be idle for 100 ps every clock cycle.
	* Stage C will be idle for 50 ps every clock cycle.
	* Only stage B will be continuously active for whole 170 ps every clock cycle.
* Computations:
	* We must set the clock cycle to 150 + 20 = 170 picoseconds.
	* $Throughput = \frac{1~instruction} {(20+150)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 5.88~GIPS$ - decreasing from 8.33 GIPS.
	* $Latency = (150+20)\times 3 = 510~ps$ - increasing from 360 ps.
* Challenges:
	* Devising a partitioning of the system computation into a series of stages having uniform delays can be a major challenge for hardware designers.
	* Some of the hardware units in a processor, such as the ALU and the memories, cannot be subdivided into multiple units with shorter delay.
	* This makes it difficult to create a set of balanced stages.

### Practice Problem 4.28
Suppose we analyze the combinational logic above and determine that it can be separated into a sequence of 6 blocks, named A to F, having delays of 80, 30, 60, 50, 70, and 10 ps, respectively, illustrated as follows:
![[Pasted image 20240920153326.png|400]]
We can create pipelined versions of this design by inserting pipeline registers between pairs of these blocks. Different combinations of pipeline depth (how many stages) and maximum throughput arise, depending on where we insert the pipeline registers. Assume that a pipeline register has a delay of 20 ps.

A. Inserting a single register gives a two-stage pipeline. Where should the register be inserted to maximize throughput? What would be the throughput and latency?

B. Where should 2 registers be inserted to maximize the throughput of a three-stage pipeline? What would be the throughput and latency?

C. Where should 3 registers be inserted to maximize the throughput of a 4-stage pipeline? What would be the throughput and latency?

D. What is the minimum number of stages that would yield a design with the maximum achievable throughput? Describe this design, its throughput, and its latency.

**Solution**:
A.
The point is to create the shortest latency.
Insert between C and D.
$Throughput = \frac{1~instruction} {(20+80+30+60)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 5.26~GIPS$
$Latency = (80+30+60+20)\times 2 = 380~ps$

B.
Insert 1 register between B and C.
Insert 1 register between D and E.
$Throughput = \frac{1~instruction} {(20+80+30)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 7.69~GIPS$
$Latency = (80+30+20)\times 3 = 390~ps$

C.
Insert 1 register between A and B.
Insert 1 register between C and D.
Insert 1 register between D and E.
$Throughput = \frac{1~instruction} {(20+30+60)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 9.09~GIPS$
$Latency = (30+60+20)\times 4 = 440~ps$

D.
5 stages design:
4 registers are inserted in AB, BC, CD and DE.
$Throughput = \frac{1~instruction} {(20+80)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 10~GIPS$
$Latency = (80+20)\times 5 = 500~ps$

6 stages design (maximum number of stages):
5 registers are inserted in AB, BC, CD, DE and EF.
$Throughput = \frac{1~instruction} {(20+80)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} \approx 10~GIPS$
$Latency = (80+20)\times 6 = 600~ps$

The minimum number of stages that would yield a design with the maximum achievable throughput is 5.

## Diminishing Returns of Deep Pipelining
![[Pasted image 20240920164726.png|500]]
* The system divides the computation into 6 stages, each requiring 50 ps.
* The minimum clock period for this system is 50 + 20 = 70 picoseconds, giving a throughput of 14.29 GIPS.
* We improve the performance by a factor of 14.29/8.33 = 1.71.
* Even though we have cut the time required for each computation block by a factor of 2, we do not get a doubling of the throughput, due to the delay through the pipeline registers.
	* This delay becomes a limiting factor in the throughput of the pipeline. 
	* In our new design, this delay consumes 28.6% of the total clock period.
* Challenges：
	* Modern processors employ very deep pipelines (15 or more stages) in an attempt to maximize the processor clock rate. 
	* The **processor architects** divide the instruction execution into a large number of very simple steps so that each stage can have a very small delay. 
	* The **circuit designers** carefully design the pipeline registers to minimize their delay.
	* The **chip designers** must also carefully design the clock distribution network to ensure that the clock changes at the exact same time across the entire chip.
	* All of these factors contribute to the challenge of designing high-speed microprocessors.

### Practice Problem 4.29
![[Pasted image 20240920171328.png|300]]
Suppose we could take the system above and divide it into an arbitrary number of pipeline stages k, each having a delay of 300/k, and with each pipeline register having a delay of 20 ps.

A. What would be the latency and the throughput of the system, as functions of k?

B. What would be the ultimate limit on the throughput?

**Solution**:
A.
$Throughput = \frac{1~instruction} {(20+300/k)picoseconds}\times \frac{1,000~picoseconds}{1~nanoseconds} = \frac{1000k}{300+20k}~GIPS$

$Latency=(20+300/k)\times k = 20k+300~ps$

B.
With k goes to infinity, the throughput will be 50 GIPS.

# 4.4.4 Pipelining a System with Feedback
## Dependencies between instructions
* For a system that executes machine programs such as x86-64 or Y86-64, however, there are potential ==dependencies== between successive instructions.
### Data dependency
* Example - **data dependency**:
	![[Pasted image 20240922193615.png|200]]
	* There is a **data dependency** between each successive pair of instructions
	* The `irmovq` instruction (line 1) stores its result in `%rax`, which then must be read by the `addq` instruction (line 2); and this instruction stores its result in `%rbx`, which must then be read by the `mrmovq` instruction (line 3).
### Control dependency
* Example - **control dependency**:
	![[Pasted image 20240922194758.png|200]]
	* The `jne` instruction (line 3) creates a control dependency since the outcome of the conditional test determines whether the next instruction to execute will be the `irmovq` instruction (line 4) or the halt instruction (line 7).
## Introducing pipelining into a system
![[Pasted image 20240922212657.png|400]]

![[Pasted image 20240922213403.png|300]]
* In the unpipelined system, the result of each instruction is fed back around to the next instruction.

![[Pasted image 20240922213627.png|400]]
* Pipeline diagram - the result of I1 becomes an input to I2, and so on.

![[Pasted image 20240922214551.png|450]]
* If convert this to a three-stage pipeline above in the most straightforward manner, we change the behavior of the system in a wrong way.
	* The result of I1 becomes an input to I4.
* When we introduce pipelining into a Y86-64 processor, we must deal with feedback effects properly.
