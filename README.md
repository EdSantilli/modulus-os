# modulus-os
This is an OS that I wrote a couple years ago. I'd like to bring it to a more usable state, but as a graduate student, I do not have the time to do so. You are encouraged to use this code to help learn about basic OS development. If anyone wants to take some of the code for their own project, modify the code in any way, or build onto it...feel free! Just let me know. I'd love to see what you do with this. My email address is listed in the code headers.

Here's how to get the kernel running...

The code compiles using gcc version 4.3.3 and links using ld version 2.19.1. Untar the project and run 'make all' in the directory with the makefile. If make emits a line that says '--- Finished making /path_to_project/kernel0 ---', then the compile was successful.
The virtual hard drive is a 10MB file called c.img. It should already be formatted and setup with Grub. In order to access the virtual filesystem inside of c.img, I setup a loopback device. By running 'sh setup.sh', the loopback device should setup itself. If that doesn't work, try changing from loop7 to loop0 or loop/0 or whatever your OS likes.
You are almost there! Now, to copy the kernel to c.img and give it a test run, just call 'sh go.sh'. To be honest, you may need to set up bochsrc for your system (I've had portability issues with the Bochs script in the past).
If all goes well, you should get a page fault. This is where I left off. You can modify page.c to activate a few more page frames if you like. This will get rid of the page fault.

At the moment, the kernel does several things...

Sets up basic video services.
Sets up the global descriptor table.
Sets up the interrupt descriptor table (remaps the PIC, registers some error handling functions, etc...)
Sets up the system timer. (If you uncomment lines 70-72 of timer.c, you'll see that the timer works.)
Tries to sort the data provided by BIOS and Grub to form a realistic map of the memory layout.
Calculates the space needed for page frame stacks for the DMA and normal memory zones.
Creates the kernel's page directory and the page tables for the DMA memory zone. (Then starts paging.)
I used to have a terminal in this OS that didn't do much more than read from the keyboard buffer and print text to the screen. Maybe if I look hard enough through my backups, I can find the code, but I'd really like to get a nice malloc working first (like dlmalloc). That seems more important right now.
