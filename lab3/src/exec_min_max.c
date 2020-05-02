#include <stdio.h>
#include <unistd.h>

int main(void)
{
execl("sequential_min_max"," ","50","70",NULL);
return 0;
}