#include <stdio.h>
#include "defs.h"
#include "utils.h"

void startpump(char * mssge)
{
     double x = PRICE;
     printf("%s.\nPrice is $%5.2f per gallon.\n", mssge, x);
}
