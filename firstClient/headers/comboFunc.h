/*
 * comboFunc.h
 *
 *  Created on: Feb 3, 2012
 *      Author: user
 */

#ifndef COMBOFUNC_H_
#define COMBOFUNC_H_

typedef struct
{
	// Structure of a block status word
	unsigned short cmdWrdContentErr :1;
	unsigned short rtRtSecondCmdErr :1;
	unsigned short rtRtAddrErr :1;
	unsigned short invWrd :1;
	unsigned short incSync :1;
	unsigned short wrdCntErr :1;
	unsigned short reserved :1;
	unsigned short dataStkRollover :1;
	unsigned short goodDataBlkTransfer :1;
	unsigned short noResTimeout :1;
	unsigned short formatErr :1;;
	unsigned short rtRtTransfer :1;
	unsigned short errFlag :1;
	unsigned short chBorA :1;
	unsigned short som :1;
	unsigned short eom :1;
}blkStsWrd;

#endif /* COMBOFUNC_H_ */
