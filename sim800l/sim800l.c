#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "sim800l_cmds.h"
#include "sim800l.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"

///Pin Config
#define TXD  				(GPIO_NUM_4)
#define RXD  				(GPIO_NUM_16)
#define RESET_PIN 			(GPIO_NUM_13)

#define GSM_DEBUG 			1
#define BUF_SIZE 			(1024)
#define GSM_OK_Str 			"OK"

#define uart_num 			UART_NUM_1
#define uart_baudrate		38400
		
		
static const char* TAG = "[SIM800L DRIVER]";

static uint8_t gsm_connected = 0;
static uint8_t gprs_connected = 0;
static uint8_t htpp_connected  = 0;
static uint8_t uart_configured = 0;


uint8_t runSingleGSMCommand(GSM_Cmd* command)
{
	return atCmd_waitResponse(command->cmd, command->cmdResponseOnOk, NULL, command->cmdSize, command->timeoutMs, NULL, 0);
}

static void enableAllInitCmd(GSM_Cmd* init[], int CmdsSize)
{
	for (int idx = 0; idx < CmdsSize; idx++) {
		init[idx]->skip = 0;
	}
}

void resetSim800l()
{
	gpio_set_direction(RESET_PIN,GPIO_MODE_OUTPUT);
	
	gpio_set_level(RESET_PIN, 1);
	vTaskDelay(200 / portTICK_RATE_MS);	
	
	gpio_set_level(RESET_PIN, 0);
	vTaskDelay(2500 / portTICK_RATE_MS);
	
	gpio_set_level(RESET_PIN, 1);
	
	uint8_t res = runSingleGSMCommand(&cmd_deactGPRS);
	if(res)
	{
		ESP_LOGW(TAG,"SIM800L GPRS is deactivated");
	}	
	res = runSingleGSMCommand(&cmd_deactBearer);
	if(res)
	{
		ESP_LOGW(TAG,"SIM800L Bearer profile is deactivated");
	}
	
	res = runSingleGSMCommand(&cmd_terminateHTTP);
	if(res)
	{
		ESP_LOGW(TAG,"SIM800L HTTP Session is deactivated");
	}
	
	gsm_connected = 0;
	gprs_connected = 0;
	htpp_connected = 0;
	
	ESP_LOGI(TAG,"SIM800L is reset!");
}

static void infoCommand(char *cmd, int cmdSize, char *info)
{
	char buf[cmdSize+2];
	memset(buf, 0, cmdSize+2);

	for (int i=0; i<cmdSize;i++) {
		if ((cmd[i] != 0x00) && ((cmd[i] < 0x20) || (cmd[i] > 0x7F))) 
			buf[i] = '.';
		else 
			buf[i] = cmd[i];
		if (buf[i] == '\0') 
			break;
	}
	ESP_LOGI(TAG,"%s [%s]", info, buf);
}

void configureUART()
{
	gpio_set_direction(TXD, GPIO_MODE_OUTPUT);
	gpio_set_direction(RXD, GPIO_MODE_INPUT);
	gpio_set_pull_mode(RXD, GPIO_PULLUP_ONLY);
	
    uart_config_t uart_config = {
        .baud_rate = uart_baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, TXD, RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);
	
	uart_configured = 1;
	
	///Reset Pin -> Active Low Pin
	gpio_set_direction(RESET_PIN,GPIO_MODE_OUTPUT);
    gpio_set_level(RESET_PIN, 1);
}

