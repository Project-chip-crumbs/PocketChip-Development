/*--------------------------------------------------------------------------------------------
// Name: blink.c
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
#include "chip_xio.h"

// Test program
void main( void )
{
     int i;
     if( geteuid() != 0 )
     {
	printf("Run as a sudo user!\n");
	return;
     }
     int count;
     printf("Checking for export---------------------------------------------\n");
     if( check_export_dir() == 0 )
     {
	printf("Export dir not found!\n");
	return;
     }
     if( check_export_pin( XIO_P4 ) == 1 )
     {
	printf("Pin %d is in use!\n", XIO_P4);
	return;
     }
     else
	printf("Pins are not in use!\n");

     printf( "export pin XIO_P0\n" );
     export_pin( XIO_P4 );
     printf("Checking for direction------------------------------------------\n");
     printf( "XIO_P4 Pin Direction is: %s\n", get_pin_direction( XIO_P4 ));

     if( strcmp( get_pin_direction( XIO_P4 ), "in" ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P4 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }
     printf("Run test program------------------------------------------------\n");
     for( i = 0 ; i < 100; i++ )
     {
       // printf( "Setting XIO_P4 high\n");
        set_pin_high( XIO_P4 );
       // printf( "XIO_P4 Pin Value is: %s\n", get_pin_value( XIO_P4 ) );
        sleep( 1 );
       // printf("Setting XIO_P4 low\n");
        set_pin_low( XIO_P4 );
       // printf( "XIO_P4 Pin Value is: %s\n", get_pin_value( XIO_P4 ) );
        sleep( 1 );
      }
      printf( "Unexport pin XIO_P4\n" );
      unexport_pin( XIO_P4 );
}
