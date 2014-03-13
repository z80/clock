/*
 * This source code is a product of Sun Microsystems, Inc., modified
 * by CMU,  and is provided  for unrestricted use.  Users may copy
 * or modify this source code without  charge.
 *
*/

/*
 * g72x.c
 *
 * Common routines for G.721 and G.723 conversions.
 */

#include "g72x.h"

static short power2[15] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
			0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000};

/*
 * quan()
 *
 * quantizes the input val against the table of size short integers.
 * It returns i if table[i - 1] <= val < table[i].
 *
 * Using linear search for simple coding.
 */
static int
quan_1(
	int		val,
	short		*table,
	int		size)
{
	int		i;

	for (i = 0; i < size; i++)
		if (val < *table++)
			break;
	return (i);
}

static int
quan(
	int		val,
	short		*table,
	int		size)
{
/*
	for (i = 0; i < size; i++)
		if (val < *table++)
			break;
	return (i);
*/
	 int		i=-1;
#define  STEP  i++; if(val < *table++) return (i);
         STEP; STEP; STEP; STEP; STEP;
         STEP; STEP; STEP; STEP; STEP;
         STEP; STEP; STEP; STEP; STEP;
}


/*
 * quantize()
 *
 * Given a raw sample, 'd', of the difference signal and a
 * quantization step size scale factor, 'y', this routine returns the
 * ADPCM codeword to which that sample gets quantized.  The step
 * size scale factor division operation is done in the log base 2 domain
 * as a subtraction.
 */
int
quantize(
	int		d,	/* Raw difference signal sample */
	int		y,	/* Step size multiplier */
	short		*table,	/* quantization table */
	int		size)	/* table size of short integers */
{
	short		dqm;	/* Magnitude of 'd' */
	short		exp;	/* Integer part of base 2 log of 'd' */
	short		mant;	/* Fractional part of base 2 log */
	short		dl;	/* Log of magnitude of 'd' */
	short		dln;	/* Step size scale factor normalized log */
	int		i;

	/*
	 * LOG
	 *
	 * Compute base 2 log of 'd', and store in 'dl'.
	 */
	dqm = abs(d);
	exp = quan(dqm >> 1, power2, 15);
	mant = ((dqm << 7) >> exp) & 0x7F;	/* Fractional portion. */
	dl = (exp << 7) + mant;

	/*
	 * SUBTB
	 *
	 * "Divide" by step size multiplier.
	 */
	dln = dl - (y >> 2);

	/*
	 * QUAN
	 *
	 * Obtain codword i for 'd'.
	 */
	i = quan_1(dln, table, size);
	if (d < 0)			/* take 1's complement of i */
		return ((size << 1) + 1 - i);
	else if (i == 0)		/* take 1's complement of 0 */
		return ((size << 1) + 1); /* new in 1988 */
	else
		return (i);
}
/*
 * reconstruct()
 *
 * Returns reconstructed difference signal 'dq' obtained from
 * codeword 'i' and quantization step size scale factor 'y'.
 * Multiplication is performed in log base 2 domain as addition.
 */
int
reconstruct(
	int		sign,	/* 0 for non-negative value */
	int		dqln,	/* G.72x codeword */
	int		y)	/* Step size multiplier */
{
	short		dql;	/* Log of 'dq' magnitude */
	short		dex;	/* Integer part of log */
	short		dqt;
	short		dq;	/* Reconstructed difference signal sample */

	dql = dqln + (y >> 2);	/* ADDA */

	if (dql < 0) {
/*		return ((sign) ? -0x8000 : 0);   */
		return (0);
	} else {		/* ANTILOG */
		dex = (dql >> 7) & 15;
		dqt = 128 + (dql & 127);
		dq = (dqt << 7) >> (14 - dex);
/*		return ((sign) ? (dq - 0x8000) : dq);  */
		return ((sign) ? -dq : dq);
	}
}


/**************************** BLOCK 4L *******************************/

