//---------------------------------------------------------------------------

#ifndef ModPLCH
#define ModPLCH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <System.Win.ScktComp.hpp>
#include "Define.h"

#include <deque>

//---------------------------------------------------------------------------
//	디바이스 코드
//---------------------------------------------------------------------------
const int DEVCODE_M			=	0x90;		//	내부 릴레이
const int DEVCODE_L			=	0x92;	  	//	래치 릴레이
const int DEVCODE_D			=	0xA8;		//	데이터 레지스터
const int DEVCODE_W			=	0xB4;		//	링크 레지스터
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	Index 구분
//---------------------------------------------------------------------------
const int PLC_INDEX_INTERFACE[1]				= { 1 };
const int PC_INDEX_INTERFACE					=		11;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	시작 번지(통합)
//---------------------------------------------------------------------------
const int PLC_D_INTERFACE_START_DEV_NUM	 			=	{40000};
const int PLC_D_INTERFACE_LEN	 					= 	100;
//----------------------------------------------------------------------------

const int PC_D_INTERFACE_START_DEV_NUM				=	{41000};
const int PC_D_INTERFACE_LEN  						= 	870;

const int PC_D_INTERFACE_CURRENT					=	{42000};
const int PC_D_INTERFACE_CURRENT_LEN				= 	800;
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------
//	PLC - PC Interface
//---------------------------------------------------------------------------
const int PLC_D_HEART_BEAT				   			=	0;
const int PLC_D_AUTO_MANUAL			   				=	1;
const int PLC_D_PRE_ERROR		   	  				=	2;
const int PLC_D_PRE_TRAY_IN	    	  		        =   3;
const int PLC_D_PRE_PROB_OPEN   	  		        =   4;
const int PLC_D_PRE_PROB_CLOSE 		   	  	        =   5;
const int PLC_D_PRE_TRAY_POS	                    =   6;

const int PLC_D_PRE_TRAY_ID    	  			        =   10;

// TRAY INFO
const int PLC_D_PRE_TRAY_CELL_DATA				   	=	30;
//---------------------------------------------------------------------------
//	PC - PLC Interface
//---------------------------------------------------------------------------
const int PC_D_HEART_BEAT			  				=	0;
const int PC_D_PRE_STAGE_AUTO_READY     	        =   1;
const int PC_D_PRE_ERROR						   	=	2;
const int PC_D_PRE_TRAY_OUT    	  		        	=   3;
const int PC_D_PRE_PROB_OPEN   	  		        	=   4;
const int PC_D_PRE_PROB_CLOSE    	  	        	=   5;
const int PC_D_PRE_CHARGING                      	=   6;
const int PC_D_PRE_COMPLETE1		                =   7;
const int PC_D_PRE_COMPLETE2		                =   8;
const int PC_D_PRE_TRAY_POS_MOVE	                =   9;

const int PC_D_PRE_NG_COUNT                      	=   10;
const int PC_D_PRE_CURRENT_MIN                      =   11;
const int PC_D_PRE_CHARGE_VOLTAGE                   =   13;
const int PC_D_PRE_CHARGE_CURRENT                   =   15;
const int PC_D_PRE_CHARGE_TIME                      =   17;
const int PC_D_PRE_NG_ALARM                         =   19;

// OK/NG - D41030
const int PC_D_PRE_MEASURE_OK_NG			   		=	30;

