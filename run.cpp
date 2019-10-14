#include <stdlib.h>
#include <stdio.h>


using namespace std;

int main(int argc, char *argv[])
{
    char arc[2484];
    printf("Run system command:\n\n:>");
    gets(arc);
    system(arc);
    system("PAUSE");
    return EXIT_SUCCESS;
}
