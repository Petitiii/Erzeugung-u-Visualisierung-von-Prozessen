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

//maps fehlt noch
//interface
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
vector<vector<string>> getStatData(vector<pid_t> prozesse)
{

  vector<vector<string>> stats;
  string procStat;
  for (pid_t pid : prozesse)
  {
    string path = "/proc/" + to_string(pid) + "/stat";
    string procstat = readstat(path); // info von /proc/[pid]/stat

    int i = 0, data = 44;
    vector<string> stat(data);
    stringstream ssin(procStat);
    while (ssin.good() && i < data)
    {

      ssin >> stat[i];
      i++;
    }

    stats.push_back(stat);
  }
  return stats;
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
    cout << "lul" << endl;
    cout << "Ich Existiere" << endl;
    execl("./TestProzess", args, (char *)NULL);
  }
  pid_t pid = getpid();
  pidArray.push_back(pid);
}

int main()
{
  testProzess();
  // Erstellt einen Prozess.
  pid_t currpid = getpid();
  pidArray.push_back(currpid);

  char *argument_list[] = {NULL};

  if (fork() == -1)
  {
    sleep(2);
    std::cout << "Ich Existiere";
    execl("./helloworld", args, (char *)NULL);
    std::cout << "Das darf nicht ausgegeben werden, wenn alles gut lief";
  }

  pid_t pid = getpid();
  pidArray.push_back(pid);
  int size;
  string statm_file = "/proc/" + to_string(pid) + "/statm";
  // Schliesst die datei
  ifstream statm(statm_file);

  getStatData(pidArray);

  return 0;
}
