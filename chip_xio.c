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
//--------------------------------------------------------------------------
// Return: String length
//
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

// Check for superuser assess.
// Check for export directory
//---------------------------
// Return: 0 on error; 1 on success.
//
int chip_xio_start( void  )
{
   if( getuid() != 0 )
   {
      perror("Sudo user required!\n");
      return 0;
   }
   if( check_export_dir() < 0 )
   {
      printf("Cannot access export dir: %s\n", command );
      return 0;
   }
   return 1;
}

// Check for valid address.
//-------------------------
// Return: 0 on error; 1 on success
//
int is_xio_pin( int pin )
{
   int base_add;
   for( base_add = BASE; base_add < (BASE+8); base_add++ )
   {
	if( pin == base_add )
	{
	   //printf("Valid Address!\n");
	   return 1;
        }
   }
   printf("%d is not a valid XIO pin address\n", pin);
   return 0;
}
// Get xio base address.
//---------------------
// Return: BASE from header file.
//
int get_xio_pin_base( )
{
   return BASE;
}
// Use to check for export directory.
// ----------------------------------
// Return: -1 no export dir; 0 export dir found.
//
int check_export_dir( )
{
     int status;
     struct stat info;
     create_command( command, "%s%s%s", EXPORT_DIR );
     status = lstat( command, &info );

     //perror( "stat\n" );
     //printf( "return status: %d\n", status );

     if( status == -1  )
     {
        printf("Export directory not found!\n");
	return -1;
     }
     else
     {
        printf("Export directory found!\n");
	return 0;
     }
}
// Use to check avalability of pin for export.
// -------------------------------------------
// Return: -1 pin dir not found; 0 pin dir found.
//
int check_export_pin( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return 0;
     }
     struct stat info;
     int status;
     create_command( command, "%s%s%s%s%d", EXPORT_DIR, tok[2], pin );
     status = lstat( command, &info );

     //perror( "stat\n" );
     //printf( "return status: %d\n", status );

     if( status == -1  )
     {
        printf( "%d pin directory not found!\n", pin );
	return -1;
     }
     else
     {
        printf( "%d pin directory Found!\n", pin );
	return 0;
     }
}
// Export a pin.
//--------------
// Return: 0 on error; 1 on success
//
int export_pin( int pin )
{
     int status;
     status = check_export_pin( pin );
     if( status == -1 )
     {
        create_command( command, "%s %d %s %s%s%s%s", tok[7], pin, tok[12], EXPORT_DIR, tok[3] );
        status = system( command ); // system function returns 0 on success change to 1.
	if( status == -1 )
	{
	   perror("System command failed: export_pin!\n");
	   status = 0;
	} else {
	   //printf("Exporting pin\n");
	   return 1;
        }
     }
     return status;
}
// Unexport a pin.
//-----------------
// Return: 0 on error; 1 on success
//
int unexport_pin( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return 0;
     }
     int status;
     create_command( command, "%s %d %s %s%s%s%s", tok[7], pin, tok[12], EXPORT_DIR, tok[4] );
     status = system( command ); // system function returns 0 on success change to 1.
     if( status == -1 )
     {
        perror("System command failed: unexport_pin!\n");
	status = 0;
     } else {
	//printf("Unexport pin\n");
	return 1;
     }
}
// Set the selected pin for output.
//---------------------------------
// Return: 0 on error; 1 on success
//
int set_pin_output( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return 0;
     }
     int status;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[8],
	tok[12], EXPORT_DIR, tok[2], pin, tok[6], tok[13] );
     status = system( command );
     if( status == -1 )
     {
        perror("System command failed: set_pin_output!\n");
	status = 0;
     } else {
	//printf("Pin output mode success!\n");
	return 1;
     }
}
// Set the selected pin for input
//-------------------------------
// Return: 0 on error; 1 on success
//
int set_pin_input( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return 0;
     }
     int status;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[9],
	tok[12], EXPORT_DIR, tok[2], pin, tok[6], tok[13] );
     status = system( command );
     if( status == -1 )
     {
        perror("System command failed: set_pin_input!\n");
	status = 0;
     } else {
	//printf("Pin input mode success\n!");
	return 1;
     }
}
// Get the direction of the pin.  Input or output.
//------------------------------------------------
// Returns: NULL string on error; "in" or "out" string on success
// 
char* get_pin_direction( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return NULL;
     }
     int fd;
     create_command( command, "%s%s%s%s%d%s", EXPORT_DIR, tok[2], pin, tok[6] );
     if(( fd = open( command, O_RDONLY, 0 )) == -1 )
     {
        perror( "get_pin_direction: can't open" );
	return NULL;
     }
     read( fd, buff, 3 );
     if( strncmp( buff, "out", 3 ) != 0 )
     {
	buff[2] = 0;                            //Make sure to add NULL character after "in".
     }
     close(fd);
     return &buff[0];
}
// Get the current value of the pin.
//----------------------------------
// Returns: NULL string on error; "1" or "0" string on success
// 
char *get_pin_value( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return NULL;
     }
     int fd;
     create_command( command, "%s%s%s%s%d%s", EXPORT_DIR, tok[2], pin, tok[5] );
     if(( fd = open( command, O_RDONLY, 0 )) == -1 )
     {
        perror( "get_pin_value: can't open" );
	return NULL;
     }
     buff[1] = 0;                               //Make sure to add NULL after "1" or "0".
     read( fd, buff, 1 );
     close(fd);
     return &buff[0];
}
// Set the selected output pin high.
//----------------------------------
// Return:  0 on error; 1 on success
//
int set_pin_high( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return 0;
     }
     int status;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[10],
	tok[12], EXPORT_DIR, tok[2], pin, tok[5], tok[13] );
     status = system( command );
     if( status == -1 )
     {
        perror("System command failed: set_pin_high!\n");
	return 0;
     } else {
	//printf("Set output pin high success\n!");
	return 1;
     }
}
// Set the selected output pin low.
//----------------------------------
// Return:  0 on error; 1 on success
//
int set_pin_low( int pin )
{
     if( !is_xio_pin( pin ) )
     {
	return 0;
     }
     int status;
     create_command( command, "%s %s %s %s%s%s%s%d%s%s", tok[7], tok[11],
	tok[12], EXPORT_DIR, tok[2] , pin, tok[5], tok[13] );
     status = system( command );
     if( status == -1 )
     {
        perror("System command failed: set_pin_low!\n");
	return 0;
     } else {
	//printf("Set output pin low success\n!");
	return 1;
     }
}
