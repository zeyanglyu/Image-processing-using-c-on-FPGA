# Image-processing-using-c-on-FPGA

This implementation transfers a .ppm image to ASCII graf using embedded C on FPGA.

Includes reading RGB image, transferring to grayscale, resizing the grayscale, and transferring to ASCII characters graf. 

It uses different methods: message queue, bare-metal(without RTOS), and Multiprocessor(Pipeline).

The FPGA is Altera DE2 board and the processor architecture is Nios II.
