/*--------------------------------------------------------------------------------------------
// Name: chip_xio.h
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
#ifndef CHIP_XIO_H_INCLUDED
#define CHIP_XIO_H_INCLUDED

// This will change based on kernal 
// version.  See note above.
#define BASE     1013
#define XIO_P0   BASE
#define XIO_P1   BASE+1
#define XIO_P2   BASE+2
#define XIO_P3   BASE+3
#define XIO_P4   BASE+4
#define XIO_P5   BASE+5
#define XIO_P6   BASE+6
#define XIO_P7   BASE+7

extern int   check_export_pin  ( int pin );
extern int   export_pin        ( int pin );
extern int   unexport_pin      ( int pin );
extern char* get_pin_direction ( int pin );
extern void  set_pin_output    ( int pin );
extern void  set_pin_input     ( int pin );
extern char* get_pin_value     ( int pin );
extern void  set_pin_high      ( int pin );
extern void  set_pin_low       ( int pin );

#endif
