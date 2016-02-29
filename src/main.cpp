#include <iostream>
#include <cstdlib>
#include <vector>

//getlogin() and gethostname().
#include <unistd.h>

//perror.
#include <stdio.h>

//fork,wait and execvp.
#include <sys/types.h>
#include <sys/wait.h>

//boost!
#include "boost/algorithm/string.hpp"
#include "boost/tokenizer.hpp"
#include "boost/foreach.hpp"

//that one class.
#include "Command.h"

using namespace std;
using namespace boost;

//execute command.
void execute(const vector<string> & s);

//takes a string and returns vector of parsed string.
vector<string> parseInput(string s);

//display vector.
void display_vector(vector<string> v);

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

    //warn user of upcoming trouble.
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

        //placeholder to tell its the program.
        cout << " (program) ";

        //geting input as a string.
        getline(cin,input);
        cout << input << endl;

        //break.
        if(input == "exit")
            exit(0);
        
        //testing parse.
        cout << "Testing parse" << endl;
        display_vector(parseInput(input));

        //execute command.
        //execute(input);

    }
    
    cout << "End of program" << endl;
   
    return 0;
}

void execute(const vector<string> &s)
{
    //c-string to hold command.
    char* args[2048];
    
    //place and convert commands.
   
    for(unsigned int i = 0; i < s.size(); ++i)
        args[i] = (char*)s.at(i).c_str();
    args[s.size()] = NULL;

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

vector<string> parseInput(string s)
{
    //make temp vector to hold parsed strings.
    vector<string> temp;

    //create boost magic function.
    char_separator<char> sep(" ;||&(){}\"", ";||&()[]\"", keep_empty_tokens);

    //create boost magic holder thingy.
    tokenizer< char_separator<char> > cm(s,sep);

    //for each loop to grab each peice and push it into a vector.
    for(tokenizer< char_separator<char> >::iterator it = cm.begin(); it != cm.end(); ++it)
        if(*it != "")
            temp.push_back(*it);

    //return that vector.
    return temp;
}

void display_vector(vector<string> v)
{
    for(unsigned int i = 0; i < v.size(); ++i)
        cout << i+1 << ": " << v.at(i) << endl;
    return;
}



