#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
ofstream myFile_Handler;
enum{
  Parent = 0,
  child
};
void writeStatus(int processid, int type)
{
  std::ofstream file;
  if(type == Parent){
    file.open("ppid.txt");
  }else{
  file.open("kp.txt");
  }

  string status_file = "/proc/" + to_string(processid) + "/status";
  ifstream status(status_file);
  string line;
  while (getline(status, line))
  {
    file << line << endl;
  }
}
void readStatus(int type)
{
  std::ifstream file;
  std::string line;
    if(type == Parent){
    file.open("ppid.txt");
  }else{
  file.open("kp.txt");
  
  }
  if (file.is_open())
  {
    while (std::getline(file, line))
    {
      std::cout << line << std::endl;
    }
    file.close();
  }
  else
  {
    std::cerr << "Fehler beim öffnen der Datei";
  }
}

int main()
{

  // Erstellt einen Prozess.
  int currpid = getpid();
  
  // File path um spaeter informationen auslesen zu koennen.
  //string statm_file = "/proc/" + to_string(pid) + "/statm";
  //Schreibt in die Datei 
  //writeStatus(pid);

  // Oeftnet die Datei
 

  char *argument_list[] = {NULL};

  // Ueberprueft ob das erstellen des Prozesses erfolgreich war.

  if (fork() == -1){
    sleep(1);
    std::cout << "Ich Existiere";
    execvp("gnome-terminal", argument_list);
    std::cout << "Das darf nicht ausgegeben werden, wenn alles gut lief";
  }
  

    // Child process.
  //int childpid = getpid();
  //std::cout << "childpid:" << childpid << "\n";
  int pid = getpid();
  if(currpid == pid){
  std::cout << "parentid:" << pid << "\n"; 
  writeStatus(pid, Parent);
  }else{
  std::cout << "childpid:" << pid << "\n"; 
  writeStatus(pid, child);
  
  }
  
  string statm_file = "/proc/" + to_string(pid) + "/statm";
  // Schliesst die datei
  ifstream statm(statm_file);


  int size;
  statm >> size;

  return 0;
}

