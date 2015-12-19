#ifndef __DP566VAR_H__
#define __DP566VAR_H__

#include <sys/types.h>


#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*******************************************************
  File Name: dp566var.h
  @DOC
  REVISION LOG ENTRY
  Author: B.Mansoor Ahamed
  Revision By: B.Mansoor Ahamed
  Started  on 04/03/02  12:40 PM
  Comments: ...This file contains the Macro Definitions ,
  Variable declarations and Data Strucutres for the DP566 Project
  Copyright (C)2001 Data Patterns (India) Pvt. Ltd.
  All Rights reserved.
  No.6, Arya Gowdar Road, West Mambalam, Chennai,India - 600033
  Email:datapattern@vsnl.com
  Phone: 91-44-4837460 / 4848702
  FAX:   91-44-4848703
 ********************************************************/

// Definition of the Maximum Number of Boards allowed.
#define MAX_NUM_BOARDS						10
#define USE_INTERRUPT						1

// Defining Macro
// Macro for Board Number Checking.
#define CHECK_BRDNO(BoardNo)				\
	{										 \
		if(BoardNo > g_dwNoofCards)	  \
				return DP566_INVALID_CARD_NUMBER;	\
	}

// Macro for Channel Number Checking.
#define CHECK_CHANNO(ChannelNo)													\
	{																			 \
		if((in_usChannelNo < MIN_CHAN_NUM) || (in_usChannelNo > MAX_CHAN_NUM))	  \
				return DP566_INVALID_CHANNEL_NUMBER;										\
	}

#define CHECK_VALID_CHANNO(BoardNo, ChannelNo)	    \
	{												\
	if(ChannelNo > BoardDetails[BoardNo].usNodes)		\
		return DP566_CHANNEL_NOT_ACTIVE;														\
	}

// Macro for Windriver Handle Porperly opened.
#define CHECK_WINDRVR(WinDrvrHandle)		\
	{										 \
		if(WinDrvrHandle == WINDRIVER_HANDLE_FAILED)	  \
				return DRIVER_OPEN_ERROR;	\
	}


// Get the Offset value to identify the Interrupt location depending on the
// Channel Selected.
#define CHAN_INT_OFFSET(ChannelNo)			((ChannelNo == CHANNEL_1)? DPRAM_CHAN1_INT_REG : DPRAM_CHAN2_INT_REG)


// Get the Offset value to identify the Reset Location for both Channel 1 and Channel 2.
// Channel Selected.
#define CHAN_RESET_OFFSET(ChannelNo)		((ChannelNo == CHANNEL_1)? DPRAM_CHAN1_RESET_REG : DPRAM_CHAN2_RESET_REG)


//Definition for Getting the Bar address of DP-RAM depending on channel used.
#define CHAN_BAR_ADDR(ChannelNo)			((ChannelNo == CHANNEL_1)?(PCI1553KERNAL_ADDR)BAR5 : (PCI1553KERNAL_ADDR)BAR4)

// Structure Definition.
// Structure to hold the handle for all the available boards.
typedef struct BOARDDET
{
	volatile unsigned int uiPCIRegInfo[16];
	volatile unsigned int dwMem[3];
	volatile unsigned int dwIO;
	volatile unsigned int irqLine;
	volatile int irq_id;
	volatile int isInterruptEnabled[2];
	volatile unsigned short intStat[2][15];
	volatile unsigned short BrdStat[2];
	volatile unsigned short usIntStatWrP[2];
	volatile unsigned short usIntStatRdP[2];
	volatile unsigned int   usNodes;
	volatile unsigned int   dwLCRMem;
	volatile unsigned int   dwLCRPhMem;
	volatile unsigned short IntMaskVal[2];
	volatile unsigned short LCR;
	//volatile pid_t irq_proxy[2];
	volatile unsigned short irq_pulse_code[2];
}BOARDDET;

