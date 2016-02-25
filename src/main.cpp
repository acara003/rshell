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

    }
    
    cout << "End of program" << endl;
   
    return 0;
}
