#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>
#include <vector>
#include <unistd.h> //sleep

// maps fehlt noch
// interface
//
using namespace std;
ofstream myFile_Handler;
const char *args = NULL;
enum
{
  Parent = 0,
  child
};
vector<pid_t> pidArray;
void testProzess()
{
  pid_t neuPr = fork();
  if (neuPr == 0)
  {
    execl("./TestProzess", args, (char *)NULL);
  }
  //pid_t pidneu = getpid();
  pidArray.push_back(neuPr);
  
}
void neuesTerminal()
{
  pid_t neuPr = fork();
  if (neuPr == 0)
  {
    execl("gnome-Terminal", args, (char *)NULL);
  }
  //pid_t pidneu = getpid();
  pidArray.push_back(neuPr);
  
}

string readstat(string procpath)
{
  string status_file = procpath;
  ifstream status(status_file);
  string line;
  getline(status, line);
  return line;
}
vector<vector<string>> getStatData()
{

  vector<vector<string>> stats;
  //string procStat;
  for (pid_t pid : pidArray)
  {
    string path = "/proc/" + to_string(pid) + "/stat";
    string procstat = readstat(path); // info von /proc/[pid]/stat

    int i = 0, data = 44;
    vector<string> stat(data);
    stringstream ssin(procstat);
    while (ssin.good() && i < data)
    {

      ssin >> stat[i];
      i++;
    }

    stats.push_back(stat);
  }
  return stats;
}
vector<vector<string>> mapsData()
{

  vector<vector<string>> maps;
  for (pid_t pid : pidArray)
  {
    string path = "/proc/" + to_string(pid) + "/maps";
    string mapdata = readstat(path); // info von /proc/[pid]/maps

    int i = 0, infos = 6;
    vector<string> map(infos);
    stringstream ssin(mapdata);
    while (ssin.good() && i < infos)
    {

      ssin >> map[i];
      i++;
    }

    maps.push_back(map);
  }
  return maps;
}

string printinfos()
{

  vector<vector<string>> stat = getStatData();
  vector<vector<string>> map = mapsData();

  string alleInfos = "Prozess Info\t\t\t\t\t\tSpeicher         \nPID\tName\t\tPPID\tUID\tGID\tRechte\tsize\n";
  vector<string> size;
  for(pid_t p : pidArray){
  string statmvalues;
  string statm_file = "/proc/" + to_string(p) + "/statm";

  ifstream statm(statm_file);
  statm>> statmvalues;
  size.push_back(statmvalues);
  
  }

  for (size_t i = 0; i < stat.size(); i++)
  {
    // Bei kurzem Namen Tab hinzufügen
    stat[i][1].length() < 8 ? stat[i][1] += "\t" : "";

    alleInfos += stat[i][0] + "\t" + stat[i][1] + "\t" + to_string(getpid())  + "\t" + to_string(getuid()) + "\t" + stat[i][5] + "\t" + map[i][1] + "\t" + size[i] + "\n";
  }
  return alleInfos;
}

void writeLog(string info){
  ofstream file;
  time_t now = time(0);
   
   
  char* dt = ctime(&now);

  file.open("log.txt", fstream::app);
  file<< dt<<endl;
  file<<info<<endl;

}

void readLog()
{
  std::ifstream file;
  std::string line;
  file.open("log.txt");
  if (file.is_open())
  {
    while (getline(file, line))
    {
      cout << line << endl;
    }
    file.close();
  }
  else
  {
    cerr << "Fehler beim öffnen der Datei";
  }
}
void killProzess(){
  for(pid_t eintraege: pidArray){
  kill(eintraege,SIGTERM);
  }
  
}

void auswahlmenu(){
  string info="1: Neuen Prozess erzeugen \n2: Prozessinformationen ausgeben \n3: LogFile ausgeben \n4: Alle Prozesse beenden";
  cout<<info<<endl;
  int auswahl;
  cin>>auswahl;
  switch (auswahl)
  {
  case 1:
    testProzess();
    sleep(2);
    auswahlmenu();
  case 2:
    cout<<printinfos();
    writeLog(printinfos()); 
    sleep(2);
    auswahlmenu();
  case 3:
    readLog(); 
    sleep(2);
    auswahlmenu();
  case 4:
    killProzess(); 
  
  
  default:
  cout<<"Falsche Taste"<<endl;
  break;
  }
}

int main()
{
  pidArray.push_back(getpid());// das ist damit die Prozess id von dem standart prozess anfangs gespeichert wird
  cout<<"Hallo in unserem wunderbaren Interface :D \n1Druecken sie die angezeigten Zahlen um unterschiedliche Aktionen auszufuehren"<<endl;
  auswahlmenu();
  
  
  
  
   

  return 0;
}
