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
     if( !chip_xio_start() )
	return;

     printf( "export pin XIO_P0\n" );
     if( !export_pin( XIO_P0 ) )
	return;

     printf( "export pin XIO_P1\n" );
     if( !export_pin( XIO_P1 ) )
	return;

     printf( "export pin XIO_P2\n" );
     if( !export_pin( XIO_P2 ) )
	return;

     printf( "export pin XIO_P3\n" );
     if( !export_pin( XIO_P3 ) )
	return;

     printf("Checking for direction------------------------------------------\n");
     printf( "XIO_P0 Pin Direction is: %s\n", get_pin_direction( XIO_P0 ));
     printf( "XIO_P1 Pin Direction is: %s\n", get_pin_direction( XIO_P1 ));
     printf( "XIO_P2 Pin Direction is: %s\n", get_pin_direction( XIO_P2 ));
     printf( "XIO_P3 Pin Direction is: %s\n", get_pin_direction( XIO_P3 ));

     if( strcmp( get_pin_direction( XIO_P0 ), "in" ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P0 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }

     if( strcmp( get_pin_direction( XIO_P1 ), "in" ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P1 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }
     if( strcmp( get_pin_direction( XIO_P2 ), "in" ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P2 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }
     if( strcmp( get_pin_direction( XIO_P3 ), "in" ) == 0 )
     {
        printf( "Change input to output\n" );
	set_pin_output( XIO_P3 );
     }
     else
     {
        printf( "Pin set for output\n" );
     }
     printf("Run test program------------------------------------------------\n");
     for( count = 0; count < 5; count++ )
     {
        printf( "Setting XIO_P0 high\n");
        set_pin_high( XIO_P0 );
        printf( "Setting XIO_P1 low\n");
        set_pin_low( XIO_P1 );
        printf( "Setting XIO_P2 high\n");
        set_pin_high( XIO_P2 );
        printf( "Setting XIO_P3 low\n");
        set_pin_low( XIO_P3 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );
        printf( "XIO_P2 Pin Value is: %s\n", get_pin_value( XIO_P2 ) );
        printf( "XIO_P3 Pin Value is: %s\n", get_pin_value( XIO_P3 ) );
        sleep( 1 );
        printf("Setting XIO_P0 low\n");
        set_pin_low( XIO_P0 );
        printf("Setting XIO_P1 high\n");
        set_pin_high( XIO_P1 );
        printf("Setting XIO_P2 low\n");
        set_pin_low( XIO_P2 );
        printf("Setting XIO_P3 high\n");
        set_pin_high( XIO_P3 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );
        printf( "XIO_P2 Pin Value is: %s\n", get_pin_value( XIO_P2 ) );
        printf( "XIO_P3 Pin Value is: %s\n", get_pin_value( XIO_P3 ) );
        sleep( 1 );
      }
	printf( "Setting XIO_P0 high\n");
        set_pin_high( XIO_P0 );
	printf( "Setting XIO_P1 high\n");
        set_pin_high( XIO_P1 );
	printf( "Setting XIO_P2 high\n");
        set_pin_high( XIO_P2 );
	printf( "Setting XIO_P3 high\n");
        set_pin_high( XIO_P3 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );
        printf( "XIO_P2 Pin Value is: %s\n", get_pin_value( XIO_P2 ) );
        printf( "XIO_P3 Pin Value is: %s\n", get_pin_value( XIO_P3 ) );
     printf("Checking for unexport-----------------------------------------------\n");
     printf( "Unexport pin XIO_P0\n" );
     unexport_pin( XIO_P0 );
     printf( "Unexport pin XIO_P1\n" );
     unexport_pin( XIO_P1 );
     printf( "Unexport pin XIO_P2\n" );
     unexport_pin( XIO_P2 );
     printf( "Unexport pin XIO_P3\n" );
     unexport_pin( XIO_P3 );
}
