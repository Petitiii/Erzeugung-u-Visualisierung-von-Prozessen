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

  string alleInfos = "Prozess Info      \t\t\t\t        Speicher         \nPID\tName\tPPID\tUID\tGID\tRechte\t\tsize\n";
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

    alleInfos += stat[i][0] + "\t" + stat[i][1] + stat[i][5]  + "\t"+ "\t" + stat[i][9] + "\t" + map[i][1] + "\t\t" +size[i]+"\n";
  }
  return alleInfos;
}

void writeStatus(int processid, int type)
{
  std::ofstream file;
  if (type == Parent)
  {
    file.open("child.txt");
  }
  else
  {
    file.open("parent.txt");
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
  int ausgabe[5] = {1, 4, 6, 7, 9};
  int linecounter = 0;
  std::ifstream file;
  std::string line;
  if (type == Parent)
  {
    file.open("child.txt");
  }
  else
  {
    file.open("parent.txt");
  }
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

void testProzess()
{
  if (fork() == 0)
  {
    wait(NULL);
    //execl(, args, (char *)NULL);
  }
  pid_t pid = getpid();
  pidArray.push_back(pid);
}
void testprozess(string path){
  //const char *path = path.str();
}

int main()
{
  pid_t currpid = getpid();
  pidArray.push_back(currpid);
  testProzess();
  cout<<printinfos();

  return 0;
}
