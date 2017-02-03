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
// TODO: Error Handling.
// TODO: PORT Handling.
//--------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "chip_xio.h"


#define BUFFS              3
#define COMMAND_BUFF_SIZE  50

// This will change based on kernal
// version.  See note above.
// #define ADDRESS_SIZE       4

// Future port access
//int xio_port_arr[] = { XIO_P0, XIO_P1, XIO_P2, XIO_P3, XIO_P4, XIO_P5, XIO_P6, XIO_P7 };

char buff[BUFFS],command[COMMAND_BUFF_SIZE];

char* tok[] = {
		"/sys",
		"/class",
		"/gpio",
		"/export'",
		"/unexport'",
		"/value",
		"/direction",
		"sh -c 'echo",
		"out",
		"in",
		"1",
		"0",
		">",
		"'" };

#define EXPORT_DIR tok[0], tok[1], tok[2]

// In house helper functions
//-------------------------------------------------------------------------------------------------
// Create function which takes in all tokens and assembls them in that order
// using format string as guide.
unsigned int create_command( char* retstr, char* format, ... )
{
   *retstr = 0;
   va_list a_list;
   va_start( a_list, format );
   vsprintf( retstr, format, a_list );
   va_end( a_list );
   return strlen( retstr );
}
// Functions included in header file.
//----------------------------------------------------------------------------------------------------
int chip_xio_start( void  )
{
   if( getuid() != 0 )
   {
      printf("Sudo user required!\n");
      return -1;
   }
   if( check_export_dir() < 1 )
   {
      printf("Cannot access export dir: %s\n", command );
      return -1;
   }
   return 1;
}

// Check for valid address.
int is_xio_pin( int pin )
{
   int base_add;
   for( base_add = BASE; base_add < (BASE+8); base_add++ )
   {
	if( pin == base_add )
	   return 1;
   }
   printf("%d is not a valid XIO pin address\n", pin);
   return 0;
}
// Return base address
int get_xio_pin_base( )
{
   return BASE;
}
// Use to check for export directory.
// Return '1' found directory.  Return '0' no directory.
int check_export_dir( )
{
     struct stat info;
     create_command( command, "%s%s%s", EXPORT_DIR );
     if( stat( command, &info ) != 0 ) {
        return 0;  // No DIR access
        print("%s\n", command);
     }
     else if( info.st_mode & S_IFDIR )  //Is a DIR
        return 1;
     else {
        return 0;  //No DIR
        print("%s\n", command );
    }
}
// Use to check avalability of pin for export.
// Return '1' found directory.  Return '0' no directory.
int check_export_pin( int pin )
{
     if( !is_xio_pin( pin ) )
	return -1;
     struct stat info;
     create_command( command, "%s%s%s%s%d", EXPORT_DIR, tok[2], pin );
     if(stat( command, &info ) != 0)
        return 0;  // No DIR access
     else if(info.st_mode & S_IFDIR)  //Is a DIR
        return 1;
     else
        return 0;  //No DIR
}
// Export a pin. On ERROR = -1.
int export_pin( int pin )
{
     int i;
     i = check_export_pin( pin );
     if( i == -1 )
        return -1;
     else if( i == 1)
     {
        printf("Pin %s already in use!\n", pin );
	return -1;
     }
     create_command( command, "%s %d %s %s%s%s%s", tok[7], pin, tok[12],
	EXPORT_DIR, tok[3] );
     return system( command );
}
// Unexport a pin. On ERROR = -1.
int unexport_pin( int pin )
{
     if( !is_xio_pin( pin ) )
	return -1;
     create_command( command, "%s %d %s %s%s%s%s", tok[7], pin, tok[12],
	EXPORT_DIR, tok[4] );
     return system( command );
}
// Set the selected pin for output
int set_pin_output( int pin )
{
     if( !is_xio_pin( pin ) )
	return -1;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[8],
	tok[12], EXPORT_DIR, tok[2], pin, tok[6], tok[13] );
     system( command );
}
// Set the selected pin for input
int set_pin_input( int pin )
{
     if( !is_xio_pin( pin ) )
	return -1;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[9],
	tok[12], EXPORT_DIR, tok[2], pin, tok[6], tok[13] );
     system( command );
}
// Get the direction of the pin.  Input or output.
// Returns "in" or "out" for result.  Error "0"
// pin Address is bad.
char* get_pin_direction( int pin )
{
     if( !is_xio_pin( pin ) )
	return "-1";
     int fd;
     create_command( command, "%s%s%s%s%d%s", EXPORT_DIR, tok[2], pin, tok[6] );
     if(( fd = open( command, O_RDONLY, 0 )) == -1 )
        error( "get_pin_direction: can't open" );

     read( fd, buff, 3 );
     if( strncmp( buff, "out", 3 ) != 0 )
	buff[2] = 0;                            //Make sure to add NULL character after "in".

     close(fd);
     return &buff[0];
}
// Get the current value of the pin.
// Returns "1" or "0" for result.  Error "0"
// pin address is bad.
char *get_pin_value( int pin )
{
     if( !is_xio_pin( pin ) )
	return "-1";
     int fd;
     create_command( command, "%s%s%s%s%d%s", EXPORT_DIR, tok[2], pin, tok[5] );
     if(( fd = open( command, O_RDONLY, 0 )) == -1 )
        error( "get_pin_value: can't open" );

     buff[1] = 0;                               //Make sure to add NULL after "1" or "0".
     read( fd, buff, 1 );
     close(fd);
     return &buff[0];
}
// Set the selected output pin high.
int set_pin_high( int pin )
{
     if( !is_xio_pin( pin ) )
	return -1;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[10],
	tok[12], EXPORT_DIR, tok[2], pin, tok[5], tok[13] );
     system( command );
}
// Set the selected output pin low.
int set_pin_low( int pin )
{
     if( !is_xio_pin( pin ) )
	return -1;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[11],
	tok[12], EXPORT_DIR, tok[2] , pin, tok[5], tok[13] );
     system( command );
}
