#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

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

//replaces char in string.
void replace_char(string &s, char o, char r);

//takes a string and returns vector of parsed string.
vector<string> parseInput(string s);

//display vector.
template<typename unit>
void display_vector(vector<unit> v);

//helper function that finds amount of character.
int find_char_amount(const string s,char c);

//removes comment from input.
void remove_comment(string &s);

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

        //remove extra white space.
        trim(input);

        //remove comments.
        remove_comment(input);
        
        //trim again just in case.
        trim(input);

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

    //replace spaces.
    replace_char(s,' ','_');

    //make temp vector to hold parsed strings.
    vector<string> temp;

    //create boost magic function.
    //char_separator<char> sep(" ;||&(){}\"", ";||&()[]\"", keep_empty_tokens);

    //test copy
    char_separator<char> sep(" ;||&&(){}", ";||&&()[]",keep_empty_tokens);

    //create boost magic holder thingy.
    tokenizer< char_separator<char> > cm(s,sep);

    //for each loop to grab each peice and push it into a vector.
    for(tokenizer< char_separator<char> >::iterator it = cm.begin(); it != cm.end(); ++it)
        if(*it != "")
            temp.push_back(*it);

    //return that vector.
    return temp;
}

void replace_char(string &s, char o, char r)
{
    if(s.find("\"") != string::npos)
        for(unsigned int i = s.find("\""); i < s.find("\"",s.find("\"")+1);++i)
            if(s.at(i) == o)
                s.at(i) = r;    
    return;
}

template<typename unit>
void display_vector(vector<unit> v)
{
    for(unsigned int i = 0; i < v.size(); ++i)
        cout << i+1 << ": " << v.at(i) << endl;
    return;
}

void remove_comment(string &s)
{
    //just add a " to the end to avoid errors.
    if(find_char_amount(s,'\"') %2 != 0)
        s += '\"';

    //delete everything!
    if(s.find("#") == 0)
    {
        s = "";
        return;
    }

    //return if no comments.
    if(s.find("#") == string::npos)
    {
        return;
    }

    //if no comments then delets everything from hash forward.
    if(s.find("\"") == string::npos && s.find("#") != string::npos)
    {
        s = s.substr(0,s.find("#"));
        return;
    }

    //if comment before quote then delete.
    if(s.find("\"") > s.find("#"))
    {
        s = s.substr(0,s.find("#"));
        return;
    }

    //advanced situations.
    //get a vector to hold positions of quotes and hash.
    vector<int> quotePos;
    vector<int> hashPos;

    //grab pos.
    for(unsigned int i = 0; i < s.size(); ++i)
    {
        if(s.at(i) == '\"')
            quotePos.push_back(i);
        else if(s.at(i) == '#')
            hashPos.push_back(i);
    }

    //hold pos for hash.
    int i = 0;
        
    //checks to see if in middle.
    bool check = true;
    
    //pos of lone hash.
    int pos = 0;

    for(unsigned int j = 0; j < quotePos.size() - 1; j += 2)
    {
        if(hashPos.at(i) > quotePos.at(j) && hashPos.at(i) < quotePos.at(j+1))
        {
            check = true;
            ++i;
        }
        else
        {
            check = false;
            pos = hashPos.at(i);
        }            
    }

    if(!check)
    {
        s = s.substr(0,pos);
        return;            
    } 

    if(hashPos.at(hashPos.size()-1) > quotePos.at(quotePos.size()-1))
        s = s.substr(0,hashPos.at(hashPos.size()-1));   
    
    return;
}

int find_char_amount(const string s, char c)
{
    if(s.find(c) == string::npos)
        return 0;

    int count = 0;
    for(unsigned int i = 0; i < s.size(); ++i)
        if(s.at(i) == c)
            count++;
    return count;

}
