#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
ofstream myFile_Handler;
enum{
  Parent = 0,
  child
};
 vector<int> pidArray;

stringstream readstat(string path)
{
  stringstream infos;
  
  return 
}
string procReq(pid_t pid, string info)
{
    string path = "/proc/" + to_string(pid) + "/" + info;
    return readFile(path);
}
vector<vector<string>> getStatData(vector<pid_t> *prozesse)
  {
    
    vector<vector<string>> stats;
    string procPIDstat;
    for (pid_t pid : *prozesse)
    {
        procPIDstat = procReq(pid, "stat"); // info von /proc/[pid]/stat

        int i = 0, words = 44;
        vector<string> statArray(words);
        stringstream ssin(procPIDstat);
        while (ssin.good() && i < words)
        {
            ssin >> statArray[i];
            i++;
        }
        stats.push_back(statArray);
    }
    return stats;
  }



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
  // Array um festzulegen welche Zeilen nur angezeigt werden sollen
  int ausgabe[5] = {1,4,6,7,9};
  int linecounter = 0;
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
      cout << line << std::endl;
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
  pidArray.push_back (currpid);
  
  // File path um spaeter informationen auslesen zu koennen.
  //string statm_file = "/proc/" + to_string(pid) + "/statm";
  //Schreibt in die Datei 
  //writeStatus(pid);

  // Oeftnet die Datei
 

  char *argument_list[] = {NULL};
  const char *args =NULL;


  // Ueberprueft ob das erstellen des Prozesses erfolgreich war.

  if (fork() == -1){
    sleep(2);
    std::cout << "Ich Existiere";
    execl("./helloworld",args,(char *)NULL);
    std::cout << "Das darf nicht ausgegeben werden, wenn alles gut lief";
  }
  

    // Child process.
  //int childpid = getpid();
  //std::cout << "childpid:" << childpid << "\n";
  int pid = getpid();
  pidArray.push_back (pid);
  if(currpid == pid){
  std::cout << "parentid:" << pid << "\n"; 
  writeStatus(pid, Parent);
  readStatus(Parent);
  }else{
  std::cout << "childpid:" << pid << "\n"; 
  writeStatus(pid, child);
  readStatus(child);
  
  }
  
  string statm_file = "/proc/" + to_string(pid) + "/statm";
  // Schliesst die datei
  ifstream statm(statm_file);


  int size;
  statm >> size;

  return 0;
}

