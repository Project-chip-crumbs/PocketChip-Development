/*--------------------------------------------------------------------------------------------
//Copyright (c) 2017 Ovide N. Mercure
//Name: chip_xio
//Synopsis: chip_xio is a basic application to drive the xio port on the C.H.I.P.  Layered on
//          the existing sysfs implementation for NON-RTA pin driving.
//          At this time it only controls individual pin input or output.  This code is not
//          portable with other OS's without rewrite.
//
// Please note this software is not warrantied and any resulting damage from its use is not
// the writers responsibility.  Use at your own risk!
---------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BASE     1013
#define XIO_P0   BASE
#define XIO_P1   BASE+1
#define XIO_P2   BASE+2
#define XIO_P3   BASE+3
#define XIO_P4   BASE+4
#define XIO_P5   BASE+5
#define XIO_P6   BASE+6
#define XIO_P7   BASE+7
#define BUFFS    3
#define ADDRESS_SIZE 4

// Future port access
//int xio_port_arr[] = { XIO_P0, XIO_P1, XIO_P2, XIO_P3, XIO_P4, XIO_P5, XIO_P6, XIO_P7 };

char buff[BUFFS];

// Used to check avalability of pin for export.
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
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
        printf("Setting XIO_P1 low\n");
        set_pin_low( XIO_P1 );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );
        sleep( 5 );
      }
	printf( "Setting XIO_P0 high\n");
        set_pin_high( XIO_P0 );
        printf( "XIO_P0 Pin Value is: %s\n", get_pin_value( XIO_P0 ) );
	printf( "Setting XIO_P1 high\n");
        set_pin_high( XIO_P1 );
        printf( "XIO_P1 Pin Value is: %s\n", get_pin_value( XIO_P1 ) );

     printf( "Unexport pin XIO_P1\n" );
     unexport_pin( XIO_P1 );
     printf( "Unexport pin XIO_P0\n" );
     unexport_pin( XIO_P0 );
}
