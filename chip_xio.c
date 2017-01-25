/*--------------------------------------------------------------------------------------------
// Name: chip_xio.c
//
// Synopsis: chip_xio is a basic application to drive the xio port on the C.H.I.P.  Layered on
//           the existing sysfs implementation for NON-RTA pin driving.
//           At this time it only controls individual pin input or output.  This code is not
//           portable with other OS's without rewrite.
//
//           GPIO Sysfs Interface for Userspace
//           https://www.kernel.org/doc/Documentation/gpio/sysfs.txt
//
//           Next Thing Co. documentation.
//           https://docs.getchip.com/chip.html#gpio
//           Pins XIO-P0 to P7 linearly map to gpio408 to gpio415 on kernel 4.3 and gpio1016 to
//           gpio1023 on kernel 4.4.11. For kernel 4.4.13-ntc-mlc the range is gpio1013 to
//           gpio1019. See documentation to determine version.
//
// Please note this software is not warrantied and any resulting damage from its use is not
// the writers responsibility.  Use at your own risk!
//
// Copyright (c) 2017 Ovide N. Mercure
---------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "chip_xio.h"

#define BUFFS    3
// This will change based on kernal 
// version.  See note above.
#define ADDRESS_SIZE 4

// Future port access
//int xio_port_arr[] = { XIO_P0, XIO_P1, XIO_P2, XIO_P3, XIO_P4, XIO_P5, XIO_P6, XIO_P7 };

char buff[BUFFS];

// Use to check avalability of pin for export.
// Return '1' found directory.  Return '0' no directory.
int check_export_pin( int pin )
{
     struct stat info;
     char* str = "/sys/class/gpio/gpio";
     unsigned int size;
     size = (unsigned int)strlen( str );
     size += ADDRESS_SIZE;
     char path[size];
     sprintf( path, "%s%d", str, pin );
     if(stat( path, &info ) != 0)
        return 0;  // No DIR access
     else if(info.st_mode & S_IFDIR)  //Is a DIR
        return 1;
     else
        return 0;  //No DIR
}
// Export a pin
int export_pin( int pin )
{
     char* front = "sh -c 'echo";
     char* back  = "> /sys/class/gpio/export'";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += (ADDRESS_SIZE+2);
     char str[size];
     sprintf( str, "%s %d %s", front, pin, back );
     return system( str );
}
// Unexport a pin
int unexport_pin( int pin )
{
     char* front = "sh -c 'echo";
     char* back  = "> /sys/class/gpio/unexport'";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += (ADDRESS_SIZE+2);
     char str[size];
     sprintf( str, "%s %d %s", front, pin, back );
     return system( str );
}
// Set the selected pin for output
void set_pin_output( int pin )
{
     char* front = "sh -c 'echo out > /sys/class/gpio/gpio";
     char* back  = "/direction'";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += ADDRESS_SIZE;
     char str[size];
     sprintf( str, "%s%d%s", front, pin, back );
     system( str );
}
// Set the selected pin for input
void set_pin_input( int pin )
{
     char* front = "sh -c 'echo in > /sys/class/gpio/gpio";
     char* back  = "/direction'";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += ADDRESS_SIZE;
     char str[size];
     sprintf( str, "%s%d%s", front, pin, back );
     system( str );
}
// Get the direction of the pin.  Input or output.
char* get_pin_direction( int pin )
{
     int fd;
     char* front = "/sys/class/gpio/gpio";
     char* back  = "/direction";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += ADDRESS_SIZE;
     char str[size];
     sprintf( str, "%s%d%s", front, pin, back );

     if(( fd = open( str, O_RDONLY, 0 )) == -1 )
        error( "get_pin_direction: can't open" );
     read( fd, buff, 3 );
     close(fd);
     return &buff[0];
}
// Get the current value of the pin.
char *get_pin_value( int pin )
{
     int fd;
     char* front = "/sys/class/gpio/gpio";
     char* back  = "/value";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += ADDRESS_SIZE;
     char str[size];
     sprintf( str, "%s%d%s", front, pin, back );

     if(( fd = open( str, O_RDONLY, 0 )) == -1 )
        error( "get_pin_value: can't open" );
     buff[1] = 0;
     read( fd, buff, 1 );
     close(fd);
     return &buff[0];
}
// Set the selected output pin high.
void set_pin_high( int pin )
{
     char* front = "sh -c 'echo 1 > /sys/class/gpio/gpio";
     char* back  = "/value'";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += ADDRESS_SIZE;
     char str[size];
     sprintf( str, "%s%d%s", front, pin, back );
     system( str );
}
// Set the selected output pin low.
void set_pin_low( int pin )
{
     char* front = "sh -c 'echo 0 > /sys/class/gpio/gpio";
     char* back  = "/value'";
     unsigned int size;
     size = (unsigned int)strlen( front );
     size += (unsigned int)strlen( back );
     size += ADDRESS_SIZE;
     char str[size];
     sprintf( str, "%s%d%s", front, pin, back );
     system( str );
}
