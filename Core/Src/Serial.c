#include "main.h"
#include "stdio.h"
#include "Serial.h"
#include "Defines.h"
#include "Hardware.h"
#include "Menu.h"
#include "PB_Num.h"
#include "Variable.h"


void CAB_SerialDataTransmition(void)
{
	
	//////////////////////////////
	///// Send Data To Cabin /////
	//////////////////////////////
	if(CAB_TX_AfterLastByteTimer > 20) //There is 20ms rest time between both packet
	{
		CAB_TX_AfterLastByteTimer = 0;
		
		CAB_TX_Buffer[0] = 0xFF;
		CAB_TX_Buffer[1] = 0xFE;
		CAB_TX_Buffer[2] = CAB_SERIAL_ID;
		CAB_TX_Buffer[3] = 27; //Packet Len
		
		CAB_TX_Buffer[4] = NumR; 				//0
		CAB_TX_Buffer[5] = NumL;				//1
		CAB_TX_Buffer[6] = SevenBitToByte(SegUp,SegDn,Safety_90,Safety_71,Safety_66,Safety_69,Safety_68,LandingF);
		CAB_TX_Buffer[7] = SevenBitToByte(COut.Door1Close,COut.Door2Close,COut.Door3Close,COut.Door1Open,COut.Door2Open,COut.Door3Open,CMD_URA,StandbyF);
		CAB_TX_Buffer[8] = CAB_TX_Buffer[7];
		
		_Bool WelcomeF = (Stng[WELCOME_FLOOR] == E_Level);
		CAB_TX_Buffer[9] 	=	SevenBitToByte(WelcomeF,RunOrStop,COut.Fan,COut.Ovl,COut.Gang,COut.Ava,0,0) ;
		CAB_TX_Buffer[10]	=	SevenBitToByte(COut.RES1,COut.RES2,COut.RES3,COut.RES4,COut.RES5,0,0,0) | (Stng[GANG_SELECT] & 0x07);
		CAB_TX_Buffer[11]	=	CMD_MP3File;
		CAB_TX_Buffer[12]	=	Floor[E_Level].Talk;//Stng[TALK_ADD + E_Level];
		CAB_TX_Buffer[13]	=	((Stng[MUSIC_VOLUME] << 4) & 0xF0) | (Stng[TALK_VOLUME] & 0x0F);
		CAB_TX_Buffer[14]	=	E_Level;
		CAB_TX_Buffer[15]	=	PB.CabinSet;
		CAB_TX_Buffer[16]	=	PB.CabinClear;
		CAB_TX_Buffer[17]	=	PB.CabinBlinker[0];
		CAB_TX_Buffer[18]	=	PB.CabinBlinker[1];
		CAB_TX_Buffer[19]	=	PB.CabinBlinker[2];
		CAB_TX_Buffer[20]	=	CAB_TransferHWAdd[TransferHWAddCounter];	
		CAB_TX_Buffer[21]	=	CAB_TX_Buffer[20];
		CAB_TX_Buffer[22]	=	TransferHWAddCounter;
		CAB_TX_Buffer[23]	=	CAB_TX_Buffer[22];
		CAB_TX_Buffer[24] = CAB_TransferHWAdd[TransferHWAddCounter+1];
		CAB_TX_Buffer[25] = CAB_TX_Buffer[24]; //Reserved //21
		
		TransferHWAddCounter+=2;
		if(TransferHWAddCounter > 68) {TransferHWAddCounter =0;}
		
		uint8_t Checksum = CAB_TX_Buffer[4];
		for(uint8_t i=5;i<26;i++) {Checksum ^= CAB_TX_Buffer[i];}
		CAB_TX_Buffer[26]= Checksum;
		CAB_TX_Buffer[27]=0;
		
		CAB_SendData(28);
		
		CAB_SendDataCounter++;
	}

	//////////////////////////////
	///// Get Data From Main /////
	//////////////////////////////
	while(CAB_RX_DMA_CNT != CAB_RXDataCNT)
	{
		uint8_t DataIn = CAB_RX_Buffer[CAB_RXDataCNT];
		
		if(DataIn == 0xFF && CAB_SyncStatus == 0) 	{CAB_PacketLen = 0;	CAB_SyncStatus++;}
		else if(CAB_SyncStatus == 0)								{CAB_PacketLen = 0;}
		else if(DataIn == 0xFE && CAB_SyncStatus == 1) 	{CAB_PacketLen = 0;	CAB_SyncStatus++;}
		else if(CAB_SyncStatus == 2)
		{
			if(DataIn == CAB_SERIAL_ID) 							{CAB_PacketLen = 0; CAB_SyncStatus++;}
			else {CAB_SyncStatus = 0;} //ID doesn't match
		}
		else if(CAB_SyncStatus == 3) {CAB_PacketLen = DataIn;	CAB_SyncStatus++; CAB_PacketCounter = 0;}
		else if(CAB_SyncStatus < CAB_PacketLen && CAB_SyncStatus > 3)
		{
			CAB_ReceivedByte[CAB_PacketCounter] = DataIn;
			CAB_PacketCounter++;
			CAB_SyncStatus++;
		}
		else
		{
			if(CAB_SyncStatus == CAB_PacketLen && CAB_PacketLen > 0 && CAB_PacketCounter == 38)
			{
				uint8_t	Cheksum = CAB_ReceivedByte[0];
				for(uint16_t iw=1;iw < CAB_PacketLen;iw++) 
				{
					Cheksum ^= CAB_ReceivedByte[iw];
				}
				
				if(DataIn == Cheksum ) 
				{
					CAB_NewPacketF = 1;
					CAB_GetDataCounter ++;
					CAB_GetDataTimer = 0;
				}
			}
			
			if(CAB_NewPacketF == 0) {CAB_PacketError ++;}
			CAB_SyncStatus = 0;
			CAB_PacketLen = 0;
		}
		
		
		if(CAB_NewPacketF)
		{
			for(uint8_t i=0;i<8;i++)
			{
				PB.Cabin[i]				=	CAB_ReceivedByte[0] & 0x01;
				PB.Cabin[i + 8]		=	CAB_ReceivedByte[1] & 0x01;
				PB.Cabin[i + 16]	=	CAB_ReceivedByte[2] & 0x01;
				PB.Cabin[i + 24]	=	CAB_ReceivedByte[3] & 0x01;
				PB.Cabin[i + 32]	=	CAB_ReceivedByte[4] & 0x01;
				PB.Cabin[i + 40]	=	CAB_ReceivedByte[5] & 0x01;
				PB.Cabin[i + 48]	=	CAB_ReceivedByte[6] & 0x01;
				PB.Cabin[i + 56]	=	CAB_ReceivedByte[7] & 0x01;
				PB.Cabin[i + 64]	=	CAB_ReceivedByte[8] & 0x01;
				PB.Cabin[i + 72]	=	CAB_ReceivedByte[9] & 0x01;
				PB.Cabin[i + 80]	=	CAB_ReceivedByte[10] & 0x01;
				PB.Cabin[i + 88]	=	CAB_ReceivedByte[11] & 0x01;
				PB.Cabin[i + 96]	=	CAB_ReceivedByte[12] & 0x01;
				PB.Cabin[i + 104]	=	CAB_ReceivedByte[13] & 0x01;
				PB.Cabin[i + 112]	=	CAB_ReceivedByte[14] & 0x01;
				
				PB.Cabin_Cancel[i]				=	CAB_ReceivedByte[15] & 0x01;
				PB.Cabin_Cancel[i + 8]		=	CAB_ReceivedByte[16] & 0x01;
				PB.Cabin_Cancel[i + 16]		=	CAB_ReceivedByte[17] & 0x01;
				PB.Cabin_Cancel[i + 24]		=	CAB_ReceivedByte[18] & 0x01;
				PB.Cabin_Cancel[i + 32]		=	CAB_ReceivedByte[19] & 0x01;
				PB.Cabin_Cancel[i + 40]		=	CAB_ReceivedByte[20] & 0x01;
				PB.Cabin_Cancel[i + 48]		=	CAB_ReceivedByte[21] & 0x01;
				PB.Cabin_Cancel[i + 56]		=	CAB_ReceivedByte[22] & 0x01;
				PB.Cabin_Cancel[i + 64]		=	CAB_ReceivedByte[23] & 0x01;
				PB.Cabin_Cancel[i + 72]		=	CAB_ReceivedByte[24] & 0x01;
				PB.Cabin_Cancel[i + 80]		=	CAB_ReceivedByte[25] & 0x01;
				PB.Cabin_Cancel[i + 88]		=	CAB_ReceivedByte[26] & 0x01;
				PB.Cabin_Cancel[i + 96]		=	CAB_ReceivedByte[27] & 0x01;
				PB.Cabin_Cancel[i + 104]	=	CAB_ReceivedByte[28] & 0x01;
				PB.Cabin_Cancel[i + 112]	=	CAB_ReceivedByte[29] & 0x01;
				
				//if(i > 0)
				//{
					for(int8_t j=0;j<30;j++)
					{
						CAB_ReceivedByte[j] >>=1;
					}
				//}
			}
			
			CIn.Ovl	=	CAB_ReceivedByte[30] & 0x80;
			CIn.Ful	=	CAB_ReceivedByte[30] & 0x40;
			CIn.RES1	=	CAB_ReceivedByte[30] & 0x20;
			CIn.RES2	=	CAB_ReceivedByte[30] & 0x10;
			CIn.RES3	=	CAB_ReceivedByte[30] & 0x08;
			CIn.RES4	=	CAB_ReceivedByte[30] & 0x04;
			CIn.RES5	=	CAB_ReceivedByte[30] & 0x02;
			
			CIn.PHC1	=	CAB_ReceivedByte[31] & 0x80;
			CIn.PHC2	=	CAB_ReceivedByte[31] & 0x40;
			CIn.PHC3	=	CAB_ReceivedByte[31] & 0x20;
			CIn.DO1		=	CAB_ReceivedByte[31] & 0x10;
			CIn.DO2		=	CAB_ReceivedByte[31] & 0x08;
			CIn.DO3		=	CAB_ReceivedByte[31] & 0x04;
			CIn.DC		=	CAB_ReceivedByte[31] & 0x02;
			
			if(CAB_ReceivedByte[32] == CAB_ReceivedByte[33])
			{
				CIn.Rev	=	CAB_ReceivedByte[32] & 0x80;
				CIn.RUP	=	CAB_ReceivedByte[32] & 0x40;
				CIn.RDN	=	CAB_ReceivedByte[32] & 0x20;
				CIn.STP	=	CAB_ReceivedByte[32] & 0x10;
			}
			
			BatteryVoltage = CAB_ReceivedByte[34] / 30.0;
			PowerVoltageCabin = CAB_ReceivedByte[35] / 5.0;
			
			CAB_NewPacketF = 0;
			CAB_ClearDataInDataLostF = 0;
		}
		
		CAB_RXDataCNT++;
		if(CAB_RXDataCNT >= UART_BUFFER_SIZE) CAB_RXDataCNT = 0;
	}
	
	
	
	if(CAB_DataLost == 100 && CAB_ClearDataInDataLostF == 0)
	{
		for(uint8_t i=0;i<8;i++)
		{
			PB.Cabin[i]				=	0;
			PB.Cabin[i + 8]		=	0;
			PB.Cabin[i + 16]	=	0;
			PB.Cabin[i + 24]	=	0;
			PB.Cabin[i + 32]	=	0;
			PB.Cabin[i + 40]	=	0;
			PB.Cabin[i + 48]	=	0;
			PB.Cabin[i + 56]	=	0;
			PB.Cabin[i + 64]	=	0;
			PB.Cabin[i + 72]	=	0;
			PB.Cabin[i + 80]	=	0;
			PB.Cabin[i + 88]	=	0;
			PB.Cabin[i + 96]	=	0;
			PB.Cabin[i + 104]	=	0;
			PB.Cabin[i + 112]	=	0;
			
			PB.Cabin_Cancel[i]				=	0;
			PB.Cabin_Cancel[i + 8]		=	0;
			PB.Cabin_Cancel[i + 16]		=	0;
			PB.Cabin_Cancel[i + 24]		=	0;
			PB.Cabin_Cancel[i + 32]		=	0;
			PB.Cabin_Cancel[i + 40]		=	0;
			PB.Cabin_Cancel[i + 48]		=	0;
			PB.Cabin_Cancel[i + 56]		=	0;
			PB.Cabin_Cancel[i + 64]		=	0;
			PB.Cabin_Cancel[i + 72]		=	0;
			PB.Cabin_Cancel[i + 80]		=	0;
			PB.Cabin_Cancel[i + 88]		=	0;
			PB.Cabin_Cancel[i + 96]		=	0;
			PB.Cabin_Cancel[i + 104]	=	0;
			PB.Cabin_Cancel[i + 112]	=	0;
		}
		
		CIn.Ovl	=	0;
		CIn.Ful	=	0;
		CIn.RES1	=	0;
		CIn.RES2	=	0;
		CIn.RES3	=	0;
		CIn.RES4	=	0;
		CIn.RES5	=	0;
		
		CIn.PHC1	=	0;
		CIn.PHC2	=	0;
		CIn.PHC3	=	0;
		CIn.DO1		=	0;
		CIn.DO2		=	0;
		CIn.DO3		=	0;
		CIn.DC		=	0;
		
		CIn.Rev	=	0;
		CIn.RUP	=	0;
		CIn.RDN	=	0;
		CIn.STP	=	0;
		
		BatteryVoltage = 0;
		PowerVoltageCabin = 0;
			
			
		CAB_ClearDataInDataLostF = 1;
	}
	
}



