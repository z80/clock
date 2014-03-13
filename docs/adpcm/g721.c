/*
 * This source code is a product of Sun Microsystems, Inc., modified
 * by CMU,  and is provided  for unrestricted use.  Users may copy
 * or modify this source code without  charge.
 *
*/

/*
 * g721.c
 *
 * Description:
 *
 * g721_encoder(), g721_decoder()
 *
 * These routines comprise an implementation of the CCITT G.721 ADPCM
 * coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which
 * take advantage of work station attributes, such as hardware 2's
 * complement arithmetic and large memory.  Specifically, certain time
 * consuming operations such as multiplications are replaced
 * with lookup tables and software 2's complement operations are
 * replaced with hardware 2's complement.
 *
 * The deviation from the bit level specification (lookup tables)
 * preserves the bit level performance specifications.
 *
 * As outlined in the G.721 Recommendation, the algorithm is broken
 * down into modules.  Each section of code below is preceded by
 * the name of the module which it is implementing.
 *
 */
#include <stdio.h>
#include <math.h>
#include <strings.h>
#include "g72x.h"

static short qtab_721[7] = {-124, 80, 178, 246, 300, 349, 400};
/*
 * Maps G.721 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static short	_dqlntab[16] = {-2048, 4, 135, 213, 273, 323, 373, 425,
				425, 373, 323, 273, 213, 135, 4, -2048};

/* Maps G.721 code word to log of scale factor multiplier. */
static short	_witab[16] = {-12, 18, 41, 64, 112, 198, 355, 1122,
				1122, 355, 198, 112, 64, 41, 18, -12};
/*
 * Maps G.721 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */

/*
 * g721_encoder()
 *
 * Encodes the input vale of linear PCM, A-law or u-law data sl and returns
 * the resulting code. -1 is returned for unknown input coding value.
 */
int
g721_encoder(
	int		sl,
	int		in_coding)
{
    static short	se = 0;
    static short        y0 = 544;
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
		sl >>= 2;			/* 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	d = sl - se;			/* estimation difference from block4l*/

	/* quantize the prediction difference */
        y = y0;
	i = quantize(d, y, qtab_721, 7);	/* i = ADPCM code */

	dq = reconstruct(i & 8, _dqlntab[i], y);	/* quantized est diff */

        y0 = y + (((_witab[i] << 5) - y) >> 5);

        if (y0 < 544)        y0 = 544;
        else if (y0 > 5120)  y0 = 5120;

/*        se = block4l((dq<0) ? -(dq+0x8000) : dq);  */
        se = block4l(dq);

	return (i);
}

/*
 * g721_decoder()
 *
 * Description:
 *
 * Decodes a 4-bit code of G.721 encoded data of i and
 * returns the resulting linear PCM, A-law or u-law value.
 * return -1 for unknown out_coding value.
 */
int
g721_decoder(
	int		i,
	int		out_coding)
{
    static short	se = 0;
    static short        y0 = 544;
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;

	i &= 0x0f;			/* mask to get proper bits */

        y = y0;
	dq = reconstruct(i & 0x08, _dqlntab[i], y); /* quantized diff. */

/*	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : se + dq;	   reconst. signal */
	sr =  se + dq;					/* reconst. signal */

        y0 = y + (((_witab[i] << 5) - y) >> 5);

        if (y0 < 544)        y0 = 544;
        else if (y0 > 5120)  y0 = 5120;

/*        se_bloc = block4l((dq<0) ? -(dq+0x8000) : dq);   */
        se = block4l(dq);
/*
	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/ sr was 14-bit dynamic range /
	default:
		return (-1);
	}
*/
		return (sr << 2);
}
