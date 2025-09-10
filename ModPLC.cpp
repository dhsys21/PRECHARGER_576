//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RVMO_main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMod_PLC *Mod_PLC;
//---------------------------------------------------------------------------
__fastcall TMod_PLC::TMod_PLC(TComponent* Owner)
	: TDataModule(Owner)
{
	// PLC
	plc_Data.SubHeader[0] = 0x50;
	plc_Data.SubHeader[1] = 0x00;
	plc_Data.NetNum = 0x03;
	plc_Data.PlcNum = 0x01;
	plc_Data.ReqIONum[0] = 0xff;
	plc_Data.ReqIONum[1] = 0x03;
	plc_Data.ReqOfficeNum = 0x00;
	plc_Data.ReqDataLen[0] = 0x0c;
	plc_Data.ReqDataLen[1] = 0x00;
	plc_Data.CpuTime[0] = 0x10;
	plc_Data.CpuTime[1] = 0x00;
	plc_Data.Command[0] = 0x01;
	plc_Data.Command[1] = 0x04;

	plc_index = PLC_INDEX_INTERFACE[0];



	// PC
	pc_Data.SubHeader[0] = 0x50;
	pc_Data.SubHeader[1] = 0x00;
	pc_Data.NetNum = 0x03;
	pc_Data.PlcNum = 0x01;
	pc_Data.ReqIONum[0] = 0xff;
	pc_Data.ReqIONum[1] = 0x03;
	pc_Data.ReqOfficeNum = 0x00;

	pc_Data.CpuTime[0] = 0x10;
	pc_Data.CpuTime[1] = 0x00;
	pc_Data.Command[0] = 0x01;
	pc_Data.Command[1] = 0x14;

	pc_index = PC_INDEX_INTERFACE;

	plc_Interface = "";

	bClose = false;

	//* PLC Data Init
	memset(plc_Interface_Data, 0, sizeof(unsigned char) * PLC_D_INTERFACE_LEN * 2);
    memset(plc_Interface_Cell_Serial, 0, sizeof(unsigned char) * PLC_D_CELL_SERIAL_LEN * 2);
    //* PC Data Init
	memset(pc_Interface_Data, 0, sizeof(unsigned char) * PC_D_INTERFACE_LEN * 2);
    memset(pc_Interface_Volt_Data, 0, sizeof(unsigned char) * PC_D_INTERFACE_VOLTAGE_LEN * 2);
	memset(pc_Interface_Curr_Data, 0, sizeof(unsigned char) * PC_D_INTERFACE_CURRENT_LEN * 2);

    PLC_Write_Result = false; //voltage, current ���� �ʿ� �ÿ��� ���⸦ �Ѵ�.
    CellSerialIndex = 0;
    currentPLCTask = nPLCDATA;
    currentPCTask = nPCDATA;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
//	���� & ����
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::Connect(AnsiString ip, int port1, int port2)
{
	try
	{
		bClose = false;

		ClientSocket_PLC->Address = ip;
		ClientSocket_PLC->Port = port1;
		ClientSocket_PLC->Open();

		ClientSocket_PC->Address = ip;
		ClientSocket_PC->Port = port2;
		ClientSocket_PC->Open();
	}
	catch(...)
	{
		ShowMessage("PLC -> PC : ��� ���� ����.");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::DisConnect()
{
	bClose = true;
	Timer_PLC_WriteMsg->Enabled = false;
	Timer_PC_WriteMsg->Enabled = false;
	ClientSocket_PLC->Close();
	ClientSocket_PC->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::PLC_Initialization()
{
	plc_Read = "";
	plc_ReadCount = 0;
	plc_ReadFlag = true;

	Timer_PLC_WriteMsg->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::PC_Initialization()
{
	pc_Read = "";
	pc_ReadFlag = true;
	pc_ReadCount = 0;

	Timer_PC_WriteMsg->Enabled = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  Client Socket PC
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::ClientSocket_PCConnect(TObject *Sender, TCustomWinSocket *Socket)

{
    PC_Initialization();
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::ClientSocket_PCDisconnect(TObject *Sender, TCustomWinSocket *Socket)

{
    if(bClose) bClose = false;
	else Timer_PC_AutoConnect->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::ClientSocket_PCError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorCode = 0;
	Socket->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::ClientSocket_PCRead(TObject *Sender, TCustomWinSocket *Socket)

{
    int length = Socket->ReceiveLength();
	pc_Read_Temp = Socket->ReceiveText();

	for(int i = 0; i < length; i++)
		pc_Read += IntToHex((unsigned char)pc_Read_Temp[i + 1], 2);

	while(!pc_Read.IsEmpty() && (pc_Read.Length() >= 22) && (pc_Read.Pos("D000")))
	{
		int index = pc_Read.Pos("D000");

		if(index == 1)
		{
			if(pc_Read.SubString(19, 4) == "0000")	//	������ �о�� ��, �׷��� ����
			{
				pc_ReadFlag = true;
				pc_ReadCount = 0;
			}
			pc_Read = "";
			break;
		}
		else pc_Read.Delete(1, index - 1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::Timer_PC_WriteMsgTimer(TObject *Sender)
{
    bool flag = false;
	TTimer *tmr;
	tmr = (TTimer*)Sender;
	//int i = tmr->Tag;
    if(ClientSocket_PC->Active)
	{
		if(pc_ReadFlag)
		{
			if(pc_index == PC_INDEX_INTERFACE)
			{
				switch(currentPCTask)
                {
                    case nPCDATA:
                        //* Heart Beat
                        flag = GetData(pc_Interface_Data, PC_D_HEART_BEAT, 0);
                        SetDouble(pc_Interface_Data, PC_D_HEART_BEAT, (int)!flag);

                        //* General Data, Result Data, Min/Max Data, IR Data
                        PC_DataChange(0, PC_D_INTERFACE_START_DEV_NUM, DEVCODE_D, PC_D_INTERFACE_LEN);
                        ClientSocket_PC->Socket->SendBuf(&pc_Data, sizeof(pc_Data));        // should comment for emulator
                        ClientSocket_PC->Socket->SendBuf(&pc_Interface_Data, sizeof(pc_Interface_Data));
                        currentPCTask = nVOLTAGE;
                        break;
                    case nVOLTAGE: //* 2word �� �ؾ� �ϸ� ���⿡ 2���� ������ ����. ���̿� sleep(50) �߰�
                        PC_DataChange(0, PC_D_INTERFACE_VOLTAGE, DEVCODE_D, PC_D_INTERFACE_VOLTAGE_LEN);
                        ClientSocket_PC->Socket->SendBuf(&pc_Data, sizeof(pc_Data));
                        ClientSocket_PC->Socket->SendBuf(&pc_Interface_Volt_Data, sizeof(pc_Interface_Volt_Data));
                        currentPCTask = nCURRENT;
                        break;
                    case nCURRENT: //* 2word �� �ؾ� �ϸ� ���⿡ 2���� ������ ����. ���̿� sleep(50) �߰�
                        PC_DataChange(0, PC_D_INTERFACE_CURRENT, DEVCODE_D, PC_D_INTERFACE_CURRENT_LEN);
                        ClientSocket_PC->Socket->SendBuf(&pc_Data, sizeof(pc_Data));
                        ClientSocket_PC->Socket->SendBuf(&pc_Interface_Curr_Data, sizeof(pc_Interface_Curr_Data));
                        currentPCTask = nPCDATA;
                        break;
                }

				pc_ReadFlag = false;
			}
		}
		else if(pc_ReadCount > 20)
		{		//	3�ʵ��� ����Ȯ��
			//ClientSocket_PC->Close();
		}

		pc_ReadCount++;
	}
	else
	{
		ClientSocket_PC->Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::Timer_PC_AutoConnectTimer(TObject *Sender)
{
	ClientSocket_PC->Active = true;
	Timer_PC_AutoConnect->Enabled = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	������ ����
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::PC_DataChange(int subCommand, int address, int devCode, int devLen)
{
	pc_Data.SubCommand[0] = subCommand;
	pc_Data.SubCommand[1] = 0x00;

	pc_Data.ReqDataLen[0] = (0x0c + (devLen * 2)) % 256;
	pc_Data.ReqDataLen[1] = (0x0c + (devLen * 2)) / 256;;

	pc_Data.StartDevNum[0] = address % 256;
	pc_Data.StartDevNum[1] = (address / 256) % 256;
	pc_Data.StartDevNum[2] = address / (256 * 256);
	pc_Data.DevCode = devCode;

	pc_Data.DevLen[0] = devLen % 256;
	pc_Data.DevLen[1] = devLen / 256;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//  Client Socket PLC
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::ClientSocket_PLCConnect(TObject *Sender, TCustomWinSocket *Socket)

{
	PLC_Initialization();
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::ClientSocket_PLCDisconnect(TObject *Sender, TCustomWinSocket *Socket)

{
    if(bClose) bClose = false;
	else Timer_PLC_AutoConnect->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::ClientSocket_PLCError(TObject *Sender, TCustomWinSocket *Socket,
          TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorCode = 0;
	Socket->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMod_PLC::ClientSocket_PLCRead(TObject *Sender, TCustomWinSocket *Socket)

{
    int length = Socket->ReceiveLength();
	plc_Read_Temp = Socket->ReceiveText();

	for(int i = 0; i < length; i++)
		plc_Read += IntToHex((unsigned char)plc_Read_Temp[i + 1], 2);
   	BaseForm->Memo1->Lines->Add(plc_Read);
	while(!plc_Read.IsEmpty() && (plc_Read.Length() >= 54) && (plc_Read.Pos("D000")))
	{
		int index = plc_Read.Pos("D000");

		if(index != 1) plc_Read.Delete(1, index - 1);		//	��� ��ġ ���� Ȯ��

		if(plc_Read.SubString(19, 4) == "0000")		// ���� �ڵ� Ȯ��(����)
		{
			int length = 18 + StrToInt("0x" + plc_Read.SubString(15, 2))
						+ (StrToInt("0x" + plc_Read.SubString(17, 2)) * 256);		//	������ ���� Ȯ��

			if(plc_Read.Length() >= length)
			{
                switch(currentPLCTask)
                {
                    case nPLCDATA:
                        PLC_Recv_Interface();
                        currentPLCTask = nCELLSERIAL;
                        break;
                    case nCELLSERIAL:  // 720���带 8���� ������ ����
                        int wordsRead = PLC_D_CELL_SERIAL_READLEN;
                        PLC_Recv_Interface_CellSerial(CellSerialIndex, wordsRead);
                        CellSerialIndex++;

                        if(CellSerialIndex >= 8) CellSerialIndex = 0;
                        currentPLCTask = nPLCDATA;
                        break;
                }
//				if(plc_index == PLC_INDEX_INTERFACE[0]) PLC_Recv_Interface(0);
//                else plc_index = PLC_INDEX_INTERFACE[0];
			}
			else break;
		}
		plc_Read = "";
		break;
	}

	plc_ReadCount = 0;
	plc_ReadFlag = true;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::Timer_PLC_WriteMsgTimer(TObject *Sender)
{
    int startAddress;
	if(ClientSocket_PLC->Active)
	{
		if(plc_ReadFlag)
		{
            switch(currentPLCTask)
            {
                case nPLCDATA:
                    PLC_DataChange(0, PLC_D_INTERFACE_START_DEV_NUM, DEVCODE_D, PLC_D_INTERFACE_LEN);
                    break;
                case nCELLSERIAL: // �ѹ��� 720 word�� ��û
                    startAddress = PLC_D_CELL_SERIAL_NUM + (CellSerialIndex * PLC_D_CELL_SERIAL_READLEN);
                    PLC_DataChange(0, startAddress, DEVCODE_D, PLC_D_CELL_SERIAL_READLEN);
                    break;
                default:
                	break;
            }
            ClientSocket_PLC->Socket->SendBuf(&plc_Data, sizeof(plc_Data));
//			if(plc_index == PLC_INDEX_INTERFACE[0])
//				PLC_DataChange(0, PLC_D_INTERFACE_START_DEV_NUM, DEVCODE_D, PLC_D_INTERFACE_LEN);
//
//			ClientSocket_PLC->Socket->SendBuf(&plc_Data, sizeof(plc_Data));
			plc_ReadFlag = false;
            plc_ReadCount = 0;
		}
		else if(plc_ReadCount > 10)		//	3�ʵ��� ����Ȯ��
			ClientSocket_PLC->Close();

		plc_ReadCount++;
	}
	else ClientSocket_PLC->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::Timer_PLC_AutoConnectTimer(TObject *Sender)
{
	ClientSocket_PLC->Active = true;
	Timer_PLC_AutoConnect->Enabled = false;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	������ ����
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::PLC_DataChange(int subCommand, int address, int devCode, int devLen)
{
	plc_Data.SubCommand[0] = subCommand;
	plc_Data.SubCommand[1] = 0x00;

	plc_Data.StartDevNum[0] = address % 256;
	plc_Data.StartDevNum[1] = (address / 256) % 256;
	plc_Data.StartDevNum[2] = address / (256 * 256);

	plc_Data.DevCode = devCode;

	plc_Data.DevLen[0] = devLen % 256;
	plc_Data.DevLen[1] = devLen / 256;
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::PLC_Recv_Interface(int nTag)
{
	int num = 0;
	plc_Interface = plc_Read.SubString(23, PLC_D_INTERFACE_LEN);

	for(int i = 0; i < PLC_D_INTERFACE_LEN; i++)
	{
		plc_Interface_Data[i][0] = StrToInt("0x" + plc_Read.SubString(23 + num, 2));
		plc_Interface_Data[i][1] = StrToInt("0x" + plc_Read.SubString(23 + num + 2, 2));
		num += 4;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::PLC_Recv_Interface_CellSerial(int index, int wordsToRead)
{
    int num = 0;

    for(int i = 0; i < wordsToRead; i++)
    {
        int destIndex = i + (index * PLC_D_CELL_SERIAL_READLEN);
        if(destIndex >= PLC_D_CELL_SERIAL_LEN) break; // 5760 �̻��̸� �ߴ�. overflow ����

        plc_Interface_Cell_Serial[destIndex][0] = StrToInt("0x" + plc_Read.SubString(23 + num, 2));
        plc_Interface_Cell_Serial[destIndex][1] = StrToInt("0x" + plc_Read.SubString(23 + num + 2, 2));
        num += 4;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	������ ���� & �б�
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetData(unsigned char (*data)[2], int column, int num, bool flag)
{
	int num1 = num / 8;
	int num2 = num % 8;

	if(flag) data[column][num1] |= (1 << num2);
	else data[column][num1] &= ~(1 << num2);
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetDouble(unsigned char (*data)[2], int column, double value)
{
//	int temp = value;
//
//	data[column][1] = temp / 256;
//	data[column][0] = temp % 256;
    short temp = static_cast<short>(value); // signed 16-bit ������ ����ȯ

    data[column][0] = temp & 0xFF;         // LSB (���� ������ ���� �ٸ�)
    data[column][1] = (temp >> 8) & 0xFF;  // MSB
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetString(unsigned char (*data)[2], int column, AnsiString msg)
{
	if(msg.Length() % 2) msg += (char)0x0;

	int num = 0;
	for(int i = 0; i < msg.Length() / 2; i++)
	{
		data[column + i][0] = msg[1 + num];
		data[column + i][1] = msg[2 + num];
		num += 2;
	}
}
//---------------------------------------------------------------------------
int __fastcall TMod_PLC::GetData(unsigned char (*data)[2], int column, int num)
{
	bool value = false;
	int num1 = num / 8;
	int num2 = num % 8;

	value = data[column][num1] & (1 << num2);

	return value;
}
//---------------------------------------------------------------------------
double __fastcall TMod_PLC::GetDouble(unsigned char (*data)[2], int column)
{
	double value = -1;

	value = (data[column][1] * 256) + data[column][0];

	return value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_PLC::GetString(unsigned char (*data)[2], int column, int count)
{
	AnsiString m_GetStringValue = "";
	for(int i = 0; i < count; i++)
	{
		m_GetStringValue += (char)data[column + i][0];
		m_GetStringValue += (char)data[column + i][1];
	}

	return m_GetStringValue.Trim();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//	������ ���� & �б�
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// PLC ��ɾ�
//---------------------------------------------------------------------------
int __fastcall TMod_PLC::GetTrayPos()
{
    int value = GetPlcValue(PLC_D_PRE_TRAY_POS);
    return value;
}
//---------------------------------------------------------------------------
double __fastcall TMod_PLC::GetPlcValue(int plc_address)
{
    double value = GetDouble(plc_Interface_Data, plc_address);
    return value;
}
//---------------------------------------------------------------------------
int __fastcall TMod_PLC::GetPlcData(int plc_address, int bit_num)
{
    int value = GetData(plc_Interface_Data, plc_address, bit_num);
    return value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_PLC::GetPlcValue(int plc_address, int size)
{
    AnsiString value = GetString(plc_Interface_Data, plc_address, size);
    return value;
}
//---------------------------------------------------------------------------
double __fastcall TMod_PLC::GetPcValue(int pc_address)
{
    double value = GetDouble(pc_Interface_Data, pc_address);
    return value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_PLC::GetCellSrial(int plc_address, int index, int size)
{
    AnsiString value = GetString(plc_Interface_Cell_Serial, plc_address + index * 10, size);
    return value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TMod_PLC::GetCellSrialTrayId(int plc_address, int size)
{
    AnsiString value = GetString(plc_Interface_Cell_Serial, plc_address, size);
    return value;
}
//---------------------------------------------------------------------------
double __fastcall TMod_PLC::GetCellSrialValue(int plc_address)
{
    double value = GetDouble(plc_Interface_Cell_Serial, plc_address);
    return value;
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetPcData(int pc_address, int bit_num, bool bValue)
{
    SetData(pc_Interface_Data, pc_address, bit_num, bValue);
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetPcValue(int pc_address, int value)
{
    SetDouble(pc_Interface_Data, pc_address, value);
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetSpecValue(int pc_address, int value)
{
	SetDouble(pc_Interface_Data, pc_address, static_cast<int16_t>(value));
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetVoltValue(int pc_address, int index, int value)
{
	SetDouble(pc_Interface_Volt_Data, pc_address + index, static_cast<int16_t>(value));
}
//---------------------------------------------------------------------------
void __fastcall TMod_PLC::SetCurrValue(int pc_address, int index, int value)
{
    SetDouble(pc_Interface_Curr_Data, pc_address + index, static_cast<int16_t>(value));
}
//---------------------------------------------------------------------------
int __fastcall TMod_PLC::GetVoltValue(int pc_address, int index)
{
    int lowWord = GetDouble(pc_Interface_Volt_Data, pc_address + index);
    return lowWord;
}
//---------------------------------------------------------------------------
int __fastcall TMod_PLC::GetCurrValue(int pc_address, int index)
{
    int lowWord = GetDouble(pc_Interface_Curr_Data, pc_address + index);
    return lowWord;
}
//---------------------------------------------------------------------------
// PLC ��ɾ�
//---------------------------------------------------------------------------




