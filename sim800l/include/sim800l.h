#ifndef SIM800l_H_
#define SIM800l_H_

#include <stdio.h>
#include <stdint.h>

#define GSM_Init_CmdsSize		9
#define GPRS_Init_CmdsSize		5
#define HTTP_Init_CmdsSize		3

#define uart_num 				UART_NUM_1
#define Restart_Treshold		5

uint8_t runGSMCommands(GSM_Cmd* init[], int CmdsSize);

static void infoCommand(char *, int, char *);

void configureUART();

static uint8_t atCmd_waitResponse(char *, char *, char *, int, int, char **, int );

static void enableAllInitCmd(GSM_Cmd* init[], int CmdsSize);

void resetSim800l();

uint8_t runSingleGSMCommand(GSM_Cmd* cmd);

uint8_t HTTP_Get();

uint8_t HTTP_Post(char* dataToSend);

void parseJSONResponse(const char* buffer, unsigned int bufferSize, char* response);

#endif