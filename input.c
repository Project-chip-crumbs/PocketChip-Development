#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chip_xio.h"

void main( void )
{
	if( !chip_xio_start() )
	{
		printf("Initialization faild!\n");
		return;
	}

        if( !export_pin( XIO_P4 ) )
        {
		printf("Export failed!\n");
		return;
        }
	 else
		printf("Success\n");

	//printf( "current direction is: %s\n", get_pin_direction( XIO_P4) );
	if( strcmp( get_pin_direction( XIO_P4 ), "in") != 0 )
		set_pin_input( XIO_P4 );
	else
		printf("Pin is input\n");

	int i;
        printf("***** Test Pin Value *******\n");
	for( i=0; i < 25; i++)
	{
	   printf("The current value of port XIO_P4: %s\n", get_pin_value( XIO_P4 ) );
	   sleep(1);
	}
	unexport_pin( XIO_P4 );
}
