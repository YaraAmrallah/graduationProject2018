
#include <CommonFunctions.h>



/*
 * This function compares two arrays by specifying address of the firs M1,
 * Address of the second M2 and number of elements to compare n.
*/
int memCmp(const void* M1, const void* M2, uint32_t n)
{
        const uint8_t *Ptr1 = M1, *Ptr2 = M2;
        uint8_t RetVar = 0;
        while(n>0)
        {
            if(*Ptr1 != *Ptr2)
            {
                RetVar = *Ptr1 - *Ptr2;
                break;
                /*  Suggestion
                 *  ----------
                 *  while(n>0 && RetVar == 0)
                 */
            }
            else
            {
             Ptr1++, Ptr2++;
            }
            n--;
        }
        return RetVar;
}



