# Memory-Management-System-Simulator

<b> Simulator </b>

The application will simulate memory management system behavior based on a real memory memory path(reference trace). There are two files,
each file contains a plurality of lines displaying hexadecimal memory references followed by R or W  indicating that it is for writing or
reading.

Two processes P1 and P2 are reading each file and forming requests that are forwarded(IPC) to the process MM(Memory Manager), MM implements
the virtual memory mechanism based on a hashed page table. When a new report appears, MM checks if that page is in the main memory.
For page replacement, the FlushWhenFull (FWF) algorithm is applied(considers that the report trail is organized into blocks). As each process
block is scanned, it can display up to page bugs (PF). When the k + 1 PFin process occurs it flushes all frames of certain process. If a page
to be removed has been modified it is saved on the "virtual" disk. When a new page is loaded, the page table is updated accordingly. All pages
and frames are 4 KB (4096 bytes).


<b> Compilation and Running </b>

$ make

$ ./project "size" "number of frames" "number of reports to read each time" ["max num of lines to read"]
