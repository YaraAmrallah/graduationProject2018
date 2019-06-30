/* ------------------------------------- Motors and Temperature sensor ECU ---------------------------------------------- */
/* -------------------------------------------- Includes ---------------------------------------------------------------- */

#include "CAN.h"
#include "CAN_Manager.h"
#include "M4MemMap.h"
#include "GPIO.h"
#include "Adc.h"
#include "SERVO.h"
#include "button.h"

/* ------------------------------------------ Global Variables ---------------------------------------------------------- */

/* CAN Reception Call Back Flag */
uint8_t receptionLeftCB_CAN = 0;
uint8_t receptionRightCB_CAN = 0;
uint8_t StartFlag = 0;
uint8_t EndFlag = 0;

uint8_t flag[BUTTON_GROUPS_NUMBER] = {0};
uint8_t dummyBuffer[8]={0};
uint8_t servoLFrame[8] = {0};
uint8_t servoRFrame[8] = {0};
uint8_t tempFrame[8] = {0};
uint8_t temp ;
uint8_t pre_ang_1=0;
uint8_t pre_ang_2=0;

void CAN_ReceptionCallBackFunctionL(void);
void CAN_DataReceptionCBL(void);
void CAN_ReceptionCallBackFunctionR(void);
void CAN_DataReceptionCBR(void);
void nullFunction(void);

int main(void)
{
    uint8_t check = 1;

    uint16_t i = 0;
    uint8_t btn_1=0,btn_2=1;
    uint16_t counter = 0 ;

    buttonState button[4]= {buttonNotPressed};


    GPIO_Init();
    Adc_Init( &Adc_Config);
    //UART_Init();
    check = Button_Init ();
    check = PWM_Init();
    check = CAN_Init();

    GPIO_SetAlternFuntion(10,4);
    GPIO_SetAlternFuntion(11,4);
    GPIO_SetAlternFuntion(4,8);
    GPIO_SetAlternFuntion(5,8);



    /* Right Servo Initiation */
    Initiate_Servo(1);

    /* Left Servo Initiation */
    Initiate_Servo(0);

    for(counter=0;counter<BUTTON_GROUPS_NUMBER ;counter++)
    {
        Button_EnableStateRequest(counter);

    }



    while (1)

    {

        CAN_MainFunction_Handling();
        Button_MainFunction_Handling();

        for(counter = 0; counter < 10000; counter++);


        for(counter=0;counter<4;counter++)
        {
            button[counter] = Button_ReturnState(counter);
        }


        if (button[0] == buttonPressed)
        {
            AngleUp_Servo(0, pre_ang_1, &pre_ang_1);
        }
        else
        {

        }

        if (button[1] == buttonPressed)
        {
            AngleUp_Servo(1, pre_ang_2, &pre_ang_2);
            // servoLFrame[0] = pre_ang_2;
        }
        else
        {

        }
        if (button[2] == buttonPressed)
        {
            AngleDown_Servo(0, pre_ang_1, &pre_ang_1);
        }
        else
        {

        }

        if (button[3] == buttonPressed)
        {
            AngleDown_Servo(1, pre_ang_2, &pre_ang_2);
            // servoLFrame[0] = pre_ang_2;
        }
        else
        {

        }



        /*
        GPIO_Read(0,&btn_1);

       if(btn_1 == 0)
        {
            servoLFrame[0] = pre_ang_1;
            AsyncTxData_Request(2,servoLFrame);
           // GPIO_Write(3, 0xff);
        }

         */

        if(StartFlag == 1)
        {
            temp=23;
            tempFrame[0]=temp;
            AsyncTxData_Request(6,tempFrame);

        }
        else
        {
            /* do nothing */
        }

        if(EndFlag == 1)
        {
            servoLFrame[0] = pre_ang_1;
            servoRFrame[0] = pre_ang_2;
            AsyncTxData_Request(2,servoLFrame);
            AsyncTxData_Request(3,servoRFrame);
            EndFlag = 0;
            StartFlag = 0;
        }

        else
        {

        }




        //  CAN_Update_Data (1, data) ;
        //  test();
        //  CAN_TX_RemoteFrame (2);
        //  test();
        // CAN_RX(2,data_r);

    }
}

/* ------------------------------------------------ Call Back Functions ------------------------------------------- */

/* CAN Reception Call Back Function */
void CAN_ReceptionCallBackFunctionL(void)
{

    AsyncRxData_Request(0,servoLFrame);

}

void CAN_DataReceptionCBL(void)
{
    uint16_t i ;
    Rotate_Servo(0, servoLFrame[0]);
    pre_ang_1=servoLFrame[0];
    GPIO_Write(2, 0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(2, 0x00);
    for(i=0; i < 20000; i++);
}

void CAN_ReceptionCallBackFunctionR(void)
{
    AsyncRxData_Request(1,servoRFrame);
}

void CAN_DataReceptionCBR(void)
{
    uint16_t i ;
    Rotate_Servo (1, servoRFrame[0]);
    pre_ang_2=servoRFrame[0];
    GPIO_Write(2, 0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(2, 0x00);
    for(i=0; i < 20000; i++);
}

void CAN_SEND_LSERVO_Done(void)
{  //omkkkkkkkkkkkkkkkkkkkkkkkkkkkkk
    uint16_t i ;
    GPIO_Write(2, 0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(2, 0x00);
    for(i=0; i < 20000; i++);
}

void START_Notify(void)
{
    uint16_t i ;
    GPIO_Write(2, 0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(2, 0x00);
    for(i=0; i < 20000; i++);
    StartFlag=1 ;
    AsyncRxData_Request(4,dummyBuffer);
}

void CAN_SEND_RSERVO_Done(void)
{
    uint16_t i ;
    GPIO_Write(2, 0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(2, 0x00);
    for(i=0; i < 20000; i++);
}

END_Notify(void)
{
    uint16_t i ;
    GPIO_Write(3,0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(3, 0x00);
    for(i=0; i < 20000; i++);

    EndFlag = 1;
    AsyncRxData_Request(5,dummyBuffer);
}


void  CAN_TEMP_Done(void)
{
    uint16_t i ;
    GPIO_Write(3,0xFF);
    for(i=0; i < 30000; i++);
    GPIO_Write(3, 0x00);
    for(i=0; i < 20000; i++);
}
void nullFunction(void)
{
    '\0';
}

