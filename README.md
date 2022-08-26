# Memory-Allocation

<h2>Compilation Instructions</h2>
<b>gcc JKim_mem.c -o JKim_mem.exe<br>					                                                  
./JKim_mem.exe 1048576</b>

<h2>Program Design</h2>
This program simulates the contiguous memory allocation via the worst-fit-algorithm. Worst-fit algorithm looks for the biggest available hole in the memory to allocate a new process. This type of allocation produces external fragmentation- fragments of memory scattered in the memory space. Compaction pushes all free space together to make more room for the next process memory allocation. Strcmp and strcpy functions were VIP tools for my program. I used strcmp to identify free memory chunks by traversing through the LinkedList. I used Strcpy function to write over the processID variable which indicates whether a block is free or occupied by another process. 

<h2>Result</h2>
<pre>
Initial Memory: 1048576
Addresses [0 : 1048576] Free

allocator>RQ P0 262144 W
allocator>RQ P1 262144 W
allocator>RQ P2 262200 W
allocator>STAT
Addresses [0 : 262144] P0
Addresses [262145 : 524288] P1
Addresses [524289 : 786488] P2
Addresses [786489 : 1048576] Free

allocator>RL P1
allocator>RQ P3 200000 W
allocator>STAT
Addresses [0 : 262144] P0
Addresses [262145 : 462144] P3
Addresses [462145 : 524288] Free
Addresses [524289 : 786488] P2
Addresses [786489 : 1048576] Free

allocator>RQ P4 200000 W
allocator>STAT
Addresses [0 : 262144] P0
Addresses [262145 : 462144] P3
Addresses [462145 : 524288] Free
Addresses [524289 : 786488] P2
Addresses [786489 : 986488] P4
Addresses [986489 : 1048576] Free

allocator>C
allocator>STAT
Addresses [0 : 262144] P0
Addresses [262145 : 462144] P3
Addresses [462145 : 724344] P2
Addresses [724345 : 924344] P4
Addresses [924345 : 1048576] Free

allocator>QUIT
</pre>
