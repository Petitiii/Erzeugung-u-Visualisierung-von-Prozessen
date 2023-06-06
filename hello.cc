#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <sstream> 
using namespace std;
ofstream myFile_Handler;
  
int main()
{
  
  //Erstellt einen Prozess.
  int pid = fork();
  //File path um spaeter informationen auslesen zu koennen.
  string statm_file="/proc/" + to_string(pid) + "/statm";
  writeStatus(pid);
  readStatus();
    
  
    
  
  
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
  
  writeStatus(pid);
  int size;
  statm >> size;
  myFile_Handler <<"Size: "<<size <<"lol";
  myFile_Handler.close();

  return 0;
}
void writeStatus(int processid){
    string status_file = "/proc/" + to_string(processid) + "/status";
    ifstream status(status_file);
    string line;
    while (getline(status, line)) {
      myFile_Handler << line << endl;
    }
  }
  void readStatus(){
    std::ifstream file;
    std::string line;
    file.open("kp.txt");
    if(file.is_open()) {
      while(std::getline(file, line)){
        std::cout<< line<< std::endl;
      }
      file.close();
    }
    else{
      std::cerr<< "Fehler beim öffnen der Datei";
    }

  }