typedef struct tagCTRLWRD
{
	unsigned short RTtoRTFormat      :1;
	unsigned short BroadCastFormat   :1;
	unsigned short ModeCodeFormat    :1;
	unsigned short MIL_1553_A_B_Sel  :1;
	unsigned short EOMIntEnable      :1;
	unsigned short MaskBroadCastBit  :1;
	unsigned short OffLineSelfTest   :1;
	unsigned short BusChannelA_B     :1;
	unsigned short RetryEnabled      :1;
	unsigned short ReservedBitsMask  :1;
	unsigned short TerminalFlagMask  :1;
	unsigned short SubSysFlagMask    :1;
	unsigned short SubSysBusyMask    :1;
	unsigned short ServiceRqstMask   :1;
	unsigned short MsgErrorMask      :1;
	unsigned short Dummy             :1;

}tagCTRLWRD;


struct tagBRDDET
{
	unsigned char  ucBrdNo;
	unsigned char  ucChanNo;
};


// This structure is used to maintain the system status
typedef struct
{
	unsigned short CmdParamRdy       :1;
	unsigned short SelfTest          :1;
	unsigned short ModReady          :1;
	unsigned short NoError           :1;
	unsigned short CmdExecuted       :1;
	unsigned short ErrorInProcess    :1;
	unsigned short ParamError        :1;
	unsigned short ErrorInIntProcess :1;
	unsigned short  SequenceOver      :1;
	unsigned short reserved          :7;

}STATUS_REG_DP;


// Structures that are used by the RT Sequencing
typedef struct
{
	unsigned short RTNo              :5;
	unsigned short TerminalFlag      :1;
	unsigned short SubSystemFlag     :1;
	unsigned short ServiceRequest    :1;
	unsigned short Busy              :1;
	unsigned short DynaBusAccept     :1;
	unsigned short Dummy             :6;

}RTCONFIG;
// Type to be be changed to unsigned long from unsigned short

typedef struct
{
	unsigned short FrameNo;
	unsigned short NoOfMsgInFrame;
	unsigned short RepeatCount;

}tagFRAMEDET;

/* New member usFrameTime has been added */
typedef struct
{
	unsigned short FrameNo;
	unsigned short NoOfMsgInFrame;
	unsigned short RepeatCount;
	unsigned short usFrameTime;
}tagBCFRAMEDET;

typedef struct
{	unsigned short WrdCnt_ModeCode   :5;
	unsigned short SubAddr_ModeCode  :5;
	unsigned short Tx_Rx             :1;
	unsigned short RTAddress         :5;
}tagCMDWRD;

typedef struct
{
	unsigned short TxBuff1Empty      :1;
	unsigned short TxBuff2Empty      :1;
	unsigned short TxBuff3Empty      :1;
	unsigned short TxBuff4Empty      :1;
	unsigned short RxBuff1Full       :1;
	unsigned short RxBuff2Full       :1;
	unsigned short MTRxBuff1Full     :1;
	unsigned short MTRxBuff2Full     :1;
	unsigned short MTRxBuff3Full     :1;
	unsigned short MTRxBuff4Full     :1;
	unsigned short reserved          :6;
}tagBUFFSTS;

typedef struct
{
	unsigned short ACERAMFailed      :1;
	unsigned short EXTRAMFailed      :1;
	unsigned short Par_OutOfRng      :1;
	unsigned short BadCmd            :1;
	unsigned short Overflow          :1;
	unsigned short TxBuffUnderRuns   :1;
	unsigned short RXBuffOverFlows   :1;
	unsigned short NoFrameDetail     :1;
	unsigned short ConfigError       :1;
	unsigned short reserved          :7;

}tagERRORSTSTS;

// Structure used to store RT number and number of times an RT to be simulated in the sequencing
typedef struct
{
	unsigned short RTNo;
	unsigned short NoOfTimes;

}RTLIST;


// Used to store the busy table for all subaddresses of each RT
typedef struct
{
	unsigned short SA0_SA15;
	unsigned short SA16_SA31;

}BSY;
//}BUSY;


// Used to store the busy table for all subaddresses of each RT
typedef struct
{
	unsigned short WC0_WC15;
	unsigned short WC16_WC31;

}WCLEGAL;

// Used to hold the information about the XCV(Receive) table
typedef struct
{
	unsigned short RTNo;
	unsigned short SA;
	unsigned short WC;
	unsigned short XMTorXCV;

}XCV_TABLE_DET;

