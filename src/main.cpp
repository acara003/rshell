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

//the man!
#include "sys/mman.h"

//that one class.
#include "Command.h"

using namespace std;
using namespace boost;

//execute command.
void execute(const vector<string> &s, bool &result);

//replaces char in string.
void replace_char(string &s, char o, char r);

//takes a string and returns vector of parsed string.
void parseInput(string s, vector<string> &v);

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

//pass in vector of string and vector of Commands and populates the second vector.
void create_commands(const vector<string> &s, vector<Command> &c);

//perform execution.
void execute_commands(const vector<Command> &v, bool &result);

//result of execution.
static bool *execRes;

int main()
{
    //lets the bool come back from the shadow realm.
    execRes = static_cast<bool *>(mmap(NULL, sizeof *execRes, 
    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0));

    //set it to true because nothing has failed.
    *execRes = true;

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

    //vector to hold parssed string.
    vector<string> parseIn;

    //hold all the Commands.
    vector<Command> comVector;

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
        //cout << "Testing parse" << endl;

        //parse
        parseInput(input,parseIn);
        //display_vector(parseIn);

        //create them commands.
        create_commands(parseIn,comVector);

        //display.
        /*
        for(unsigned int i = 0; i < comVector.size(); ++i)
        {
            comVector.at(i).display();
            cout << endl;
        }
        */
        
        //set to true.
        *execRes = true;
        //cout << "initialize to true: " << *execRes << endl;

        //execute that stuff you know?
        execute_commands(comVector,*execRes);

        //clear vectors.
        parseIn.clear();
        comVector.clear();

        //just in case.
        cin.clear();
        input.clear();
    }

    cout << "End of program" << endl;
   
    return 0;
}

void execute(const vector<string> &s, bool &result)
{
    //check to see if user wants to quit and its the only command.
    if(s.size() == 1)
        for(unsigned int i = 0; i < s.size(); ++i)
            if(s.at(i) == "exit")
                exit(0);
    
    //c-string to hold command.
    char* args[128];

    //place, remove comments and convert commands.
    for(unsigned int i = 0; i < s.size(); ++i)
    {
        //string temp = remove_char(s.at(i),'\"');

        //does not remove comments but works.
        args[i] = (char*)s.at(i).c_str();
        
        //removes comments but does not work.
        //args[i] = (char*)temp.c_str();
    }
    args[s.size()] = '\0';

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
            
            //failed so set to false;
            result = false;

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

void parseInput(string s, vector<string> &v)
{

    //replace spaces.
    replace_char(s,' ','*');

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
            v.push_back(temp_string);
        }

    //return.
    return; 
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

//uselss.
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

bool test(vector<string> &commands, vector<string> &command_list)
{	
	//defaults to "-e"
	string flag = "-e";

	struct stat s_thing;

	//put everything into a queue for ease of use
	queue<string> coms;
	for (unsigned int i = 0; i < commands.size(); i++)
	{
		coms.push(commands.at(i));
	}

	//was a bracket used for the test command?
	bool bracketUsed = false;

	//set if a bracket was used
	if(coms.front() == "[")
		bracketUsed = true;
	
	//remove the first part of the command regardless of whether it's "test" or "["
	coms.pop();
	
	if (isTestFlag(coms.front()))
	{
		flag = coms.front();
		//now we have the flag for the test
		coms.pop();
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
			return false;
		} else
		{
			return true;
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
			return true;
		}
	}
	//Obviously something went wrong if you got here
	return true;
}

void create_commands(const vector<string> &s, vector<Command> &c)
{
    //temp variable.
    Command xcom;
    
    //make commands and push that into command vector.
    for(unsigned int i = 0; i < s.size(); ++i)
    {
        //string to compare.
        string m = s.at(i);
        if(m == ";" || m == "&" || m == "|")
        {
            if(m == ";")
            {
                //ends with a semi colon.
                xcom.set_op(1);
            }
            else if(m == "&")
            {
                if(i+1 < s.size() && s.at(i+1) == "&")
                    xcom.set_op(2);
                ++i;
            }
            else if(m == "|")
            {
                if(i+1 < s.size() && s.at(i+1) == "|")
                    xcom.set_op(3);
                ++i;
            }

            //add it.
            c.push_back(xcom);
            xcom.clear();
        }
        else
            xcom.push_back(s.at(i));
    }   
    
    //push into command vector if the command has something in it.
    if(xcom.empty() == false)
        c.push_back(xcom);    

    return;
}

void execute_commands(const vector<Command> &v, bool &result)
{
    //nothing to execute.
    if(v.size() == 0)
        return;

    //start the process.
    for(unsigned int i = 0; i < v.size(); ++i)
    {
        if(i == 0)
        {
            execute(v.at(i).get_vector(),result);
        }
        else if(i >= 1)
        {
            if(v.at(i-1).get_op() == 1 && result == true)
                execute(v.at(i).get_vector(),result);
            else if(v.at(i-1).get_op() == 2 && result == true)
                execute(v.at(i).get_vector(),result);
            else if(v.at(i-1).get_op() == 3 && result == false)
                execute(v.at(i).get_vector(),result);
        }
    }

    return;
}