int block4l (dl)
short dl ;
{
static int sl = 0 ;
        int wd1, wd2, wd3, wd4, wd5;
        static int spl = 0 ;
        static int szl = 0 ;
        static int rlt  [3] = { 0, 0, 0 } ;
        static int al   [3] = { 0, 0, 0 } ;
        static int plt  [3] = { 0, 0, 0 } ;
        static int dlt  [7] = { 0, 0, 0, 0, 0, 0, 0 } ;
        static int bl   [7] = { 0, 0, 0, 0, 0, 0, 0 } ;
        static int sg   [7] = { 0, 0, 0, 0, 0, 0, 0 } ;
/****************** pointer ****************************/
        register int *sgp, *pltp, *alp, *dltp, *blp, *rltp;
        register int *pltp_1, *dltp_1, *rltp_1;
/*************************************** BLOCK 4L, RECONS ***********/
        *dlt = dl;

        *rlt = sl + dl ;
/*
        if ( *rlt > 32767 ) *rlt = 32767;
        else if ( *rlt < -32768 ) *rlt = -32768;
*/
/*************************************** BLOCK 4L, PARREC ***********/

        *plt = dl + szl ;
/*
        if ( plt[0] > 32767 ) plt[0] = 32767;
        else if ( plt[0] < -32768 ) plt[0] = -32768;
*/
/*****************************BLOCK 4L, UPPOL2*************************/
        sgp = sg, pltp = plt, alp = al ;
                *sgp++ = *pltp++ >> 15 ;
                *sgp++ = *pltp++ >> 15 ;
                *sgp++ = *pltp++ >> 15 ;

        wd1 = *++alp << 2;

        if ( wd1 > 32767 ) wd1 = 32767;
        else if ( wd1 < -32768 ) wd1 = -32768;

        wd2= ( *sg == *(sg+1) )?  -wd1: wd1 ;
        if ( wd2 > 32767 ) wd2 = 32767;

        wd2 = wd2 >> 7 ;
        wd3= ( *sg == *(sg+2) )? 128: -128 ;
        wd4 = wd2 + wd3 ;
        wd5 = (*++alp * 32512) >> 15 ;

        *alp = wd4 + wd5 ;

        if ( *alp  >  12288 )  *alp =  12288 ;
        else if ( *alp  < -12288 )  *alp = -12288 ;

/************************************* BLOCK 4L, UPPOL1 ***************/

        *sg = *plt >> 15 ;
        *(sg+1) = *(plt+1) >> 15 ;
        wd1 = ( *sg == *(sg+1) )?  192 : -192 ;

        wd2 = (*--alp * 32640) >> 15 ;

        *alp = wd1 + wd2 ;
/*
        if ( *alp > 32767 ) *alp = 32767;
        else if ( *alp < -32768 ) *alp = -32768;
*/
        wd3 = (15360 - *++alp) ;
/*
        if ( wd3 > 32767 ) wd3 = 32767;
        else if ( wd3 < -32768 ) wd3 = -32768;
*/
        if ( *--alp >  wd3)  *alp =  wd3 ;
        else if ( *alp  < -wd3)  *alp = -wd3 ;

/*************************************** BLOCK 4L, UPZERO ************/
        wd1 = ( dl == 0 ) ? 0 : 128;
        *sg = dl >> 15 ;
        sgp = sg, dltp = dlt, blp = bl;

                *++sgp = *++dltp >> 15 ;
                wd2 = ( *sgp == *sg ) ? wd1 : -wd1 ;
                wd3 = (*++blp * 32640) >> 15 ;
                *blp = wd2 + wd3 ;
/*
                if ( *blp > 32767 ) *blp = 32767;
                else if ( *blp < -32768 ) *blp = -32768;
*/

                *++sgp = *++dltp >> 15 ;
                wd2 = ( *sgp == *sg ) ? wd1 : -wd1 ;
                wd3 = (*++blp * 32640) >> 15 ;
                *blp = wd2 + wd3 ;
/*
                if ( *blp > 32767 ) *blp = 32767;
                else if ( *blp < -32768 ) *blp = -32768;
*/

                *++sgp = *++dltp >> 15 ;
                wd2 = ( *sgp == *sg ) ? wd1 : -wd1 ;
                wd3 = (*++blp * 32640) >> 15 ;
                *blp = wd2 + wd3 ;
/*
                if ( *blp > 32767 ) *blp = 32767;
                else if ( *blp < -32768 ) *blp = -32768;
*/

                *++sgp = *++dltp >> 15 ;
                wd2 = ( *sgp == *sg ) ? wd1 : -wd1 ;
                wd3 = (*++blp * 32640) >> 15 ;       /* for g721_40, use 32704 instead of 32640 */
                *blp = wd2 + wd3 ;
/*
                if ( *blp > 32767 ) *blp = 32767;
                else if ( *blp < -32768 ) *blp = -32768;
*/

                *++sgp = *++dltp >> 15 ;
                wd2 = ( *sgp == *sg ) ? wd1 : -wd1 ;
                wd3 = (*++blp * 32640) >> 15 ;
                *blp = wd2 + wd3 ;
/*
                if ( *blp > 32767 ) *blp = 32767;
                else if ( *blp < -32768 ) *blp = -32768;
*/

                *++sgp = *++dltp >> 15 ;
                wd2 = ( *sgp == *sg ) ? wd1 : -wd1 ;
                wd3 = (*++blp * 32640) >> 15 ;
                *blp = wd2 + wd3 ;
/*
                if ( *blp > 32767 ) *blp = 32767;
                else if ( *blp < -32768 ) *blp = -32768;
*/

/********************************* BLOCK 4L, DELAYA ******************/
        dltp_1 = dltp - 1;
                *dltp--  = *dltp_1-- ;
                *dltp--  = *dltp_1-- ;
                *dltp--  = *dltp_1-- ;
                *dltp--  = *dltp_1-- ;
                *dltp--  = *dltp_1-- ;
                *dltp--  = *dltp_1-- ;

        rltp =rlt+2, pltp = plt+2 ;
        rltp_1 = rltp - 1, pltp_1 = pltp - 1;

                *rltp-- = *rltp_1--;
                *pltp-- = *pltp_1--;
                *rltp-- = *rltp_1--;
                *pltp-- = *pltp_1--;
/********************************* BLOCK 4L, FILTEP ******************/

        wd1 = ( *alp * *++rltp ) >> 14 ;

        wd2 = ( *++alp * *++rltp ) >> 14 ;

        spl = wd1 + wd2 ;
/*
        if ( spl > 32767 ) spl = 32767;
        else if ( spl < -32768 ) spl = -32768;
*/
/*************************************** BLOCK 4L, FILTEZ ***********/

        blp = blp - 6;
                szl = (*++blp * *++dltp) >> 14 ;
/*
                if ( szl > 32767 ) szl = 32767;
                else if ( szl < -32768 ) szl = -32768;
*/
                szl += (*++blp * *++dltp) >> 14 ;
/*
                if ( szl > 32767 ) szl = 32767;
                else if ( szl < -32768 ) szl = -32768;
*/
                szl += (*++blp * *++dltp) >> 14 ;
/*
                if ( szl > 32767 ) szl = 32767;
                else if ( szl < -32768 ) szl = -32768;
*/
                szl += (*++blp * *++dltp) >> 14 ;
/*
                if ( szl > 32767 ) szl = 32767;
                else if ( szl < -32768 ) szl = -32768;
*/
                szl += (*++blp * *++dltp) >> 14 ;
/*
                if ( szl > 32767 ) szl = 32767;
                else if ( szl < -32768 ) szl = -32768;
*/
                szl += (*++blp * *++dltp) >> 14 ;
/*
                if ( szl > 32767 ) szl = 32767;
                else if ( szl < -32768 ) szl = -32768;
*/
/*********************************BLOCK 4L, PREDIC *******************/

        sl = spl + szl ;
/*
        if ( sl > 32767 ) sl = 32767;
        else if ( sl < -32768 ) sl = -32768;
*/

        return (sl) ;
}

