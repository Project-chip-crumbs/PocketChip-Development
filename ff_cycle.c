#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "chip_xio.h"

#define RELAY_ONE   XIO_P0
#define RELAY_TWO   XIO_P1
#define BUTTON_ONE  XIO_P4
#define BUTTON_TWO  XIO_P5


static int button_one_state;

void *button_one_press( void *arg )
{
	int *pin;
	pin = (int*)arg;

	while( 1 )
	{
		if( button_one_state != 0 ) 
		{
		   button_one_state = strcmp( get_pin_value( *pin ), "0" );
		}
	}
}
void run_ffcycle(void)
{
	button_one_state = 1;
	while( button_one_state != 0 )
	{
		set_pin_high(RELAY_ONE);
		set_pin_low(RELAY_TWO);
		sleep(12);
		set_pin_low(RELAY_ONE);
		set_pin_high(RELAY_TWO);
		sleep(12);
	}
	sleep(2);
	set_pin_low(RELAY_ONE);
	set_pin_low(RELAY_TWO);
}

void main( void ){

	if( !chip_xio_start() )
	{
		return;
	}
	if( !export_pin( RELAY_ONE ) )
	{
		return;
	}
	else if( !export_pin( RELAY_TWO ) )
	{
		return;
	}
	else if( !export_pin( BUTTON_ONE ) )
	{
		return;
	}
	else if( !export_pin( BUTTON_TWO ) )
	{
		return;
	}
	set_pin_output( RELAY_ONE );
	set_pin_output( RELAY_TWO);
	set_pin_input( BUTTON_ONE );
	set_pin_input( BUTTON_TWO );
	set_pin_low(RELAY_ONE);
	set_pin_low(RELAY_TWO);

	printf( "RELAY_ONE Direction: %s\n", get_pin_direction( RELAY_ONE ) );
	printf( "RELAY_TWO Direction: %s\n", get_pin_direction( RELAY_TWO ) );
	printf( "BUTTON_ONE Direction: %s\n", get_pin_direction( BUTTON_ONE ) );
	printf( "BUTTON_TWO Direction: %s\n", get_pin_direction( BUTTON_TWO ) );

	printf("Press button 1 to run/pause fill/flush cycle: \n");
	printf("Press button 2 to end program!\n");

	pthread_t pth;
	int button_two_state;
	int pin = BUTTON_ONE;
	pthread_create( &pth, NULL, button_one_press, &pin );

	button_one_state = button_two_state = 1;

	while( button_two_state != 0   )
	{
		if( button_one_state == 0 )
		{
		   run_ffcycle();
		   button_one_state = 1;
		}
		else
		{
		   printf("Paused Fill Flush Cycle\n");
		}
		sleep(2);
		button_two_state = strcmp( get_pin_value( BUTTON_TWO ), "0");
	}
	printf("Stop Thread and Unexport pins.\n");
	pthread_cancel( pth );
	unexport_pin(BUTTON_TWO);
	unexport_pin(BUTTON_ONE);
	unexport_pin(RELAY_TWO);
	unexport_pin(RELAY_ONE);
}
