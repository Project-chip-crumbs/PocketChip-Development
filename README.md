# PocketChip-Development

           chip_xio is a basic application to drive the xio port on the C.H.I.P.  Layered on
           the existing sysfs implementation for NON-RTA pin driving.
           At this time it only controls individual pin input or output.  This code is not
           portable with other OS's without rewrite.

           GPIO Sysfs Interface for Userspace
           https://www.kernel.org/doc/Documentation/gpio/sysfs.txt

           Next Thing Co. documentation.
           https://docs.getchip.com/chip.html#gpio
           Pins XIO-P0 to P7 linearly map to gpio408 to gpio415 on kernel 4.3 and gpio1016 to
           gpio1023 on kernel 4.4.11. For kernel 4.4.13-ntc-mlc the range is gpio1013 to
           gpio1019. See documentation to determine version.

To compile it (if you use GCC):

$ gcc -c chip_xio.c -o chip_xio.o
$ gcc -c test_chip_xio.c -o test_chip_xio.o

To link the files to create an executable file:

$ gcc test_chip_xio.o chip_xio.o -o test_chip_xio

You can test sample:

$ sudo ./test_chip_xio

TODO: Makefile
