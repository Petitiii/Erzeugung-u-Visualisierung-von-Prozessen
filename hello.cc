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
    
  
  ofstream myFile_Handler;
  //Oeftnet die Datei
  myFile_Handler.open("kp.txt");

  
  

  char* argument_list[] = { NULL};

  // Ueberprueft ob das erstellen des Prozesses erfolgreich war.
  if (pid < 0) {
    // Fehler beim Erstellen des Kind Prozesses
    cout << "Lief Schlecht";
    return 1;
  } else if (pid == 0) {
    // Child process.
    
    std::cout << "Ich Existiere";
    myFile_Handler << execvp("gnome-terminal", argument_list) << endl;
    std::cout << "Glaube ich darf nicht ausgegeben werden";

    
  }
  //Schliesst die datei
  ifstream statm(statm_file);
  int size;
  statm >> size;
  myFile_Handler <<"Size: "<<size <<"lol";
  myFile_Handler.close();

  return 0;
}