static uint8_t atCmd_waitResponse(char * cmd, char *resp, char * resp1, int cmdSize, int timeout, char **response, int size)
{
	char sresp[256] = {'\0'};
	char data[256] = {'\0'};
    int len, res = 1, idx = 0, tot = 0, timeoutCnt = 0;

	// ** Send command to GSM
	vTaskDelay(100 / portTICK_PERIOD_MS);
	uart_flush(uart_num);

	if (cmd != NULL) {
		if (cmdSize == -1) 
			cmdSize = strlen(cmd);
		#if GSM_DEBUG
		infoCommand(cmd, cmdSize, "AT COMMAND:");
		#endif
		uart_write_bytes(uart_num, (const char*)cmd, cmdSize);
		uart_wait_tx_done(uart_num, 100 / portTICK_RATE_MS);
	}

	if (response != NULL) 
	{
		// Read GSM response into buffer
		char *pbuf = *response;
		len = uart_read_bytes(uart_num, (uint8_t*)data, 256, timeout / portTICK_RATE_MS);
		while (len > 0) {
			if ((tot+len) >= size) {
				char *ptemp = realloc(pbuf, size+512);
				if (ptemp == NULL) return 0;
				size += 512;
				pbuf = ptemp;
			}
			memcpy(pbuf+tot, data, len);
			tot += len;
			response[tot] = '\0';
			len = uart_read_bytes(uart_num, (uint8_t*)data, 256, 100 / portTICK_RATE_MS);
		}
		*response = pbuf;
		return tot;
	}
    // ** Wait for and check the response
	idx = 0;
	while(1)
	{
		memset(data, 0, 256);
		len = 0;
		len = uart_read_bytes(uart_num, (uint8_t*)data, 256, 10 / portTICK_RATE_MS);
		if (len > 0) 
		{
			for (int i=0; i<len;i++) {
				if (idx < 256) {
					if ((data[i] >= 0x20) && (data[i] < 0x80)) 
						sresp[idx++] = data[i];
					else 
						sresp[idx++] = 0x2e;
				}
			}
			tot += len;
		}
		else 
		{
			if (tot > 0) {
				// Check the response
				if (strstr(sresp, resp) != NULL) 
				{
					#if GSM_DEBUG
					ESP_LOGI(TAG,"AT RESPONSE: [%s]", sresp);
					#endif
					break;
				}
				else 
				{
					if (resp1 != NULL) 
					{
						if (strstr(sresp, resp1) != NULL) 
						{
							#if GSM_DEBUG
							ESP_LOGI(TAG,"AT RESPONSE (1): [%s]", sresp);
							#endif
							res = 2;
							break;
						}
					}
					// no match
					#if GSM_DEBUG
					ESP_LOGI(TAG,"AT BAD RESPONSE: ---> [%s]", sresp);
					#endif
					res = 0;
					break;
				}
			}
		}

		timeoutCnt += 10;
		if (timeoutCnt > timeout) {
			// timeout
			#if GSM_DEBUG
			ESP_LOGE(TAG,"AT: TIMEOUT");
			#endif
			res = 0;
			break;
		}
	}

	return res;
}


uint8_t runGSMCommands(GSM_Cmd* init[], int CmdsSize)
{
	#if GSM_DEBUG
		ESP_LOGI(TAG,"Initialization starting...");
	#endif
	
	enableAllInitCmd(init, CmdsSize);
	
	vTaskDelay(500 / portTICK_PERIOD_MS);
			
	int nfail = 0;
	// * GSM Initialization loop
	for(int gsmCmdIter = 0; gsmCmdIter < CmdsSize; gsmCmdIter++)
	{
		if (init[gsmCmdIter]->skip) {
			#if GSM_DEBUG
			infoCommand(init[gsmCmdIter]->cmd, init[gsmCmdIter]->cmdSize, "Skip command:");
			#endif
			continue;
		}
		if (atCmd_waitResponse(init[gsmCmdIter]->cmd,
				init[gsmCmdIter]->cmdResponseOnOk, NULL,
				init[gsmCmdIter]->cmdSize,
				init[gsmCmdIter]->timeoutMs, NULL, 0) == 0)
		{
			#if GSM_DEBUG
			ESP_LOGW(TAG,"Wrong response, retrying...");
			#endif

			nfail++;
			if(nfail>Restart_Treshold)
			{
				#if GSM_DEBUG
				ESP_LOGE(TAG,"Initialization failed after 10 tries...");
				#endif
				resetSim800l();
				return 0;
				break;
			}				
			vTaskDelay(3000 / portTICK_PERIOD_MS);
			gsmCmdIter = 0;
		}
		if (init[gsmCmdIter]->delayMs > 0)
		{
			vTaskDelay(init[gsmCmdIter]->delayMs / portTICK_PERIOD_MS);
		}			
					
		init[gsmCmdIter]->skip = 1;
	}
	
	if(CmdsSize == GSM_Init_CmdsSize)
	{
		gsm_connected = 1;
	}else if(CmdsSize == GPRS_Init_CmdsSize)
	{
		gprs_connected = 1;
	}else if(CmdsSize == HTTP_Init_CmdsSize)
	{
		htpp_connected = 1;
	}	
	#if GSM_DEBUG
		ESP_LOGI(TAG,"Init succesful!");
	#endif
	
	return 1;
}

