#include <windows.h>    //
#include <stdio.h>

int main()
{
    int ret = MessageBox(NULL, "Fenetre", "Message", MB_ABORTRETRYIGNORE);
    if(ret == IDABORT)
    {
        printf("ABORT\r\n");
    }
    else
    {
        printf("Autre chose que ABORT\r\n");
    }
    return 0;
}