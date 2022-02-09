/*
 * main.c
 *
 *  Created on: Dec 20, 2021
 *      Author: amagd
 */


#include "LIB/LSTD_TYPES.h"
#include "MCAL/MDIO/MDIO_Interface.h"
#include "Free_RTOS/FreeRTOS.h"
#include "Free_RTOS/task.h"
#include "Free_RTOS/semphr.h"
#include "util/delay.h"
#include "key.h"
#include "LCD.h"


#define ID_NUM_DIGITS 4
#define PASSWORD_NUM_DIGITS 4

u64 power(u16 base, u16 exp)
{
	u32 res = 1;

	for (u16 i = 0; i < exp; i++) {
		res *= base;
	}
	return res;
}

u16 chartoint(u8 char_number){
	u16 num =0;
	num = char_number - '0';

	return num;
}


xSemaphoreHandle LCDSem;
xSemaphoreHandle CardSem;
xSemaphoreHandle PassSem;
xSemaphoreHandle OptionSem;
xSemaphoreHandle CashSem;
xSemaphoreHandle BalanceSem;
xSemaphoreHandle ExitSem;



typedef struct
{
	u64 ID;
	u16 Password;
	u32 Balance;
	u8 name[20];

} DataBase;
/*
DataBase Customers[] = {
		{203659302214, 1783, 3500166},
		{203659302214, 1390, 520001},
		{126355700193, 1212, 11100},
		{201455998011, 2001, 1200},
		0
};
 */
DataBase Customers[] = {
		{2036, 1783, 3500166,"ahmed abdelrazek"},
		{2214, 1390, 520001,"salma mohamed"},
		{1263, 1212, 11100,"adel khaled"},
		{2014, 2001, 1200,"saeed admin"},
		{0, 0, 0}
};

u32 Global_Balance = 0;
u8 *ptr;

/*API TASKS*/
void APP_VidWelcomeTask(void*pv);
void APP_VidGet_ID_From_Keypad(void *pv);
void APP_VidGet_PASS_From_Keypad(void *pv);

void APP_VidChoiceOption(void *pv);
//void APP_VidCashWithdraw(void *pv);
void APP_VidShowBalance(void *pv);
//void APP_VidCangePass(void *pv);
void APP_vidTaskExit(void *pv);

int main(void)
{
	/*semaphore welcome Task*/
	LCDSem     = xSemaphoreCreateCounting(1, 1);
	/*semaphore card use to enter ID*/
	CardSem    = xSemaphoreCreateCounting(1, 0);
	/*semaphore password*/
	PassSem    = xSemaphoreCreateCounting(1, 0);
	/*semaphore choice from list ATM*/
	OptionSem  = xSemaphoreCreateCounting(1, 0);
	/*semaphore Cash withdraw Task*/
	CashSem    = xSemaphoreCreateCounting(1,0);
	/*semaphore Balance inquiry Task*/
	BalanceSem = xSemaphoreCreateCounting(1,0);
	/*semaphore Exit Task*/
	ExitSem    = xSemaphoreCreateCounting(1, 0);


	MDIO_Error_State_SetPinDirection(PIN0, MDIO_PORTB, PIN_INPUT);
	MDIO_Error_State_SetPinDirection(PIN1, MDIO_PORTB, PIN_INPUT);

	lcd_init();
	keypad_init();

	xTaskCreate(APP_VidWelcomeTask,NULL,85,NULL,5,NULL);
	xTaskCreate(APP_VidGet_ID_From_Keypad,NULL,85,NULL,1,NULL);


	xTaskCreate(APP_VidGet_PASS_From_Keypad,NULL,85,NULL,1,NULL);
	xTaskCreate(APP_VidChoiceOption,NULL,85,NULL,1,NULL);
	//xTaskCreate(APP_VidCashWithdraw,NULL,100,NULL,1,NULL);
	xTaskCreate(APP_VidShowBalance,NULL,85,NULL,1,NULL);
	//xTaskCreate(APP_VidCangePass,NULL,100,NULL,1,NULL);
	xTaskCreate(APP_vidTaskExit,NULL,85,NULL,5,NULL);

	vTaskStartScheduler();

	while(1)
	{

	}

	return 0;
}

