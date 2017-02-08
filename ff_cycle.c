#include <stdio.h>
#include <stdlib.h>
#include "chip_xio.h"

static int button_one,button_two;

void run_ffcycle(void)
{
	button_one = 1;
	while( button_one != 0 )
	{
		set_pin_high(XIO_P0);
		set_pin_low(XIO_P1);
		sleep(12);
		set_pin_low(XIO_P0);
		set_pin_high(XIO_P1);
		sleep(12);
		button_one = strcmp( get_pin_value(XIO_P4), "0" );
	}
	set_pin_high(XIO_P0);
	set_pin_high(XIO_P1);
}

void main( void ){

	if( !chip_xio_start() )
	{
		return;
	}

	if( !export_pin( XIO_P0 ) )
	{
		return;
	}
	else if( !export_pin( XIO_P1 ) )
	{
		return;
	}
	else if( !export_pin( XIO_P4 ) )
	{
		return;
	}
	else if( !export_pin( XIO_P5 ) )
	{
		return;
	}

	if( strcmp( get_pin_direction( XIO_P0 ), "out" ) != 0 )
	{
		set_pin_output( XIO_P0 );
	}
	else if( strcmp( get_pin_direction( XIO_P1), "out" ) != 0  )
	{
		set_pin_output( XIO_P1);
	}
	else if( strcmp( get_pin_direction( XIO_P4), "in" ) != 0 )
	{
		set_pin_input( XIO_P4 );
	}
	else if( strcmp( get_pin_direction( XIO_P5), "in" ) != 0 )
	{
		set_pin_input( XIO_P5 );
	}
	printf( "XIO_P0 Direction: %s\n", get_pin_direction( XIO_P0 ) );
	printf( "XIO_P1 Direction: %s\n", get_pin_direction( XIO_P1 ) );
	printf( "XIO_P4 Direction: %s\n", get_pin_direction( XIO_P4 ) );
	printf( "XIO_P5 Direction: %s\n", get_pin_direction( XIO_P5 ) );

	printf("Press button 1 to run/pause fill/flush cycle: \n");
	button_one = 1;
	button_two = 1;
	while( button_two != 0   )
	{
		if( button_one == 0 )
		{
		   run_ffcycle();
		}
		else
		{
		   printf("Paused Fill Flush Cycle\n");
		}
		sleep(3);
		button_two = strcmp( get_pin_value( XIO_P5 ), "0");
		button_one = strcmp( get_pin_value( XIO_P4 ), "0");
	}
	printf("Unexport pins.\n");
	unexport_pin(XIO_P5);
	unexport_pin(XIO_P4);
	unexport_pin(XIO_P0);
	unexport_pin(XIO_P1);
}