// D41070
const int PC_D_PRE_VOLTAGE_VALUE                    =   100;
// D42000
const int PC_D_PRE_CURRENT_VALUE                    =   0;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//	PLC
//---------------------------------------------------------------------------
typedef struct
{
	unsigned char SubHeader[2];			//	서브 헤더
	unsigned char NetNum;		 		//	네트워크 번호
	unsigned char PlcNum;	   			//	PLC 번호
	unsigned char ReqIONum[2];			//	IO 번호
	unsigned char ReqOfficeNum;	  		//	국 번호
	unsigned char ReqDataLen[2];		//  요구 데이터 길이(CPU 감시 타이머 ~ 디바이스 길이)
	unsigned char CpuTime[2];			//	CPU 감시 타이머
	unsigned char Command[2];			//	커맨드
	unsigned char SubCommand[2];		//	서브 커맨드 (0 = 비트-16단위, 워드-1단위 / 1 = 비트-1단위)
	unsigned char StartDevNum[3];		//	선두 디바이스
	unsigned char DevCode;  			//	디바이스 코드
	unsigned char DevLen[2];			//	디바이스 길이
} PLC_DATA;
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//	PC
//---------------------------------------------------------------------------
typedef struct
{
	unsigned char SubHeader[2];					//	서브 헤더
	unsigned char NetNum;						//	네트워크 번호
	unsigned char PlcNum;	    				//	PLC 번호
	unsigned char ReqIONum[2];					//	IO 번호
	unsigned char ReqOfficeNum;	  				//	국 번호
	unsigned char ReqDataLen[2];				//  요구 데이터 길이(CPU 감시 타이머 ~ 디바이스 길이)
	unsigned char CpuTime[2];					//	CPU 감시 타이머
	unsigned char Command[2];					//	커맨드
	unsigned char SubCommand[2];				//	서브 커맨드
	unsigned char StartDevNum[3];				//	선두 디바이스
	unsigned char DevCode;  					//	디바이스 코드
	unsigned char DevLen[2];					//	디바이스 길이
} PC_DATA;
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
class TMod_PLC : public TDataModule
{
__published:	// IDE-managed Components
	TTimer *Timer_Read;
	TClientSocket *ClientSocket_PC;
	TClientSocket *ClientSocket_PLC;
	TTimer *Timer_PC_AutoConnect;
	TTimer *Timer_PC_WriteMsg;
	TTimer *Timer_PLC_WriteMsg;
	TTimer *Timer_PLC_AutoConnect;
	void __fastcall ClientSocket_PCConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PCDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PCError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ClientSocket_PCRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Timer_PC_WriteMsgTimer(TObject *Sender);
	void __fastcall Timer_PC_AutoConnectTimer(TObject *Sender);
	void __fastcall ClientSocket_PLCConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PLCDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall ClientSocket_PLCError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall ClientSocket_PLCRead(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall Timer_PLC_WriteMsgTimer(TObject *Sender);
	void __fastcall Timer_PLC_AutoConnectTimer(TObject *Sender);

private:	// User declarations
	bool bClose;

//---------------------------------------------------------------------------
//	PLC
//---------------------------------------------------------------------------
	void __fastcall PLC_Initialization();
	void __fastcall PLC_DataChange(int subCommand, int address, int devCode, int devLen);
	void __fastcall PLC_Recv_Interface(int nTag);

	PLC_DATA plc_Data;
	AnsiString plc_Read, plc_Read_Temp;
	bool plc_ReadFlag;
	int plc_ReadCount, plc_index;
	AnsiString plc_Interface;

//---------------------------------------------------------------------------
//	PC
//---------------------------------------------------------------------------
	void __fastcall PC_Initialization();
	void __fastcall PC_DataChange(int subCommand, int address, int devCode, int devLen);

	PC_DATA pc_Data;
	AnsiString pc_Read, pc_Read_Temp;
	bool pc_ReadFlag;
	int pc_ReadCount, pc_index;

//---------------------------------------------------------------------------
//	Tray Info
//---------------------------------------------------------------------------
	void __fastcall SaveTrayInfo(AnsiString trayID);
    AnsiString m_trayID;
	int m_slotNum, m_slotNumTemp;
public:		// User declarations
	__fastcall TMod_PLC(TComponent* Owner);

	void __fastcall Connect(AnsiString ip, int port1, int port2);
	void __fastcall DisConnect();

	void __fastcall SetData(unsigned char (*data)[2], int column, int num, bool flag);
	void __fastcall SetDouble(unsigned char (*data)[2], int column, double value);
	void __fastcall SetString(unsigned char (*data)[2], int column, AnsiString msg);

	int __fastcall GetData(unsigned char (*data)[2], int column, int num);
	double __fastcall GetDouble(unsigned char (*data)[2], int column);
	AnsiString __fastcall GetString(unsigned char (*data)[2], int column, int count);

	bool PLC_Write_Result; //voltage, current 값은 필요 시에만 쓰기를 한다.
	unsigned char plc_Interface_Data[PLC_D_INTERFACE_LEN][2];
	unsigned char pc_Interface_Data[PC_D_INTERFACE_LEN][2];
	unsigned char pc_Interface_Curr_Data[PC_D_INTERFACE_CURRENT_LEN][2];
};
//---------------------------------------------------------------------------
extern PACKAGE TMod_PLC *Mod_PLC;
//---------------------------------------------------------------------------
#endif
