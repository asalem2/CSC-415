# Homework 6 - Paging

Write a program that simulates the FIFO and LRU page-replacement algorithms. Your program should
accept four command-line arguments specifying the page size, the total logical memory size, the pagereplacement
algorithm to use, and the number of frames allocated. The page and logical memory sizes
should be specified by their binary logs. Thus, the following command would simulate a page size of
1KB and a logical memory size of 64KB, using LRU page-replacement with 8 frames allocated:

    ./simulate-paging 10 16 LRU 8
    
Your program should read lines from standard input, each containing a hexadecimal number
representing the virtual address of a memory reference. With each memory reference, your program
should update an internal page table, keeping track of the number of page faults generated (using pure
demand paging). Your program should also maintain any other data structures necessary to implement
the two page-replacement algorithms. Your program should terminate upon reading a blank line from
standard input. It should then print the total number of page faults and exit.

You should submit your source code file(s) and a short writeup in pdf format that includes a description
of what you did and the compilation and execution output from each of your programs. For this
assignment, you do not need to implement separate versions of your programs for POSIX and Win32.
In your execution output, test both the FIFO and LRU page-replacement algorithms using a page size
of 4KB and a total logical memory size of 64KB, with the number of frames allocated ranging from 1
to 16 (i.e. 1, 2, ..., 15, 16). Test each number of frames with 100 random memory references and create
a graph of the results (with number of frames on the x-axis and number of page faults on the y-axis).
Your writeup should include the two graphs and a discussion of the results they show, including
whether Belady's anomaly occurred. Submit everything (including the writeup) to the regular
submission link on iLearn, and then submit just the writeup to the TurnItIn link to generate an
originality report.
