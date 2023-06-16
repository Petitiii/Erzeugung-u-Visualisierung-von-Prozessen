#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
int main()
{
    std::cout <<"Hallo ich bin ein neuer Prozess"<<std::endl;
    std::cout<< "Prozess Info      \t\t\t        Speicher         \nPID\tName\tPPID\tUID\tGID\tRechte\t\tsize\tresident\tdata\n";
    return 0;
}