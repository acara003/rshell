#include <iostream>
#include <cstdlib>

//getlogin() and gethostname().
#include <unistd.h>

//perror.
#include <stdio.h>

//fork,wait and execvp.
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

//execute command.
void execute(string s);

int main()
{

    //grab the login.
    char* login = getlogin();

    //start a flag.
    bool login_check = true;

    if((!login) != 0)
    {
        //check flag.
        login_check = false;
        
        //output error.
        perror("Error: could not retrieve login name");
    }

    //hold c-string for host name.
    char host[150];

    //start another flag.
    bool host_check = true;

    //gets hostname while checking if it actually grabbed it.
    if(gethostname(host,sizeof(host)) != 0)
    {
        //check flag.
        host_check = false;

        //output error.
        perror("Error: could not rerieve host name.");
    }

    if(login_check == false || host_check == false)
        cout << "Unable to display login and/or host information." << endl;
    
    //string to hold user input.
    string input;

    while(true)
    {
        //output login@hostname.
        if(login_check && host_check)
            cout << login << '@' << host << ' ';   

        //bash money.
        cout << "$ ";    

        //geting input as a string.
        getline(cin,input);
        cout << input << endl;

        //break.
        if(input == "exit")
            exit(0);

        //execute command.
        execute(input);

    }
    
    cout << "End of program" << endl;
   
    return 0;
}

void execute(string s)
{
    char* args[2048];
    
    args[0] = (char*)s.c_str();
    args[1] = NULL;

    pid_t pid = fork();

    if(pid == -1)
    {
        //fork didn't work.
        perror("fork");
    }
    if(pid ==0)
    {
        if(execvp(args[0], args) == -1)
        {
            //execute didn't work.
            perror("execvp");
            
            //break out of shadow realm.
            exit(1);
        }

    }
    if(pid > 0)
    {
        //wait for child to die.
        if(wait(0) == -1)
        {
            //didnt wait.
            perror("wait");
        }
    }
   
    return;
}



