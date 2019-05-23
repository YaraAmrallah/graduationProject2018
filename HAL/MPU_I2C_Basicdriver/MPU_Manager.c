/*************************************************************
 *                      Includes
 **************************************************************/
#include "MPU_Manager.h"
#include "MPU.h"
#include <stdint.h>
#include "Reg.h"
#include <math.h>

/*************************************************************
 *                      Macros
 **************************************************************/
#define READ_SIZE 14                     // The number of bytes (Registers) to read from the sensor
#define ACCELEROMETER_SENSITIVITY 8192.0 // Sensitivity of Accelerometer Equivalent to a Full-Scale Range of (+or- 4g)
#define GYROSCOPE_SENSITIVITY 65.536     // Sensitivity of Gyroscope Equivalent to a Full-Scale Range of (+or- 500 degrees/sec)
#define M_PI 3.14159265359               // Value of Pi
#define dt 0.01                          // Sensors Read Interval of 0.01 sec = 10 msec
#define GYRO_X_OFFSET -14.1185
#define GYRO_Y_OFFSET -10.154
#define GYRO_Z_OFFSET 3.7505
/*************************************************************
 *              Private Functions Declarations
 **************************************************************/
static void MPU_GETDATA(void);

/*************************************************************
 *              Global Variables Definitions
 **************************************************************/
Accident_CheckType Indicator;

/*************************************************************
 *          Static Private Variables Definitions
 **************************************************************/

static ReadRequest_Flag RequestFlag; // A global variable indicating whether there's a "Read Request" or not
//static uint8_t* SensorReadPtr; // Pointer used in getting sensor's readings
static uint8_t SensorReadPtr[14];
static float Roll;
static float Pitch;

/**********************2***************************************
 *              Global Functions Definitions
 **************************************************************/

void MPUManager_Init(void) //Initialization of all Global Variables
{

    RequestFlag = NO_READ_REQ; //Modified
    Indicator = NO_ACCIDENT;

}
MPUManager_ReadRetType Request_MPU(void) //Requesting to read processed data
{
    MPUManager_ReadRetType RetVar;

    if (RequestFlag == NO_READ_REQ)
    {
        RequestFlag = READ_REQ;
        RetVar = REQ_VALID;
    }
    else
    {
        RetVar = REQ_INVALID;
    }

    return RetVar;
}

/*************************************************************
 *              Private Functions Definitions
 **************************************************************/

void MPU_Cyclic(void) //Processing data for read
{
    static MPUManager_State ManagerState = IDLE; // To hold the state of the function FSM
    mpu6050_CheckType CheckRead = MPU_NOK; // To hold the return value of the mpu6050 Read function
    switch (ManagerState)
    {
    case IDLE: // Nothing to do but wait for a read request
        if (RequestFlag == NO_READ_REQ)
        {
            ManagerState = IDLE;
        }
        else
        {
            ManagerState = READ_IN_PROGRESS;
        }
        break;

    case READ_IN_PROGRESS: // Just reading the sensors values as is

        CheckRead = MPU_readBytes(MPU6050_RA_ACCEL_XOUT_H, READ_SIZE,
                                  SensorReadPtr);
        if (CheckRead == MPU_OK)
        {
            MPU_GETDATA();
            ManagerState = DECISION_IN_PROGRESS;

        }
        else
        {
            ManagerState = READ_IN_PROGRESS;
        }
        break;
    case DECISION_IN_PROGRESS: // Processing the read data and calling back

        if (((Roll) >= 46) || ((Pitch) >= 46) || ((Pitch) <= -46)
                || ((Roll) <= -46))
        {
            Indicator = ACCIDENT;
        }
        else
        {
            Indicator = NO_ACCIDENT;
        }
        (*CallBack)();/*MPU Call back*/
        RequestFlag = NO_READ_REQ;
        ManagerState = IDLE;
        break;
    default:
        break;
    }

}

static void MPU_GETDATA(void)
{
    int16_t Gyro_x, Gyro_y, Gyro_z, Acc_x, Acc_y, Acc_z, Acc_total_vector; // Read raw values from sensors
    //  float Gyro_Pitch, Gyro_Roll, Output_Pitch, Output_Roll; /* Should be initialized at first with the first accelerometer's angles*/
    float Acc_Pitch, Acc_Roll;
    /**************First 6 bytes ACC *******************/
    Acc_x = (((int16_t) SensorReadPtr[0]) << 8) | SensorReadPtr[1];
    Acc_y = (((int16_t) SensorReadPtr[2]) << 8) | SensorReadPtr[3];
    Acc_z = (((int16_t) SensorReadPtr[4]) << 8) | SensorReadPtr[5];
    /**************second 6 bytes GYrO *******************/
    Gyro_x = (((int16_t) SensorReadPtr[8]) << 8) | SensorReadPtr[9];
    Gyro_y = (((int16_t) SensorReadPtr[10]) << 8) | SensorReadPtr[11];
    Gyro_z = (((int16_t) SensorReadPtr[12]) << 8) | SensorReadPtr[13];

    // 1. For each gyroscope reading offset (initialized in init fn) should be subtracted
    Gyro_x -= GYRO_X_OFFSET; //Subtract the offset calibration value from the raw Gyro_x value
    Gyro_y -= GYRO_Y_OFFSET; //Subtract the offset calibration value from the raw Gyro_y value
    Gyro_z -= GYRO_Z_OFFSET; //Subtract the offset calibration value from the raw Gyro_z value

    // 2. Gyroscope angles calculations by integration
    //Gyro_Pitch += Gyro_x * (dt / GYROSCOPE_SENSITIVITY) ; //Calculate the traveled pitch angle and add this to the Gyro_Pitch variable
    //Gyro_Roll += Gyro_y * (dt / GYROSCOPE_SENSITIVITY);   //Calculate the traveled roll angle and add this to the Gyro_Roll variable

    // 3. Accelerometer angle calculations

    Acc_total_vector = sqrt((Acc_x * Acc_x) + (Acc_y * Acc_y) + (Acc_z * Acc_z)); //Calculate the total accelerometer vector
    Acc_Pitch = asin((float) Acc_y / Acc_total_vector) * (180 / M_PI); //Calculate the pitch angle
    Acc_Roll = asin((float) Acc_x / Acc_total_vector) * (-180 / M_PI); //Calculate the roll angle
    Roll = Acc_Roll;
    Pitch = Acc_Pitch;

}
