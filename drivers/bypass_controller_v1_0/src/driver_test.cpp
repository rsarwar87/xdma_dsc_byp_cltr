#include "DSC_controller.h"

int main ()
{
    DSCcontroller *ptr = new DSCcontroller (0x0, true, false, false, 0x36B000000, 0x36B100000, 0x2000, 0x2 );

    ptr->IssueCommand();



}