// BC Structures
// Structure specification
// Structure to specify the BC configuration
typedef struct
{
	unsigned short usRetry				:1;
	unsigned short usSingleorDouble		:1;
	unsigned short usFirstSameorAlt		:1;
	unsigned short usSecondSameorAlt	:1;
	unsigned short Reserved				:12;

}BCCONF;

// Structure to specify the BC Messages Details.
typedef struct
{
	unsigned short usBusAorB			:1;
	unsigned short usWordCount			:5;
	unsigned short usSrcSubAddress		:5;
	unsigned short usSrcRtAddress		:5;
	unsigned short Reserved				:3;
	unsigned short usMsgTransType		:3;
	unsigned short usDestSubAddress		:5;
	unsigned short usDestRtAddress		:5;
	unsigned short usMsgGapTime;
	unsigned short usData[32];
}BCMSG;


// Structure to specify the Resultant BC Messages.
typedef struct
{
	unsigned short usBlkStsWrd;
	unsigned short usTimeTagLSW;
	unsigned short usTimeTagMSW;
	unsigned short usCmdWrd;
	unsigned short usStsWrd1;
	unsigned short usStsWrd2;
	unsigned short usData[32];
}tagMSGRESULT;

// Structure for specifying the Message Type.
typedef struct
{
	unsigned short usCtrlWord;
	unsigned short usCommandWord_1;
	unsigned short usCommandWord_2;
	unsigned short usData[32];
	unsigned short usMsgGapTime;
	unsigned short Dummy1;
	unsigned short Dummy2;

}BCMSG_DET;

// Structure specifying the Frame Details.
typedef struct
{
	unsigned short usFrameLength;
	BCMSG_DET	   BcMsgDet[512];
	unsigned short usFrmRepeatTimes;
}BCFRAME_DET;


// Structure specifying the Frame Details.
typedef struct
{
	unsigned short usFrameLength;
	BCMSG		   *BcMsgDetails;
	unsigned short usFrmRepeatTimes;
}BCFRAME_STORE_DET;


// MT Structures.
// Structure Used by MT.
// Structure that specifies the Trigger Enable.
typedef struct
{
	unsigned short usMtStopOnMsg		:1;
	unsigned short usMtCmdStackSize		:2;
	unsigned short usMtDataStackSize	:3;
	unsigned short usTrigEnaborDisb		:1;
	unsigned short usStartonTrig		:1;
	unsigned short usStoponTrig			:1;
	unsigned short Reserved				:7;
}MTCONF;

// Structure to specify the Message Details.
typedef struct
{
	unsigned short usRTAddress			:5;
	unsigned short usSubAddress			:5;
	unsigned short usTransorRecv		:2;
	unsigned short Reserved				:4;
}MTMSG;

// Structure specifying the Resultant values.
typedef struct
{
	unsigned short usData[32];
	unsigned short usBlkStsWrd;
	unsigned short usIntStsWrd;
	unsigned short usCmdWrd;
	unsigned short usTimeTagWrd;
}MTRESULT_DATA;

// Structure to specify the Command Stack Location
typedef struct
{
	unsigned short usBlkStsWrd;
	unsigned short usTimeTagWrd;
	unsigned short usDataPtr;
	unsigned short usCmdWrd;
}MTCMDSTK_DET;

typedef struct
{
	unsigned long TerminalFlag      :1;
	unsigned long SubSystemFlag     :1;
	unsigned long ServiceRequest    :1;
	unsigned long Busy              :1;
	unsigned long DynaBusAccept     :1;
	unsigned long Reserved          :11;
}tagRTSTATUSWRD;

typedef union
{
	struct tagBRDDET  BrdDet;
	unsigned short	  usHandle;
}DP566Details;

typedef struct
{
	unsigned short usCtrlWord;
	unsigned short usCmdWrd_1;
	unsigned short usCmdWrd_2;
	unsigned short usMsgGapTime;
	unsigned short Reserved;

}tagBCMSG;


#if defined(__cplusplus) || defined(__cplusplus__)
	}
#endif


#endif  //__DP566VAR_H__
