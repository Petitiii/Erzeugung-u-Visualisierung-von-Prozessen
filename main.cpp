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

using namespace std;
ofstream myFile_Handler;
const char *args = NULL;
vector<pid_t> pidVector;

void testProzess() // Erstellt einen neuen Prozess durch aufrufen einer cpp datei, diese gibt eime Konsolenausgabe aus
{
  pid_t neuPr = fork();
  if (neuPr == 0)
  {
    // execl öffnet die datei
    execl("./TestProzess", args, (char *)NULL);
  }
  // Fügt die PID in ein Vector hinzu
  pidVector.push_back(neuPr);
}
void neuesTerminal()
{
  pid_t neuPr = fork();
  if (neuPr == 0)
  {
    execl("gnome-Terminal", args, (char *)NULL);
  }

  pidVector.push_back(neuPr);
}
// Methode zum auslesen der /proc/[PID]/stat
string readstat(string procpath)
{
  string status_file = procpath;
  ifstream status(status_file);
  string line;
  getline(status, line);
  return line;
}
vector<vector<string>> procStat()
{
  // zwei dimensionaler Vector zum Abspeichern der PID und den zugehörigen Informationen
  vector<vector<string>> stats;
  for (pid_t pid : pidVector)
  {
    string path = "/proc/" + to_string(pid) + "/stat";
    string procstat = readstat(path); // info von /proc/[pid]/stat
    // 44 da 44 werte zurück gegeben werden.
    int i = 0, data = 44;
    vector<string> stat(data);
    //
    stringstream ssin(procstat);
    while (ssin.good() && i < data)
    {
      // fügt die Werte in den Vector
      ssin >> stat[i];
      i++;
    }
    // fügt den Vector in den hauptvector ein, dieser wird dann returned
    stats.push_back(stat);
  }
  return stats;
}
// info von /proc/[pid]/maps , funkroiniert gleich wie procStat
vector<vector<string>> procMaps()
{

  vector<vector<string>> maps;
  for (pid_t pid : pidVector)
  {
    string path = "/proc/" + to_string(pid) + "/maps";
    string mapdata = readstat(path);

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
// Gibt alle Informationen in der Konsole aus, und returned einen String um diesen dann später in einer Log datei abspeichern zu können
string printinfos()
{
  // Ruft die dateien procStat und Procmaps aus und speichert diese in einem 2 Dimensionalen Array
  vector<vector<string>> stat = procStat();
  vector<vector<string>> map = procMaps();

  // Konsolen anzeige
  string alleInfos = "Prozess Info\t\t\t\t\t\tSpeicher         \nPID\tName\t\tPPID\tUID\tGID\tRechte\tSize\tRAM-Ausnutzung\n";
  // Vectoren für die Programm größe und den Ram verbrauch
  vector<string> size;
  vector<string> ram;

  for (pid_t p : pidVector)
  {
    string statmvalues;
    string ramusage;
    string statm_file = "/proc/" + to_string(p) + "/statm";

    ifstream statm(statm_file);
    statm >> statmvalues >> ramusage;
    size.push_back(statmvalues);
    ram.push_back(ramusage);
  }
  // Schleife zum durchlaufen aller Vectoren und Ausgabe der Informationen

  for (size_t i = 0; i < stat.size(); i++)
  {

    stat[i][1].length() < 8 ? stat[i][1] += "\t" : "";

    alleInfos += stat[i][0] + "\t" + stat[i][1] + "\t" + to_string(getpid()) + "\t" + to_string(getuid()) + "\t" + stat[i][5] + "\t" + map[i][1] + "\t" + size[i] + "\t" + ram[i] + "\n";
  }
  // returned den string um später abgespeichert oder ausgegeben zu werden
  return alleInfos;
}

// Schreibt in die Log File, nimmt als Input einen String
void writeLog(string info)
{
  ofstream file;
  time_t now = time(0);
  // Datum
  char *dt = ctime(&now);

  file.open("log.txt", fstream::app);
  file << dt << endl;
  file << info << endl;
}
// Ließt die Logfile
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
  // Schließt alle Prozesse die in dem Vector abgespeichert sind, damit der Speicher frei gegeben wird.
  void killProzess()
  {
    for (pid_t eintraege : pidVector)
    {
      kill(eintraege, SIGTERM);
    }
  }
  // Das Menü was in der Konsole angezeigt wird um unterschiedliche Funktionen auszuwählen
  void auswahlmenu()
  {
    string info = "1: Neuen Prozess erzeugen \n2: Prozessinformationen ausgeben \n3: LogFile ausgeben \n4: Alle Prozesse beenden";
    cout << info << endl;
    int auswahl;
    // Konsolen input
    cin >> auswahl;
    switch (auswahl)
    {
    case 1:
      testProzess();
      sleep(2);
      auswahlmenu();
    case 2:
      cout << printinfos();
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
      cout << "Falsche Taste" << endl;
      break;
    }
  }

  int main()
  {
    pidVector.push_back(getpid()); // das ist damit die Prozess id von dem standart prozess anfangs gespeichert wird
    cout << "Hallo in unserem wunderbaren Interface :D \nDruecken sie die angezeigten Zahlen um unterschiedliche Aktionen auszufuehren" << endl;
    auswahlmenu();// ruft das menü auf um den User funktonen auswählen zu lassen

    return 0;
  }
