#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "sim800l_cmds.h"
#include "sim800l.h"

char * TAG = "[MAIN]";


void app_main()
{
	configureUART();	

	uint8_t res = runGSMCommands(GSM_Init, GSM_Init_CmdsSize);
	ESP_LOGW(TAG, "GSM Initilazation returned %d",res);
	
	res = runGSMCommands(GPRS_Init, GPRS_Init_CmdsSize);
	ESP_LOGW(TAG, "GPRS Initilazation returned %d",res);
	
	res = runGSMCommands(HTTP_Init, HTTP_Init_CmdsSize);
	ESP_LOGW(TAG, "HTTP Initilazation returned %d",res);
	
	uint8_t nPostFail = 0;
	uint8_t postRes = 0;
	uint8_t postCmdRes = 0;
	uint8_t commitPost = 0;
	
	char* dataToSend = "Hello, World!\r\n";
	//These for loops demonstrates how to use SIM800L Driver for HTTP Post&Get
	//In POST&GET methods, the processor posts and gets data from server periodically.
	//In case of failure of data transmission, ESP32 counts the failure and tries to implement instruction Restart_Treshold times.
	//After specified times of retrying, esp32 restarts the SIM800L.
	
	/*
	while(1)
	{
		uint8_t postRes = HTTP_Post(dataToSend);
		ESP_LOGW(TAG, "HTTP POST returned %d",postRes);
		vTaskDelay(3000 / portTICK_PERIOD_MS);
		
		if(postRes == 0)
		{
			nPostFail++;
		}
		
		if(nPostFail > Restart_Treshold)
		{
			nPostFail = 0;
			resetSim800l();
		}
		
		while(postRes)
		{
			postCmdRes =  runSingleGSMCommand(&cmd_setHTTPPostBoundary);
			
			if(postCmdRes)
			{
				//Start sendind data over UART
				vTaskDelay(100 / portTICK_PERIOD_MS);
				uart_flush(uart_num);
				uart_write_bytes(uart_num,dataToSend, strlen(dataToSend));
				uart_wait_tx_done(uart_num, 500 / portTICK_RATE_MS);
				vTaskDelay(5000 / portTICK_PERIOD_MS);
			
				commitPost = runSingleGSMCommand(&cmd_startHTTPPostData);
				vTaskDelay(2000 / portTICK_PERIOD_MS);
			
				ESP_LOGW(TAG, "Just HTTP POST thread is run successfully!");
			}				
							
			if(!postCmdRes || !commitPost)
			{
				nPostFail++;
				postRes = 0;
			}
			
			vTaskDelay(3000 / portTICK_PERIOD_MS);
		}
	}
	*/
	
	
	uint8_t nGetFail = 0;
	uint8_t readRes = 0;
	uint8_t readCmdRes = 0;
	
	while(1)
	{
		readRes = HTTP_Get();
		ESP_LOGW(TAG, "HTTP GET returned %d", readRes);
		vTaskDelay(5000 / portTICK_PERIOD_MS);
		
		if(!readRes)
		{
			nGetFail++;
		}
		
		if(nGetFail > Restart_Treshold)
		{
			nGetFail = 0;
			resetSim800l();
		}
		
		while(readRes)
		{
			readCmdRes =  runSingleGSMCommand(&cmd_HTTPRead);
			ESP_LOGW(TAG, "Just HTTP READ is run successfully!");
			vTaskDelay(5000 / portTICK_PERIOD_MS);
			if(!readCmdRes)
			{
				nGetFail++;
				readRes = 0;
			}
		}
	}	
		
}
