#ifndef RVMO_define
#define RVMO_define


#define APP_PATH					"D:\\PreCharger\\"
#define BIN_PATH					APP_PATH"Bin\\"
#define DATA_PATH					APP_PATH"Data\\"
#define LOG_PATH                    APP_PATH"Log\\"
#define TRAY_PATH                   APP_PATH"Tray\\"
#define TRAY_PATH2                  "D:\\CELL_SERIAL\\Data\\"
//#define MES_PATH                   "X:\\Data\\Charger\\"


//---------------------------------------------------------------------------
//	STEP 정보
//---------------------------------------------------------------------------
#define STEP_WAIT					0
#define STEP_MEASURE				1
#define STEP_FINISH					2
//---------------------------------------------------------------------------

#define chstNone		0
#define chstRunning		1
#define chstFail		2
#define chstAbort		3
#define chstOK			4

#define		COMM_RECEIVE	WM_USER + 1003

//---------------------------------------------------------------------------
//	CONTROLLER 명령어, 설비상태 (SEN)
//---------------------------------------------------------------------------
const int SEN = 1;
const int MON = 2;

const int ASB = 3;
const int ENA = 4;
const int STP = 5;
const int nCAL = 6;
const int STC = 7;
const int TRB = 8;
const int IDN = 9;
const int RST = 10;
const int RBT = 11;
const int DST = 12;
const int ERR = 13;
const int RPY = 14;

const int IDL = 21;
const int RUN = 22;
const int WDT = 23;
const int REC = 24;
const int NOA = 25; // noanswer

//---------------------------------------------------------------------------
//	Channel 갯수
//---------------------------------------------------------------------------
const int CONTCOUNT 	= 1;
const int MAXCHANNEL 	= 400;
const int LINECOUNT		= 20;

const int SEND 			= 1;
const int RECEIVE 		= 2;
const int ETC 			= 3;
const int KEYSIGHT 		= 4;

const char nAuto 		= 0;
const char nRemote 		= 1;
const char nLocal 		= 2;
const char nAllRetest 	= 3;
const char nCellRetest 	= 4;
const char nCalibration = 5;

const char nNoAnswer	= 0;
const char nIdle		= 1;
const char nVacancy		= 2;
const char nIN			= 3;
const char nREADY		= 4;
const char nRUN			= 5;
const char nEND			= 6;
const char nFinish 		= 7;
const char nManual		= 8;
const char nError       = 9;

const char nRedEnd 		= 20;
const char nBlueEnd 	= 21;
const int nRunningError = 28;
const int nReadyError 	= 30;
const int nFinishError  = 32;
const int nDefaultError = 40;

// Process Status
const int sReady   		= 0;
const int sTrayIn  		= 1;
const int sBarcode 		= 2;
const int sProbeDown 	= 3;
const int sCharge       = 4;
const int sFinish       = 5;
const int sProbeOpen    = 6;
const int sTrayOut      = 7;

// Error Message
const int nNgErr    	= 0;
const int nRebootErr  	= 1;
const int nResetErr     = 2;
const int nEnaErr       = 3;

typedef struct{
    bool rst;
	bool set;
	bool ams;
	bool amf;
	bool trayin;
	bool channel_charging;
    bool end_charging;
	AnsiString trayid;
	int cell[MAXCHANNEL];
	int cell_count;
	int measure_result[MAXCHANNEL];
	int error_time_count[MAXCHANNEL];
	AnsiString cell_data[MAXCHANNEL];
	AnsiString cell_serial[MAXCHANNEL];
	AnsiString mdl_name;
	AnsiString batch_id;
	AnsiString lot_id;
	AnsiString cell_model;
	AnsiString lot_number;
}TRAY_INFO;

typedef struct{
	int arl_reserve;
	int arl;
    int status;
    int oldstatus;
	bool init;
	int err;
	int alarm_status;
	int alarm_cnt;
    int runcount;
    bool bconnected;
}STAGE_INFO;

typedef struct{
	bool recontact;
	double volt;
	double curr;
	int time;
    int remeasure_alarm_cnt;
}CONFIG;

typedef struct{
	double volt;
	double curr;
	int time;
	double failvolt;
}PRECHARGE_CONFIG;

typedef struct{
	double volt;
	double curr;
	int time;
	double failvolt;
}CHARGE_CONFIG;

typedef struct{
	double max_volt;
	double max_curr;
	int max_time;

}MAX_CONFIG;

typedef struct{
	unsigned short int stage_status;
	AnsiString step_index;

	AnsiString volt[MAXCHANNEL];
	AnsiString final_volt[MAXCHANNEL];

	AnsiString curr[MAXCHANNEL];
	AnsiString final_curr[MAXCHANNEL];

	AnsiString capa[MAXCHANNEL];
	AnsiString final_capa[MAXCHANNEL];
	
	int status[MAXCHANNEL];       //* mon status ( > 0 현재step, < 0 에러 상태)
    int final_status[MAXCHANNEL];

    AnsiString result[MAXCHANNEL];
	AnsiString final_result[MAXCHANNEL];
    bool bBT1; //* bt1 응답 읽음
    bool bBT2; //* bt2 응답 읽음

	unsigned int step_time;
	unsigned int test_time;
}REAL_TIME;


#endif
