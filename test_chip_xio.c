/*--------------------------------------------------------------------------------------------
// Name: test_chip_xio.c
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
     int count;

     if( check_export_pin( XIO_P0 ) == 1 )
     {
	printf("Pin %d is in use!\n", XIO_P0);
	return;
     }
     else if( check_export_pin(XIO_P1) == 1)
     {
	printf("Pin %d is in use!\n", XIO_P1);
        return;
     }
     else
	printf("Pins are not in use!\n");

     printf( "export pin XIO_P0\n" );
     export_pin( XIO_P0 );
     printf( "export pin XIO_P1\n" );
     export_pin( XIO_P1 );


     printf( "XIO_P0 Pin Direction is: %s\n", get_pin_direction( XIO_P0 ));
     printf( "XIO_P1 Pin Direction is: %s\n", get_pin_direction( XIO_P1 ));

     // strncmp is used because 
     if( strncmp( get_pin_direction( XIO_P0 ), "in", 2 ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P0 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }

     if( strncmp( get_pin_direction( XIO_P1 ), "in", 2 ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P1 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }

     for( count = 0; count < 10; count++ )
     {
        printf( "Setting XIO_P0 high\n");
        set_pin_high( XIO_P0 );
        printf( "Setting XIO_P1 high\n");
        set_pin_high( XIO_P1 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );
        sleep( 5 );
        printf("Setting XIO_P0 low\n");
        set_pin_low( XIO_P0 );
        printf("Setting XIO_P1 low\n");
        set_pin_low( XIO_P1 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );
        sleep( 5 );
      }
	printf( "Setting XIO_P0 high\n");
        set_pin_high( XIO_P0 );
	printf( "Setting XIO_P1 high\n");
        set_pin_high( XIO_P1 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );

     printf( "Unexport pin XIO_P1\n" );
     unexport_pin( XIO_P1 );
     printf( "Unexport pin XIO_P0\n" );
     unexport_pin( XIO_P0 );
}
