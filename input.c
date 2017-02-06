#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chip_xio.h"
//extern int   export_pin        ( int pin );
//extern char* get_pin_direction ( int pin );
//extern int   set_pin_input     ( int pin );
//extern char* get_pin_value     ( int pin );
//extern int   unexport_pin      ( int pin );


void main( void )
{
	if( chip_xio_start() < 1 )
	{
		printf("Initialization faild!\n");
		return;
	}

        if( export_pin( XIO_P4 ) == -1 )
        {
		printf("Export failed!\n");
		return;
        }
	 else
	{
		printf("Somehow\n");
		return;
	}

	if( get_pin_direction( XIO_P4 ) == "out" )
		set_pin_input( XIO_P4 );
	int i;
        printf("***** Test Pin Value *******\n");
	for( i=0; i < 25; i++)
	{
	   printf("The current value of port XIO_P4: %s\n", get_pin_value( XIO_P4 ) );
	   sleep(2);
	}
	unexport_pin( XIO_P4 );
	return;
}
