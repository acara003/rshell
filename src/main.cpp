#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <queue>
#include <sys/stat.h>

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

//removes char in string.
string remove_char(const string &s, char c);

//checks if string passed in contains a flag
bool isFlag(string f);

//creates command types from vector of strings.
vector<Command> create_commands(const vector<string> &v);

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

        //testing parse.
        cout << "Testing parse" << endl;
        display_vector(parseInput(input));

        //execute command.
        execute(parseInput(input));

    }

    cout << "End of program" << endl;
   
    return 0;
}

void execute(const vector<string> &s)
{
    //check to see if user wants to quit and its the only command.
    if(s.size() == 1)
        for(unsigned int i = 0; i < s.size(); ++i)
            if(s.at(i) == "exit")
                exit(0);
    
    //c-string to hold command.
    char* args[128];

    int count = 0;

    cout << endl;
    //place, remove comments and convert commands.
    for(unsigned int i = 0; i < s.size(); ++i)
    {
       // string temp = remove_char(s.at(i),'\"');

        //cout << "(char*)temp.c_str(): " << (char*)temp.c_str() << endl;
        cout << "s.at(i): " << s.at(i) << endl;    

        args[i] = (char*)s.at(i).c_str();
        //args[i] = (char*)temp.c_str();
        count++;

        //cout << "temp after: " << temp << endl;
        cout << "args at " << i << ": " << args[i] << endl << endl;
    }
    cout << "array before: " << endl;
    for(int i = 0; i < count; ++i)
        cout << args[i] << endl;
    cout << endl;

    args[s.size()] = '\0';

    cout << "array after: " << endl;
    for(int i = 0; i < count; ++i)
        cout << args[i] << endl;
    cout << endl;

    //creates fork process.
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
    replace_char(s,' ','*');

    //make temp vector to hold parsed strings.
    vector<string> temp;

    //create boost magic function.
    char_separator<char> sep(" ;||&&(){}", ";||&&()[]",keep_empty_tokens);

    //create boost magic holder thingy.
    tokenizer< char_separator<char> > cm(s,sep);

    //for each loop to grab each peice and push it into a vector.
    for(tokenizer< char_separator<char> >::iterator it = cm.begin(); it != cm.end(); ++it)
        if(*it != "")
        {
            //fix string.
            string temp_string = *it;
            replace_char(temp_string,'*',' ');
            temp.push_back(temp_string);
        }

    //return that vector.
    return temp;
}

void replace_char(string &s, char o, char r)
{
    //different use for the function.
    if(o == '\"')
    {
        //nothing to replace.
        if(s.find(o) == string::npos)
            return;
        
        //replace.
        for(unsigned int i = 0; i < s.size(); ++i)
            if(s.at(i) == o)
                s.at(i) = r;

        return;
    }

    //no quotes.
    if(s.find("\"") == string::npos)
        return;
    else if(s.find(o) == string::npos)
        return;

    //vector to hold quote positions.
    vector<int> pos;

    //place positions of char into vector.
    for(unsigned int i = 0; i < s.size(); ++i)
        if(s.at(i) == '\"')
            pos.push_back(i);   
    
    //count position.
    unsigned int count = 0;
   
    //replace.
    while(count < pos.size()) 
    {
        for(int i = pos.at(count); i < pos.at(count+1); ++i)
            if(s.at(i) == o)
                s.at(i) = r;
    
        count++;
        count++;
    } 

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
    if(find_char_amount(s,'\"') % 2 != 0)
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

    //if no comments then deletes everything from hash forward.
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

    //no comments or hash for some reason.
    if(hashPos.size() == 0 || quotePos.size() == 0)
        return;
    
    //just in case.
    if(quotePos.size() % 2 != 0)
        quotePos.push_back(0);

    //overall check;
    vector<bool> check;
        
    //start it up.
    for(unsigned int i = 0; i < hashPos.size(); ++i)
        check.push_back(true);
      
    //check if hash is in quotes.
    for(unsigned int i = 0; i < hashPos.size(); ++i )
        for(unsigned int j = 0; j < quotePos.size(); j+=2 )
        {
            if(hashPos.at(i) > quotePos.at(j) && hashPos.at(i) < quotePos.at(j+1))
            {
                check.at(i) = true;
                break;
            }
            else
                check.at(i) = false;
        }

    //check bool vector to delete string.
    for(unsigned int i = 0; i < check.size(); ++i)
        if(!check.at(i))
        {
            s = s.substr(0,hashPos.at(i));
            return;
        }

    //if comment at end then kill it.
    if(hashPos.at(hashPos.size()-1) > quotePos.at(quotePos.size()-1))
        s = s.substr(0,hashPos.at(hashPos.size()-1));   
    
    return;
}