/*
 * tandem_adjust(sr, se, y, i, sign)
 *
 * At the end of ADPCM decoding, it simulates an encoder which may be receiving
 * the output of this decoder as a tandem process. If the output of the
 * simulated encoder differs from the input to this decoder, the decoder output
 * is adjusted by one level of A-law or u-law codes.
 *
 * Input:
 *	sr	decoder output linear PCM sample,
 *	se	predictor estimate sample,
 *	y	quantizer step size,
 *	i	decoder input code,
 *	sign	sign bit of code i
 *
 * Return:
 *	adjusted A-law or u-law compressed sample.
 */
int
tandem_adjust_alaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	short		*qtab)
{
	unsigned char	sp;	/* A-law compressed 8-bit code */
	short		dx;	/* prediction error */
	char		id;	/* quantized prediction error */
	int		sd;	/* adjusted A-law decoded sample value */
	int		im;	/* biased magnitude of i */
	int		imx;	/* biased magnitude of id */

	if (sr <= -32768)
		sr = -1;
	sp = linear2alaw((sr >> 1) << 3);	/* short to A-law compression */
	dx = (alaw2linear(sp) >> 2) - se;	/* 16-bit prediction error */
	id = quantize(dx, y, qtab, sign - 1);

	if (id == i) {			/* no adjustment on sp */
		return (sp);
	} else {			/* sp adjustment needed */
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;

		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80) {
				sd = (sp == 0xD5) ? 0x55 :
				    ((sp ^ 0x55) - 1) ^ 0x55;
			} else {
				sd = (sp == 0x2A) ? 0x2A :
				    ((sp ^ 0x55) + 1) ^ 0x55;
			}
		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0xAA) ? 0xAA :
				    ((sp ^ 0x55) + 1) ^ 0x55;
			else
				sd = (sp == 0x55) ? 0xD5 :
				    ((sp ^ 0x55) - 1) ^ 0x55;
		}
		return (sd);
	}
}

int
tandem_adjust_ulaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	short		*qtab)
{
	unsigned char	sp;	/* u-law compressed 8-bit code */
	short		dx;	/* prediction error */
	char		id;	/* quantized prediction error */
	int		sd;	/* adjusted u-law decoded sample value */
	int		im;	/* biased magnitude of i */
	int		imx;	/* biased magnitude of id */

	if (sr <= -32768)
		sr = 0;
	sp = linear2ulaw(sr << 2);	/* short to u-law compression */
	dx = (ulaw2linear(sp) >> 2) - se;	/* 16-bit prediction error */
	id = quantize(dx, y, qtab, sign - 1);
	if (id == i) {
		return (sp);
	} else {
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;
		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80)
				sd = (sp == 0xFF) ? 0x7E : sp + 1;
			else
				sd = (sp == 0) ? 0 : sp - 1;

		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0x80) ? 0x80 : sp - 1;
			else
				sd = (sp == 0x7F) ? 0xFE : sp + 1;
		}
		return (sd);
	}
}
