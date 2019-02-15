#include "CANManager.h"
#include "CANManager_Cfg.h"

void Callbkfn1(void);
void Callbkfn2(void);

const SignalParameters SIG_ConfigParam [Signal_Groups_Number]=
{
    {
        0,
        0,
        3,
        0,
        &Callbkfn1
    },
    {
        0,
        4,
        7,
        0,
        &Callbkfn2
    }
};