void APP_VidWelcomeTask(void *Pv)
{
	u8 Loc_u8ButtonState = 1;
	u8 LCDSemState = 0;
	u8 LCDFlag = 0;

	while(1) {

		LCDSemState=xSemaphoreTake(LCDSem,10);
		if(LCDSemState==pdPASS)
		{
			if (!LCDFlag) {
				lcd_clear();
				lcd_write_string("NTI Bank...");
				LCDFlag = 1;
			}
			MDIO_Error_State_GetPinValue(PIN0, MDIO_PORTB, &Loc_u8ButtonState);
			if(Loc_u8ButtonState == 0)
			{
				xSemaphoreGive(CardSem);
				LCDFlag = 0;
			}
			else
			{
				xSemaphoreGive(ExitSem);
			}
		}
		else
		{
			/*Do Nothing*/
		}

		vTaskDelay(20);
	}
}

void APP_VidGet_ID_From_Keypad(void *Pv)
{
	u8 Loc_u8CardSemState;
	char Loc_u8KeyPressed = 'N';
	u16 ID[12]={0};
	u64 id = 0;
	u8 Loc_u8Flag=0;
	u8 Loc_Count = 0;

	while(1)
	{
		Loc_Count = 0;
		Loc_u8KeyPressed = 'N';
		Loc_u8Flag=0;
		id = 0;
		Loc_u8CardSemState = xSemaphoreTake(CardSem,10);
		if(Loc_u8CardSemState==pdPASS)
		{
			lcd_clear();
			lcd_write_string("EnterID:");
			//lcd_goto_xy(1,0);
			while (Loc_u8KeyPressed != '*')
			{
				if (Loc_Count > 11)
					break;
				if(Loc_u8KeyPressed != 'N')
				{
					ID[Loc_Count] = chartoint(Loc_u8KeyPressed);
					lcd_Write_Number(ID[Loc_Count]);
					Loc_Count++;
				}
				else
				{
					/*Do Nothing*/
				}
				Loc_u8KeyPressed = keypad_getchar();
			}
			for (u8 i = ID_NUM_DIGITS; i >= 1; i--) {
				id += ID[ID_NUM_DIGITS - i] * power(10, i - 1);
			}
			Loc_Count = 0;
			while(Customers[Loc_Count].ID != 0)
			{
				if(Customers[Loc_Count].ID == id)
				{
					lcd_clear();
					lcd_write_string("Enter PASS:");
					Loc_u8Flag = 1;
					xSemaphoreGive(PassSem);
					break;
				}
				else
				{
					Loc_Count++;
				}
			}
			if(Loc_u8Flag == 0)
			{
				lcd_clear();
				lcd_write_string("ID Is not Valid..");
				xSemaphoreGive(ExitSem);
			}

		}
		else
		{
			/*Do Nothing*/
		}
		vTaskDelay(100);
	}
}



