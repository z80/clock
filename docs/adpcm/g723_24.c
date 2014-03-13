/*
 * This source code is a product of Sun Microsystems, Inc., modified
 * by CMU,  and is provided  for unrestricted use.  Users may copy
 * or modify this source code without  charge.
 *
*/

/*
 * g723_24.c
 *
 * Description:
 *
 * g723_24_encoder(), g723_24_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 24 Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which take advantage
 * of workstation attributes, such as hardware 2's complement arithmetic.
 *
 */
#include "g72x.h"

/*
 * Maps G.723_24 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static short	_dqlntab[8] = {-2048, 135, 273, 373, 373, 273, 135, -2048};

/* Maps G.723_24 code word to log of scale factor multiplier. */
static short	_witab[8] = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};

/*
 * Maps G.723_24 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short	_fitab[8] = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};

static short qtab_723_24[3] = {8, 218, 331};

/*
 * g723_24_encoder()
 *
 * Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
 * Returns -1 if invalid input coding value.
 */
int
g723_24_encoder(
	int		sl,
	int		in_coding)
{
     static short      se = 0;			/* ACCUM */
     static short      y0 = 544;	        /* INIT MIX */	
	short		d;			/* SUBTA */
	short		y;			/* MIX */
	short		dq, i;

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;		/* sl of 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	d = sl - se;			/* d = estimation diff. */

	/* quantize prediction difference d */
	y = y0;					/* quantizer step size */
	i = quantize(d, y, qtab_723_24, 3);	/* i = ADPCM code */
	dq = reconstruct(i & 4, _dqlntab[i], y); /* quantized diff. */


        y0 = y + ((_witab[i] - y) >> 5);

        if (y0 < 544)        y0 = 544;
        else if (y0 > 5120)  y0 = 5120;

/*        se = block4l((dq<0) ? -(dq+0x8000) : dq);    */
        se = block4l(dq);


	return (i);
}

/*
 * g723_24_decoder()
 *
 * Decodes a 3-bit CCITT G.723_24 ADPCM code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int
g723_24_decoder(
	int		i,
	int		out_coding)
{
	static short	se = 0;			/* ACCUM */
	static short	y0 = 544;		/*  */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;

	i &= 0x07;			/* mask to get proper bits */

	y = y0;					/* adaptive quantizer step size */
	dq = reconstruct(i & 0x04, _dqlntab[i], y); /* unquantize pred diff */

/*	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq);     reconst. signal */
        sr =  se + dq;                                  /* reconst. signal */

        y0 = y + ((_witab[i] - y) >> 5);

        if (y0 < 544)        y0 = 544;
        else if (y0 > 5120)  y0 = 5120;

/*        se_bloc = block4l((dq<0) ? -(dq+0x8000) : dq);          */
        se = block4l(dq);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	default:
		return (-1);
	}
}
