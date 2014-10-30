/* -*- c++ -*- */
/*
 * Copyright 2004 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * config.h is generated by configure.  It contains the results
 * of probing for features, options etc.  It should be the first
 * file included in your .cc file.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <logitech_27mhz_transceiver_key_src_b.h>
#include <gr_io_signature.h>



/*
 * Create a new instance of logitech_27mhz_transceiver_key_src_b and return
 * a boost shared_ptr.  This is effectively the public constructor.
 */
logitech_27mhz_transceiver_key_src_b_sptr 
logitech_27mhz_transceiver_make_key_src_b ()
{
  return logitech_27mhz_transceiver_key_src_b_sptr (new logitech_27mhz_transceiver_key_src_b ());
}

/*
 * Specify constraints on number of input and output streams.
 * This info is used to construct the input and output signatures
 * (2nd & 3rd args to gr_block's constructor).  The input and
 * output signatures are used by the runtime system to
 * check that a valid number and type of inputs and outputs
 * are connected to this block.  In this case, we accept
 * only 1 input and 1 output.
 */
static const int MIN_IN = 1;	// mininum number of input streams
static const int MAX_IN = 1;	// maximum number of input streams
static const int MIN_OUT = 1;	// minimum number of output streams
static const int MAX_OUT = 1;	// maximum number of output streams

/*
 * The private constructor
 */
logitech_27mhz_transceiver_key_src_b::logitech_27mhz_transceiver_key_src_b ()
  : gr_sync_block ("square2_ff",
		   gr_make_io_signature (0, 0, 0),
		   gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (char)))
{
  // nothing else required
}

/*
 * Our virtual destructor.
 */
logitech_27mhz_transceiver_key_src_b::~logitech_27mhz_transceiver_key_src_b ()
{
  // nothing else required
}




int
logitech_27mhz_transceiver_key_src_b::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{

//  printf("logitech_27mhz_transceiver_key_src_b %i\n", noutput_items);
  char *out = (char *) output_items[0];

  FILE * pFile,* pFile_wl, *pFile_rl;
  int c;
  bool finished = false;
  bool VERBOSE = false;
  int size=0;


  pFile_wl=fopen ("/tmp/key_src.lock","r");

  if (pFile_wl!= NULL) {
    fclose (pFile_wl);
    if (VERBOSE) printf ("key_src.lock exists\n");
  }
  else {
    // get the file size
    pFile=fopen ("/tmp/key_src.txt","r");
    if (pFile==NULL) pFile = fopen ( "/tmp/key_src.txt" , "w+" );
    fseek( pFile, 0, SEEK_END );
    size = ftell( pFile );
    if (size > 0) {
      if (VERBOSE) printf ("filesize %i\n", size);
      pFile_rl = fopen ( "/tmp/key_src.read_lock" , "w" );
      fclose (pFile_rl);
      rewind( pFile );

	  for (int i=0; i<= size; i++) {
		c = fgetc (pFile);
		out[i] =c;
		if (VERBOSE) printf ("out[%i] = %c\n",i,c);
	  }

	  finished = true;
	  fclose (pFile);
	  pFile = fopen ( "/tmp/key_src.txt" , "w" ); // clear file
	  fclose (pFile);
	  if( remove( "/tmp/key_src.read_lock" ) != 0 )
	    perror( "Error deleting file" );

    }
    else {
      if (VERBOSE) printf ("filesize 0\n");
      fclose (pFile);
    }
  }


  // Tell runtime system how many output items we produced.
  if (finished)
	return size;
  else {
	out[0] =0;
	return 1;     
  }


}