void APP_VidGet_PASS_From_Keypad(void *Pv) {
	u8 Loc_u8CardSemState;
	char Loc_u8KeyPressed = 'N';
	u16 pass[5]={0};
	//u16 Loc_u16PASS;
	u8 Loc_Count=0;
	u16 password;
	u8 Loc_u8Flag=0;

	while(1)
	{
		Loc_u8Flag = 0;
		Loc_Count = 0;
		password = 0;
		Loc_u8CardSemState = xSemaphoreTake(PassSem,10);
		if(Loc_u8CardSemState==pdPASS)
		{
			while(Loc_u8KeyPressed != '*')
			{
				if (Loc_Count > 3)
					break;
				if(Loc_u8KeyPressed != 'N') {
					pass[Loc_Count] = chartoint(Loc_u8KeyPressed);
					Loc_Count++;
					lcd_write_char('*');
				} else {
					/*Do Nothing*/
				}
				Loc_u8KeyPressed = keypad_getchar();
			}
			//Loc_u16PASS=(PASS[0]*1000)+(PASS[1]*100)+(PASS[2]*10)+(PASS[3]);
			for (u8 i = PASSWORD_NUM_DIGITS; i >= 1; i--) {
				password += pass[PASSWORD_NUM_DIGITS - i] * power(10, i - 1);
			}
			//lcd_clear();
			lcd_Write_Number(password);
			//_delay_ms(5000);
			Loc_Count = 0;
			while(Customers[Loc_Count].Password != 0)
			{
				if(Customers[Loc_Count].Password == password)
				{
					lcd_clear();
					lcd_write_string("WAITING..");
					Loc_u8Flag = 1;
					xSemaphoreGive(OptionSem);
					Global_Balance = Customers[Loc_Count].Balance;
					ptr = Customers[Loc_Count].name;
					break;
				}
				else
				{
					Loc_Count++;
				}
			}
			if(Loc_u8Flag==0)
			{
				lcd_clear();
				lcd_write_string("PASS Is not Valid..");
				xSemaphoreGive(ExitSem);

			}
		}
		else
		{
			/*Do Nothing*/
		}
		vTaskDelay(100);
	}
}



void APP_VidChoiceOption(void *pv){

	u8 Loc_chioceList;
	u8 Loc_Liststate;
	static u8 need_poll = 0;

	while(1) {
		Loc_Liststate = xSemaphoreTake(OptionSem,10);
		if(Loc_Liststate == pdPASS || need_poll == 1) {
			need_poll = 1;
			if (Loc_Liststate == pdPASS) {
				lcd_clear();
				lcd_goto_xy(1,0);
				lcd_write_string("1:Cash");
				lcd_goto_xy(1,7);
				lcd_write_string("2:Balance");
				lcd_goto_xy(2,0);
				lcd_write_string("3:New PASS");
				lcd_goto_xy(2,10);
				lcd_write_string("4:Exit");
			}
			Loc_chioceList = keypad_getchar();

			if (Loc_chioceList == '1') {
				xSemaphoreGive(CashSem);
				need_poll = 0;
			} else if (Loc_chioceList == '2') {
				xSemaphoreGive(BalanceSem);
				need_poll = 0;
			} else if (Loc_chioceList == '3') {
				xSemaphoreGive(PassSem);
				need_poll = 0;
			} else if (Loc_chioceList == '4') {
				xSemaphoreGive(ExitSem);
				need_poll = 0;
			}
		} else{
			/*do nothing */
		}

		vTaskDelay(50);
	}

}




void APP_VidShowBalance(void *pv)
{

	u8 Loc_showbalance;
	u8 Loc_u8ButtonState = 0;
	while(1) {
		Loc_showbalance = xSemaphoreTake(BalanceSem,10);
		if(Loc_showbalance == pdPASS) {
			lcd_clear();
			lcd_write_string("Balance:");
			lcd_Write_Number(Global_Balance);
			lcd_goto_xy(2,0);
			lcd_write_string(ptr);
			xSemaphoreGive(ExitSem);
		}
			else {
				/*do nothing*/
			}

			vTaskDelay(100);
		}
}


void APP_vidTaskExit(void *pv)
{
	u8 Loc_u8exit;
	u8 Loc_u8ButtonState = 0;
	while(1) {

		MDIO_Error_State_GetPinValue(PIN1, MDIO_PORTB, &Loc_u8ButtonState);
		Loc_u8exit = xSemaphoreTake(ExitSem,10);
		if(Loc_u8exit == pdPASS || Loc_u8ButtonState == 0){
			xSemaphoreGive(LCDSem);
		}
		else {

		}

		vTaskDelay(50); /*80*/
	}
}
