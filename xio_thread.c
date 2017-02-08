#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "chip_xio.h"

#define BUTTON_ONE XIO_P4
#define BUTTON_TWO XIO_P5

static int button_one_state;



void *button_one_press( void *arg )
{
	int *pin;
	pin = (int*)arg;

	while( 1 )
	{
		if( button_one_state != 0 )
		   button_one_state = strcmp( get_pin_value( *pin ), "0" );
	}
}

void main( void )
{
	if( !chip_xio_start() )
	{
		return;
	}
	if( !export_pin( BUTTON_ONE ) )
	{
		return;
	}
	else if( !export_pin( BUTTON_TWO ) )
	{
		return;
	}

	set_pin_input( BUTTON_ONE );
	set_pin_input( BUTTON_TWO );

	pthread_t pth;
	int button_two_state;
	button_one_state = button_two_state = 1;
        int pin = BUTTON_ONE;
	pthread_create( &pth, NULL, button_one_press, &pin );

	while( button_two_state != 0)
	{
		if( button_one_state == 0 )
		{
		   printf("Button one was pressed and the thread caught it!\n");
		   button_one_state = 1;
		}
		else
		{
		   printf("Waiting for button one key press!\n");
		}
		sleep(2);
		button_two_state = strcmp( get_pin_value(BUTTON_TWO), "0" );
	}
	pthread_cancel( pth );
	unexport_pin(BUTTON_ONE);
	unexport_pin(BUTTON_TWO);
}
