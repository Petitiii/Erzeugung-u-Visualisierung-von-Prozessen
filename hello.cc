#include <iostream>
#include <unistd.h>
using namespace std;
  
int main()
{
    pid_t c_pid = fork();
  
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (c_pid > 0) {
        //  wait(nullptr);
        cout << "printed from parent process " << getpid() 
             << endl;
             int id = getpid();
             string t = to_string(id);
             char const *n_char = t.c_str();
             system("cat /proc/"<<*n_char<<"/status");
    }
    else {
        
        cout << "printed from child process " << getpid()
             << endl;
    }
    
    
  
    return 0;
}