int find_char_amount(const string s, char c)
{
    //nothing there.
    if(s.find(c) == string::npos)
        return 0;

    //start counting.
    int count = 0;
    for(unsigned int i = 0; i < s.size(); ++i)
        if(s.at(i) == c)
            count++;
    return count;

}

string remove_char(const string &s, char c)
{
    //if not there then just return.
    if(s.find(c) == string::npos)
       return s;
    
    //start empty.
    string t = "";

    //add everything thats not what we dont want.
    for(unsigned int i = 0; i < s.size(); ++i)
        if(s.at(i) != c)
            t += s.at(i);

    return t;
}

int is_connector(string s)
{
    if(s == ";")
		return 1;
	if(s == "&&")
		return 2;
	if(s == "||")
		return 3;
									        
	return -1;
}

bool isFlag(string f)
{
	//default list of possible flags
	string flags = "-e -d -f";

	//see if string is one of the flags
	if (flags.find(f) != string::npos)
		return true;
	return false;
}

/*
bool test(vector<string> &commands, vector<strings> &command_list)
{	
	//defaults to "-e"
	string flag = "-e";

	struct stat s_thing;

	//put everything into a queue for ease of use
	queue<string> coms;
	for (unsigned int i = 0; i < commands.size(); i++)
	{
		coms.push_back(commands.at(i));
	}

	//was a bracket used for the test command?
	bool bracketUsed = false;

	//set if a bracket was used
	if(coms.front() == "[";
		bracketUsed = true;
	
	//remove the first part of the command regardless of whether it's "test" or "["
	coms.pop();
	
	if (isTestFlag(coms.front()))
	{
		flag = coms.front();
		//now we have the flag for the test
		coms.pop()
	}
	
	//if there's another flag attempt then it's broken
	if (coms.front().at(0) == "-")
	{
		cout << "ERROR: incorrect flags" << endl;
		
		//keep deleting from queue till the next command
		while (!is_connector(coms.front()))
		{
			coms.pop();
		}
		return true;
	}

	// if the first part of the path is a "/" remove it (that way it can't mess up)
	if(coms.front().at(0) == "/")
		coms.front().substr(1, coms.front().size() - 1);
	
	//make a new c_string to hold the file path
	char *filePath = new char[coms.front.size()];
	
	//copy it on over from the command vector
	strcpy(filePath, coms.front().c_str());
	command_list.push_back(filePath);

	//moving along
	coms.pop();
	
	//Did we use a bracket instead of "test"? If so get rid of the last bracket
	if(bracketUsed)
	{
		coms.pop();
	}
	
	//valuse for using the stat thingy
	int current_stat;
	
	//get the status of the command
	current_stat = stat(command_list.front(), &s_thing);
	
	//Did it fail?
	if(current_stat < 0)
	{	
		//Yup it did
		perror("ERROR: Coudn't get the stat");
		return true;
	}
	//No it didn't so lets try out with "-e"
	if (flag == "-e")
	{
		return false;
	}
	
	//Try it out with "-f"
	if(flag == "-f")
	{
		if(S_ISREG(s_thing.st_mode))
		{
			return false
		} else
		{
			return true
		}
	}

	//Try it out with "-d"
	if (flag == "-d")
	{
		if (S_ISDIR(s_thing.st_mode))
		{
			return false;
		} else
		{
			return true
		}
	}
	//Obviously something went wrong if you got here
	return true
}*/

vector<Command> create_commands(const vector<string> &v)
{
    vector<Command> commandVector;
    Command temp;    
    
    

    return commandVector;
}