/*
void HALL_SerialExtentionDataTransmition(void)
{
	/////////////////////////////////////////
	///// Send Data To Serial Extention /////
	/////////////////////////////////////////
	if(HALL_TX_AfterLastByteTimer > 20 && Hall_ReceivingF == 0) //There is 20ms rest time between both packet
	{
		HALL_TX_AfterLastByteTimer = 0;
		
		HALL_TX_Buffer[0] = 0xFF;
		HALL_TX_Buffer[1] = 0xFE;
		HALL_TX_Buffer[2] = SERIAL_EXTENTION_SERIAL_ID;
		uint8_t PacketLen = 18; //(0xFF to Checksum)
		HALL_TX_Buffer[3] = PacketLen; //Packet Len
		
		HALL_TX_Buffer[4] = NumR & 0x7F; 			//0
		if(SegUp) HALL_TX_Buffer[4] |= 0x80;
		HALL_TX_Buffer[5] = NumL & 0x7F;			//1
		if(SegDn) HALL_TX_Buffer[5] |= 0x80;
		
		HALL_TX_Buffer[6]  = PB.HallBlinker[0];
		HALL_TX_Buffer[7]  = PB.HallBlinker[1];
		HALL_TX_Buffer[8]  = PB.HallBlinker[2];
		HALL_TX_Buffer[9]  = PB.HallBlinker[3];
		HALL_TX_Buffer[10] = PB.HallBlinker[4];
		HALL_TX_Buffer[11] = PB.HallBlinker[5];
		
		HALL_TX_Buffer[12] = PB.HallSet;
		HALL_TX_Buffer[13] = PB.HallSet;
		
		HALL_TX_Buffer[14] = PB.HallClear;
		HALL_TX_Buffer[15] = PB.HallClear;
		
		HALL_TX_Buffer[16] = SevenBitToByte(0,0,0,0,0,LandingF,RunOrStop,StandbyF);
		
		uint8_t Checksum = HALL_TX_Buffer[4];
		for(uint8_t i=5;i<(HALL_TX_Buffer[3] - 1);i++) {Checksum ^= HALL_TX_Buffer[i];}
		HALL_TX_Buffer[PacketLen - 1] = Checksum;
		HALL_TX_Buffer[PacketLen] = 0;
		
		HW_DIR_HALL(1); //Turn off in TIM17 100uS interrupt routine
		HALL_SendData( PacketLen + 1 );
		Hall_TxModeF = 1;
		
	}
	
	
	//////////////////////////////////////////
	///// Get Data From Serial Extention /////
	//////////////////////////////////////////
	while(HALL_RX_DMA_CNT != HALL_RXDataCNT)
	{
		uint8_t DataIn = HALL_RX_Buffer[HALL_RXDataCNT];
		
		if(DataIn == 0xFF && HALL_SyncStatus == 0) 				{HALL_PacketLen = 0;	HALL_SyncStatus++;}
		else if(HALL_SyncStatus == 0)											{HALL_PacketLen = 0;}
		else if(DataIn == 0xFE && HALL_SyncStatus == 1) 	{HALL_PacketLen = 0;	HALL_SyncStatus++;}
		else if(HALL_SyncStatus == 2)
		{
			if(DataIn == SERIAL_EXTENTION_SERIAL_ID) 				{HALL_PacketLen = 0; HALL_SyncStatus++;}
			else {HALL_SyncStatus = 0;} //ID doesn't match
		}
		else if(HALL_SyncStatus == 3) {HALL_PacketLen = DataIn;	HALL_SyncStatus++; HALL_PacketCounter = 0;}
		else if(HALL_SyncStatus < HALL_PacketLen && HALL_SyncStatus > 3)
		{
			HALL_ReceivedByte[HALL_PacketCounter] = DataIn;
			HALL_PacketCounter++;
			HALL_SyncStatus++;
		}
		else
		{
			if(HALL_SyncStatus == HALL_PacketLen && HALL_PacketLen > 0)
			{
				uint8_t	Cheksum = HALL_ReceivedByte[0];
				for(uint16_t iw=1;iw < HALL_PacketLen;iw++) 
				{
					Cheksum ^= HALL_ReceivedByte[iw];
				}
				
				if(DataIn == Cheksum ) 
				{
					HALL_NewPacketF = 1;
					HALL_GetDataCounter ++;
					HALL_GetDataTimer = 0;
					
					HALL_LinkLED_Status++;
					if(HALL_LinkLED_Status > 2) HALL_LinkLED_Status = 0;
				}
			}
			
			if(HALL_NewPacketF == 0) {HALL_PacketError ++;}
			HALL_SyncStatus = 0;
			HALL_PacketLen = 0;
		}
		
		
		if(HALL_NewPacketF)
		{
			
			for(uint8_t i=0;i<8;i++)
			{
				PB.Hall[i]				=	HALL_ReceivedByte[0] & 0x01;
				PB.Hall[i + 8]		=	HALL_ReceivedByte[1] & 0x01;
				PB.Hall[i + 16]		=	HALL_ReceivedByte[2] & 0x01;
				PB.Hall[i + 24]		=	HALL_ReceivedByte[3] & 0x01;
				PB.Hall[i + 32]		=	HALL_ReceivedByte[4] & 0x01;
				PB.Hall[i + 40]		=	HALL_ReceivedByte[5] & 0x01;
				PB.Hall[i + 48]		=	HALL_ReceivedByte[6] & 0x01;
				PB.Hall[i + 56]		=	HALL_ReceivedByte[7] & 0x01;
				PB.Hall[i + 64]		=	HALL_ReceivedByte[8] & 0x01;
				PB.Hall[i + 72]		=	HALL_ReceivedByte[9] & 0x01;
				PB.Hall[i + 80]		=	HALL_ReceivedByte[10] & 0x01;
				PB.Hall[i + 88]		=	HALL_ReceivedByte[11] & 0x01;
				PB.Hall[i + 96]		=	HALL_ReceivedByte[12] & 0x01;
				PB.Hall[i + 104]	=	HALL_ReceivedByte[13] & 0x01;
				PB.Hall[i + 112]	=	HALL_ReceivedByte[14] & 0x01;
				
				PB.Hall_Cancel[i]				=	HALL_ReceivedByte[15] & 0x01;
				PB.Hall_Cancel[i + 8]		=	HALL_ReceivedByte[16] & 0x01;
				PB.Hall_Cancel[i + 16]	=	HALL_ReceivedByte[17] & 0x01;
				PB.Hall_Cancel[i + 24]	=	HALL_ReceivedByte[18] & 0x01;
				PB.Hall_Cancel[i + 32]	=	HALL_ReceivedByte[19] & 0x01;
				PB.Hall_Cancel[i + 40]	=	HALL_ReceivedByte[20] & 0x01;
				PB.Hall_Cancel[i + 48]	=	HALL_ReceivedByte[21] & 0x01;
				PB.Hall_Cancel[i + 56]	=	HALL_ReceivedByte[22] & 0x01;
				PB.Hall_Cancel[i + 64]	=	HALL_ReceivedByte[23] & 0x01;
				PB.Hall_Cancel[i + 72]	=	HALL_ReceivedByte[24] & 0x01;
				PB.Hall_Cancel[i + 80]	=	HALL_ReceivedByte[25] & 0x01;
				PB.Hall_Cancel[i + 88]	=	HALL_ReceivedByte[26] & 0x01;
				PB.Hall_Cancel[i + 96]	=	HALL_ReceivedByte[27] & 0x01;
				PB.Hall_Cancel[i + 104]	=	HALL_ReceivedByte[28] & 0x01;
				PB.Hall_Cancel[i + 112]	=	HALL_ReceivedByte[29] & 0x01;
				
				for(int8_t j=0;j<30;j++)
				{
					HALL_ReceivedByte[j] >>=1;
				}
			}
			
			SerialExtInput.F1	=	HALL_ReceivedByte[30] & 0x80;
			SerialExtInput.F2	=	HALL_ReceivedByte[30] & 0x40;
			SerialExtInput.I1	=	HALL_ReceivedByte[30] & 0x20;
			SerialExtInput.I2	=	HALL_ReceivedByte[30] & 0x10;
			SerialExtInput.I3	=	HALL_ReceivedByte[30] & 0x08;
			SerialExtInput.I4	=	HALL_ReceivedByte[30] & 0x04;
			SerialExtInput.I5	=	HALL_ReceivedByte[30] & 0x02;
			SerialExtInput.S68=	HALL_ReceivedByte[30] & 0x01;
			
			HALL_NewPacketF = 0;
		}
		
		HALL_RXDataCNT++;
		if(HALL_RXDataCNT >= HALL_RX_BUFFER_SIZE) HALL_RXDataCNT = 0;
	}
	
	HW_LED_LINK_HALL(HALL_LinkLED_Status > 1);
}
*/








