#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <sstream> 
using namespace std;
  
int main()
{
    pid_t c_pid = fork();
    //Erstellt einen Prozess.
    int pid = fork();
    //File path um spaeter informationen auslesen zu koennen.
    string statm_file="/proc/" + to_string(pid) + "/statm";
    ifstream statm(statm_file);
  
  ofstream myFile_Handler;
  //Oeftnet die Datei
  myFile_Handler.open("kp.txt");

  
  

  char* argument_list[] = {"ls", "-l", NULL};

  // Ueberprueft ob das erstellen des Prozesses erfolgreich war.
  if (pid < 0) {
    // Fehler beim Erstellen des Kind Prozesses
    cout << "Lief Schlecht";
    return 1;
  } else if (pid == 0) {
    // Child process.
    
    cout << "Ich Existiere \n";
    myFile_Handler << execvp("ls", argument_list) << endl;
    cout << "Glaube ich darf nicht ausgegeben werden";

    
  }
  //Schliesst die datei
  myFile_Handler.close();

  return 0;
}
