#ifndef SIM800l_CMDS_H_
#define SIM800l_CMDS_H_

#include <stdint.h>

#define GSM_OK_Str "OK"


typedef struct
{
	char		*cmd;
	uint16_t	cmdSize;
	char		*cmdResponseOnOk;
	uint16_t	timeoutMs;
	uint16_t	delayMs;
	uint8_t		skip;
}GSM_Cmd;

static GSM_Cmd cmd_AT =
{
	.cmd = "AT\r\n",
	.cmdSize = sizeof("AT\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 3000,
	.delayMs = 0,
	.skip = 0,
};

static GSM_Cmd cmd_EnableErrorCode =
{
	.cmd = "AT+CMEE=2\r\n",
	.cmdSize = sizeof("AT+CMEE=2\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 3000,
	.delayMs = 0,
	.skip = 0,
};


static GSM_Cmd cmd_NoSMSInd =
{
	.cmd = "AT+CNMI=0,0,0,0,0\r\n",
	.cmdSize = sizeof("AT+CNMI=0,0,0,0,0\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 1000,
	.delayMs = 0,
	.skip = 0,
};

static GSM_Cmd cmd_Reset =
{
	.cmd = "ATZ\r\n",
	.cmdSize = sizeof("ATZ\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 300,
	.delayMs = 0,
	.skip = 0,
};

static GSM_Cmd cmd_RFOn =
{
	.cmd = "AT+CFUN=1\r\n",
	.cmdSize = sizeof("ATCFUN=1,0\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 1000,
	.skip = 0,
};

static GSM_Cmd cmd_EchoOff =
{
	.cmd = "ATE0\r\n",
	.cmdSize = sizeof("ATE0\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 300,
	.delayMs = 0,
	.skip = 0,
};

static GSM_Cmd cmd_Pin =
{
	.cmd = "AT+CPIN?\r\n",
	.cmdSize = sizeof("AT+CPIN?\r\n")-1,
	.cmdResponseOnOk = "CPIN: READY",
	.timeoutMs = 5000,
	.delayMs = 0,
	.skip = 0,
};

static GSM_Cmd cmd_isRegistered =
{
	.cmd = "AT+CREG?\r\n",
	.cmdSize = sizeof("AT+CREG?\r\n")-1,
	.cmdResponseOnOk = "+CREG: 1,1",
	.timeoutMs = 3000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_Register =
{
	.cmd = "AT+CREG=1\r\n",
	.cmdSize = sizeof("AT+CREG=1\r\n")-1,
	//.cmdResponseOnOk = "+CREG: 1,1",
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 3000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_GPRS =
{
	.cmd = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n",
	.cmdSize = sizeof("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n") -1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 1000,
	.skip = 0,
};


static GSM_Cmd isAttachedToGPRS =
{
	.cmd = "AT+CGATT?",
	.cmdSize = sizeof("AT+CGATT?")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 5000,
	.delayMs = 1000,
	.skip = 0,
};

static GSM_Cmd cmd_OperatorList =
{
	.cmd = "AT+COPS=?",
	.cmdSize = sizeof("AT+COPS=?")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 20000,
	.delayMs = 1000,
	.skip = 0,
};

static GSM_Cmd cmd_testCSTT =
{
	.cmd = "AT+CSTT=?",
	.cmdSize = sizeof("AT+CSTT=?")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 30000,
	.delayMs = 1000,
	.skip = 0,
};

static GSM_Cmd cmd_TCP =
{
	.cmd = "AT+CIPSTART=?",
	.cmdSize = sizeof("AT+CIPSTART=?")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 5000,
	.delayMs = 1000,
	.skip = 0,
};

static GSM_Cmd cmd_Connect =
{
	.cmd = "AT+CGDATA=\"PPP\",1\r\n",
	.cmdSize = sizeof("AT+CGDATA=\"PPP\",1\r\n")-1,
	//.cmd = "ATDT*99***1#\r\n",
	//.cmdSize = sizeof("ATDT*99***1#\r\n")-1,
	.cmdResponseOnOk = "CONNECT",
	.timeoutMs = 30000,
	.delayMs = 1000,
	.skip = 0,
};

static GSM_Cmd cmd_ListL2Ps = 
{
	.cmd = "AT+CGDATA=?\r\n",
	.cmdSize = sizeof("AT+CGDATA=?\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_APN = 
{
	.cmd = "AT+SAPBR=3,1,\"APN\",\"internet\"\r\n",
	.cmdSize = sizeof("AT+SAPBR=3,1,\"APN\",\"internet\"\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 30000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_signalQuality = 
{
	.cmd = "AT+CSQ\r\n",
	.cmdSize = sizeof("AT+CSQ\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 30000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_setCops = 
{
	.cmd = "AT+COPS=0\r\n",
	.cmdSize = sizeof("AT+COPS=0\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_attachGPRS = 
{
	.cmd = "AT+CGATT=1\r\n",
	.cmdSize = sizeof("AT+CGATT=1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 30000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_deactGPRS = 
{
	.cmd = "AT+CGATT=0\r\n",
	.cmdSize = sizeof("AT+CGATT=0\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};



static GSM_Cmd cmd_setupAPN = 
{
	//.cmd = "AT+CSTT=\"CMNET\"",
	//.cmdSize = sizeof("AT+CSTT=\"CMNET\"")-1,
	.cmd = "AT+CSTT=\"internet\",\"\",\"\"",
	.cmdSize = sizeof("AT+CSTT=\"internet\",\"\",\"\""),
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 30000,
	.delayMs = 2000,
	.skip = 0,
};


static GSM_Cmd cmd_QueryGPRS = 
{
	.cmd = "AT+SAPBR=2,1\r\n",
	.cmdSize = sizeof("AT+SAPBR=2,1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_getIP = 
{
	.cmd = "AT+CIFSR\r\n",
	.cmdSize = sizeof("AT+CIFSR\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_enableGPRS = 
{
	.cmd = "AT+SAPBR=1,1\r\n",
	.cmdSize = sizeof("AT+SAPBR=1,1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_enableErrorReport = 
{
	.cmd = "AT+CMEE=2\r\n",
	.cmdSize = sizeof("AT+CMEE=2\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_deactBearer = 
{
	.cmd = "AT+SAPBR=0,1\r\n",
	.cmdSize = sizeof("AT+SAPBR=0,1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_listPDPs = 
{
	.cmd = "AT+CGACT=?\r\n",
	.cmdSize = sizeof("AT+CGACT=?\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};



static GSM_Cmd cmd_activatePDP = 
{
	.cmd = "AT+CGACT=1,1\r\n",
	.cmdSize = sizeof("AT+CGACT=1,1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};


//------------------HTTP GET---------------------------------------
static GSM_Cmd cmd_HTTPInit = 
{
	.cmd = "AT+HTTPINIT\r\n",
	.cmdSize = sizeof("AT+HTTPINIT\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};


static GSM_Cmd cmd_setHTTPBearer = 
{
	.cmd = "AT+HTTPPARA=\"CID\",1\r\n",
	.cmdSize = sizeof("AT+HTTPPARA=\"CID\",1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_setHTTPURL = 
{
	.cmd = "AT+HTTPPARA=\"URL\",\"http://www.m2msupport.net/m2msupport/http_get_test.php\"\r\n",
	.cmdSize = sizeof("AT+HTTPPARA=\"URL\",\"http://www.m2msupport.net/m2msupport/http_get_test.php\"\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 6000,
	.skip = 0,
};

static GSM_Cmd cmd_startHTTPGetSession = 
{
	.cmd = "AT+HTTPACTION=0\r\n",
	.cmdSize = sizeof("AT+HTTPACTION=0\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 15000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_HTTPRead = 
{
	.cmd = "AT+HTTPREAD\r\n",
	.cmdSize = sizeof("AT+HTTPREAD\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 6000,
	.skip = 0,
};

static GSM_Cmd cmd_terminateHTTP = 
{
	.cmd = "AT+HTTPTERM\r\n",
	.cmdSize = sizeof("AT+HTTPTERM\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_readHTTPServerResponse = 
{
	.cmd = "AT+HTTPREAD=?\r\n",
	.cmdSize = sizeof("AT+HTTPREAD=?\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 2000,
	.skip = 0,
};
//----------------------------------------------

static GSM_Cmd cmd_startHTTPPostData = 
{
	.cmd = "AT+HTTPACTION=1\r\n",
	.cmdSize = sizeof("AT+HTTPACTION=1\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 15000,
	.delayMs = 2000,
	.skip = 0,
};

static GSM_Cmd cmd_setHTTPPostURL = 
{
	.cmd = "AT+HTTPPARA=\"URL\",\"http://ptsv2.com/t/gmg64-1528591843/post\"\r\n",
	.cmdSize = sizeof("AT+HTTPPARA=\"URL\",\"http://ptsv2.com/t/gmg64-1528591843/post\"\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 6000,
	.skip = 0,
};

//Sets HTTP data boundary and timeout interval.
//First param is the data size in bytes, second param is the timeout variable in ms.
static GSM_Cmd cmd_setHTTPPostBoundary = 
{
	.cmd = "AT+HTTPDATA=15,5000\r\n",
	.cmdSize = sizeof("AT+HTTPDATA=15,5000\r\n")-1,
	.cmdResponseOnOk = "DOWNLOAD",
	.timeoutMs = 10000,
	.delayMs = 6000,
	.skip = 0,
};

//Sets content type of the post message
static GSM_Cmd cmd_setHTTPPostData = 
{
	.cmd = "AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n",
	.cmdSize = sizeof("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n")-1,
	.cmdResponseOnOk = GSM_OK_Str,
	.timeoutMs = 10000,
	.delayMs = 6000,
	.skip = 0,
};

static GSM_Cmd *GSM_Init[] =
{
		&cmd_AT,
		&cmd_Reset,
		&cmd_EchoOff,
		&cmd_RFOn,
		&cmd_EnableErrorCode,
		&cmd_NoSMSInd,
		&cmd_Pin,
		&cmd_Register,
		&cmd_enableErrorReport,
};

static GSM_Cmd* GPRS_Init[] =
{
		&cmd_deactGPRS,
		&cmd_APN,
		&cmd_GPRS,
		&cmd_enableGPRS,
		&cmd_QueryGPRS,
};

static GSM_Cmd* HTTP_Init[] =
{
		&cmd_terminateHTTP,
		&cmd_HTTPInit,
		&cmd_setHTTPBearer,
};

#endif 