void ParallelAnalog_OD_Module_SerialDataTransmition(void)
{
	if(DRV_TX_AfterLastByteTimer >  14 && DRV_RxModeF == 0) //There is 15ms rest time between both packet
	{
		DRV_TX_AfterLastByteTimer = 0;
		
		DRV_TX_Buffer[0] = 0xFF;
		DRV_TX_Buffer[1] = 0xFE;
		DRV_TX_Buffer[2] = PARALLEL_ANALOG_MODULE_ID;
		DRV_TX_Buffer[3] = 11; //Packet Len
		
		int UFOut = (CMD_FOut * 100);
		uint8_t 	FOutMsb = (UFOut >> 8) & 0x00FF;
		uint8_t 	FOutLsb = (UFOut) & 0x00FF;
		DRV_TX_Buffer[4] = FOutMsb; 		//0
		DRV_TX_Buffer[5] = FOutMsb;			//1
		DRV_TX_Buffer[6] = FOutLsb; 		//2
		DRV_TX_Buffer[7] = FOutLsb;			//3
		
		DRV_TX_Buffer[8] = SevenBitToByte(0,Hw_OC7,Hw_OC6,Hw_OC5,Hw_OC4,Hw_OC3,Hw_OC2,Hw_OC1); //2
		DRV_TX_Buffer[9] = DRV_TX_Buffer[8];			//3
		
		uint8_t Checksum = DRV_TX_Buffer[4];
		for(uint8_t i=5;i<10;i++) {Checksum ^= DRV_TX_Buffer[i];}
		DRV_TX_Buffer[10]= Checksum;
		DRV_TX_Buffer[11] = 0;
		
		DRV_SendData(12);
	}
	
	///////////////////////////////////
	///// Get Data From OD Module /////
	///////////////////////////////////
	while(DRV_RX_DMA_CNT != DRV_RXDataCNT)
	{
		DRV_RxModeF = 1;
		
		uint8_t DataIn = DRV_RX_Buffer[DRV_RXDataCNT];
		
		if(DataIn == 0xFF && DRV_SyncStatus == 0) 			{DRV_SyncStatus++;}
		else if(DataIn == 0xFE && DRV_SyncStatus == 1) 	{DRV_SyncStatus++;}
		else if(DRV_SyncStatus == 2)
		{
			if(DataIn == PARALLEL_ANALOG_MODULE_ID)
			{
				DRV_GetDataTimer = 0;
					
				DRV_LinkLED_Status++;
				if(DRV_LinkLED_Status > 3) DRV_LinkLED_Status = 0;
			}
			else {DRV_SyncStatus = 0;} //ID doesn't match
		}
		
		
		DRV_RXDataCNT++;
		if(DRV_RXDataCNT >= UART_BUFFER_SIZE) DRV_RXDataCNT = 0;
	}
	
	HW_LED_LINK_DRV(DRV_LinkLED_Status > 1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////  EEPROM & Hardware Key Module Communication  ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Module_SerialDataTransmition(void)
{
	
	////////////////////////////////
	///// Get Data From Module /////
	////////////////////////////////
	int16_t Num_Of_EEPROM_Byte = 3500;
	
	_Bool		NewDataF=0;
	_Bool 	HoldInWhile = 0;
	_Bool 	SelectionStatusF=0,SendigDataF=0,GettingDataF=0;
	
	if(PassCorrect == 0) 
	{
		HoldInWhile = 1;
	}
	
	do
	{
		
		while(PR_RX_DMA_CNT != PR_RXDataCNT)
		{
			WDT_RESET;
			
			PR_ByteReciveTimer=0;
			
			uint8_t DataIn = PR_RX_Buffer[PR_RXDataCNT];
			////Sync Bytes
			if(DataIn == 0xFF && PR_SyncStatus == 0) 			{PR_DataLen = 0;	PR_SyncStatus++;}
			else if(PR_SyncStatus == 0)										{PR_DataLen = 0;}
			else if(DataIn == 0xFE && PR_SyncStatus == 1)	{PR_DataLen = 0;	PR_SyncStatus++;}
			
			else if(PR_SyncStatus == 2)
			{
				if(DataIn == MODULE_EEPROM_SERIAL_ID) 		{PR_DataLen = 0;	PR_SyncStatus++;}
				else {PR_SyncStatus = 0;} //ID doesn't match
			}
			////MSB PRDataLen
			else if(PR_SyncStatus == 3) {PR_DataLen = DataIn;	PR_SyncStatus++; PR_ByteCounter = 0;} 
			////MSB PRDataLen
			else if(PR_SyncStatus == 4) {PR_DataLen <<= 8; PR_DataLen += (DataIn+5);	PR_SyncStatus++; PR_ByteCounter = 0;}
			else if(PR_SyncStatus < PR_DataLen && PR_SyncStatus > 4)
			{
				PR_ReceivedByte[PR_ByteCounter] = DataIn;
				PR_ByteCounter++;
				PR_SyncStatus++;
			}
			else
			{
				PR_SyncStatus = 0;
				PR_DataLen = 0;
			}
			
			if(PR_SyncStatus == PR_DataLen && PR_DataLen > 0)
			{
				NewDataF = 1;				
			}
			
			
			if(NewDataF)
			{
				///////////////////////////////////////////////////////
				////// EEPROM Module ready (choose load or save) //////
				///////////////////////////////////////////////////////
				if(PR_ReceivedByte[0] == MODULE_READY)
				{
					///////////////////////////////
					//// Send UID for get pass ////
					///////////////////////////////
					if(PassCorrect == 0)
					{
						PR_TX_Buffer[0] = 0xFF;
						PR_TX_Buffer[1] = 0xFE;
						PR_TX_Buffer[2] = MODULE_EEPROM_SERIAL_ID;
						PR_TX_Buffer[3] = 0;
						PR_TX_Buffer[4] = 13;
						PR_TX_Buffer[5] = MODULE_GET_KEY;
						
						PR_TX_Buffer[6] = (U_ID[0] >> 24) & 0x000000FF;
						PR_TX_Buffer[7] = (U_ID[0] >> 16) & 0x000000FF;
						PR_TX_Buffer[8] = (U_ID[0] >> 8) 	& 0x000000FF;
						PR_TX_Buffer[9] = (U_ID[0]) & 0x000000FF;
						
						PR_TX_Buffer[10] = (U_ID[1] >> 24) & 0x000000FF;
						PR_TX_Buffer[11] = (U_ID[1] >> 16) & 0x000000FF;
						PR_TX_Buffer[12] = (U_ID[1] >> 8) 	& 0x000000FF;
						PR_TX_Buffer[13] = (U_ID[1]) & 0x000000FF;
						
						PR_TX_Buffer[14] = (U_ID[2] >> 24) & 0x000000FF;
						PR_TX_Buffer[15] = (U_ID[2] >> 16) & 0x000000FF;
						PR_TX_Buffer[16] = (U_ID[2] >> 8) 	& 0x000000FF;
						PR_TX_Buffer[17] = (U_ID[2]) & 0x000000FF;
						
						PR_SendData(19);
					}
					/////////////////////////////
					//// It is EEPROM module ////
					/////////////////////////////
					else
					{
						HoldInWhile = 1;
						SelectionStatusF = 1;
						K_Up = 0;
						K_Dn = 0;
					}
				}
				
				else if(PR_ReceivedByte[0] == MODULE_SEND_DATA)
				{
					WDT_RESET;
					for(int16_t iw=0;iw<PR_ByteCounter;iw++)
					{
						PR_ReceivedByte[iw] = PR_ReceivedByte[iw+1];
					}
					ee24_write(0,PR_ReceivedByte,PR_ByteCounter-4);
					
					HoldInWhile = 0;
					//WDT_Init();
					HAL_Delay(5000); //For reset by WDT
				}
				
				else if(PR_ReceivedByte[0] == MODULE_GET_KEY)
				{
					WDT_RESET;
					uint32_t	Byte3=0,Byte2=0,Byte1=0,Byte0=0;
					
					Byte3 = PR_ReceivedByte[1]	& 0x000000FF;
					Byte2 = PR_ReceivedByte[2]	& 0x000000FF;
					Byte1 = PR_ReceivedByte[3]	& 0x000000FF;
					Byte0 = PR_ReceivedByte[4] & 0x000000FF;
					HwPass[0] = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
					Byte3 = PR_ReceivedByte[5] & 0x000000FF;
					Byte2 = PR_ReceivedByte[6] & 0x000000FF;
					Byte1 = PR_ReceivedByte[7] & 0x000000FF;
					Byte0 = PR_ReceivedByte[8] & 0x000000FF;
					HwPass[1] = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
					Byte3 = PR_ReceivedByte[9]	 & 0x000000FF;
					Byte2 = PR_ReceivedByte[10] & 0x000000FF;
					Byte1 = PR_ReceivedByte[11] & 0x000000FF;
					Byte0 = PR_ReceivedByte[12] & 0x000000FF;
					HwPass[2] = (Byte3 << 24) | (Byte2 << 16) | (Byte1 << 8) | Byte0;
					
					SaveHwKeyPassword();
					//WDT_Init();
					HAL_Delay(50000); //For reset by WDT
				}
				
				NewDataF = 0;
			}
			
			PR_RXDataCNT++;
			if(PR_RXDataCNT >= PR_BUFFER_SIZE) PR_RXDataCNT = 0;
		}
		
		if(PassCorrect == 0)
		{
			if(LCD_UpdateF == 0)
			{
				sprintf(LcdStr,"Board is locked!");
				LCD_write_str_middle(25,LcdStr);	
				
				LCD_Update();
			}
		}
		
		else if(SelectionStatusF)
		{
			if(K_Up)
			{
				WDT_RESET;
				
				ee24_read(MAIN_DATA_START_ADDRESS,(uint8_t *)PR_TX_Buffer,Num_Of_EEPROM_Byte);
				
				for(int16_t iw=Num_Of_EEPROM_Byte;iw>=0;iw--)//0-500 -> 6-506
				{
					PR_TX_Buffer[iw+6] = PR_TX_Buffer[iw];
				}
				PR_TX_Buffer[0] = 0xFF;
				PR_TX_Buffer[1] = 0xFE;
				PR_TX_Buffer[2] = MODULE_EEPROM_SERIAL_ID;
				PR_TX_Buffer[3] = (Num_Of_EEPROM_Byte >> 8) & 0xFF;
				PR_TX_Buffer[4] =  Num_Of_EEPROM_Byte & 0xFF;
				PR_TX_Buffer[5] = MODULE_GET_DATA;
				
				PR_SendData(Num_Of_EEPROM_Byte + 7);
				
				SendigDataF = 1;
				K_Up = 0;
			}
			
			else if(K_Dn)
			{
				WDT_RESET;
				
				PR_TX_Buffer[0] = 0xFF;
				PR_TX_Buffer[1] = 0xFE;
				PR_TX_Buffer[2] = MODULE_EEPROM_SERIAL_ID;
				PR_TX_Buffer[3] = 0;
				PR_TX_Buffer[4] = 3;
				PR_TX_Buffer[5] = MODULE_SEND_DATA;
				PR_TX_Buffer[6] = (Num_Of_EEPROM_Byte >> 8) & 0xFF;
				PR_TX_Buffer[7] = Num_Of_EEPROM_Byte & 0xFF;
				PR_SendData(9);
				
				GettingDataF = 1;
				K_Dn = 0;
			}
			
			if(LCD_UpdateF == 0)
			{
				if(SendigDataF)
				{
					sprintf(LcdStr,"Sending Data!");
					LCD_write_str_middle(25,LcdStr);
					if(PR_TX_DMA_CNT == 0)
					{
						for(uint8_t	Del=0;Del<10;Del++)
						{
							WDT_RESET;
							HAL_Delay(10);
						}
						HoldInWhile = 0;
					}
				}
				else if(GettingDataF)
				{
					sprintf(LcdStr,"Getting Data!");
					LCD_write_str_middle(25,LcdStr);
				}
				
				else
				{
					sprintf(LcdStr,"Press one key");
					LCD_write_str(1,1,LcdStr);
					sprintf(LcdStr,"for save:");
					LCD_write_str(1,17,LcdStr);
					sprintf(LcdStr,"To module: UP");
					LCD_write_str(1,34,LcdStr);
					sprintf(LcdStr,"To board: DN");
					LCD_write_str(1,51,LcdStr);
				}
				LCD_Update();
			}
		}
		
		WDT_RESET;
	}
	while(HoldInWhile);
	
	
}