static uint8_t initGSM()
{
	uint8_t res = runGSMCommands(GSM_Init, GSM_Init_CmdsSize);
	
	if(res)
	{
		gsm_connected = 1;		
		return res;
	}else
	{
		gsm_connected = 0;
		return 0;
	}
}

static uint8_t initGPRS()
{
	uint8_t res = runGSMCommands(GPRS_Init, GPRS_Init_CmdsSize);
	
	if(res)
	{
		gprs_connected = 1;		
		return res;
	}else
	{
		gprs_connected = 0;
		return 0;
	}
}

static uint8_t initHTTP()
{
	uint8_t res = runGSMCommands(HTTP_Init, HTTP_Init_CmdsSize);
	
	if(res)
	{
		htpp_connected = 1;		
		return res;
	}else
	{
		htpp_connected = 0;
		return 0;
	}
}

uint8_t HTTP_Get()
{
	if(!gsm_connected)
	{
		while(!gsm_connected)
		{
			initGSM();
		}
	}
	
	if(!gprs_connected)
	{
		while(!gprs_connected)
		{
			initGPRS();
		}
	}
	
	if(!htpp_connected)
	{
		while(!htpp_connected)
		{
			initHTTP();
		}
	}
	
	uint8_t setURL = runSingleGSMCommand(&cmd_setHTTPURL);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	uint8_t startSession = runSingleGSMCommand(&cmd_startHTTPGetSession);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	uint8_t readHTTPResponse = runSingleGSMCommand(&cmd_readHTTPServerResponse);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	uint8_t readHTTP = runSingleGSMCommand(&cmd_HTTPRead);
	
	if(setURL && startSession && readHTTPResponse && readHTTP)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}    

uint8_t HTTP_Post(char* dataToSend)
{
	if(!gsm_connected)
	{
		while(!gsm_connected)
		{
			initGSM();
		}
	}
	
	if(!gprs_connected)
	{
		while(!gprs_connected)
		{
			initGPRS();
		}
	}
	
	if(!htpp_connected)
	{
		while(!htpp_connected)
		{
			initHTTP();
		}
	}
	
	uint8_t setBearer = runSingleGSMCommand(&cmd_setHTTPBearer);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	uint8_t setURL = runSingleGSMCommand(&cmd_setHTTPPostURL);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	uint8_t startSession = runSingleGSMCommand(&cmd_setHTTPPostData);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	uint8_t setBoundary = runSingleGSMCommand(&cmd_setHTTPPostBoundary);
	//Start sendind data over UART
	vTaskDelay(100 / portTICK_PERIOD_MS);
	uart_flush(uart_num);
	uart_write_bytes(uart_num,dataToSend, strlen(dataToSend));
	uart_wait_tx_done(uart_num, 100 / portTICK_RATE_MS);
	vTaskDelay(10000 / portTICK_PERIOD_MS);
	
	uint8_t startHTTPPost = runSingleGSMCommand(&cmd_startHTTPPostData);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	uint8_t readHTTP = runSingleGSMCommand(&cmd_HTTPRead);
	vTaskDelay(3000 / portTICK_PERIOD_MS);
	
	if(setBearer && setURL && startSession && setBoundary && startHTTPPost)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void parseJSONResponse(const char* buffer, unsigned int bufferSize, char* response)
{

  int start_index = 0;
  int i = 0;
  while (i < bufferSize - 1 && start_index == 0) {
    char c = buffer[i];
    if ('{' == c){
      start_index = i;
    }
    ++i;
  }

  int end_index = 0;
  int j = bufferSize - 1;
  while (j >= 0 && end_index == 0) {
    char c = buffer[j];
    if ('}' == c) {
      end_index = j;
    }
    --j;
  }

  for(int k = 0; k < (end_index - start_index) + 2; ++k){
    response[k] = buffer[start_index + k];
    response[k + 1] = '\0';
  }

}


