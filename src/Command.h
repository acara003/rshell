#ifndef Command_h
#define Command_h

using namespace std;

class Command
{
    protected:
        vector<string> s;
        int op;
        bool res;       
        
        vector<Command*> v;

    public:
        Command() : op(-1) {}
        
        ~Command() {}
        
        Command(int op) : op(op) {}
        
        Command(const vector<string> &s) : op(-1) {
            if(s.size() != 0)
                for(unsigned int i = 0; i < s.size(); ++i)
                    this->s.push_back(s.at(i));
        }
        
        Command(const vector<string> &s,int op) : op(op) {
            if(s.size() != 0)
                for(unsigned int i = 0; i < s.size(); ++i)
                    this->s.push_back(s.at(i));
        }

        Command(const Command &c) : op(c.get_op()) {
            s = c.get_vector();
        }

        Command(const vector<Command*> &v) : op(-1) {
            if(v.size() != 0)
                for(unsigned int i = 0; i < v.size(); ++i)
                    this->v.push_back(v.at(i));
        }

        vector<string> get_vector() const {
            return s;
        }

        void push_back(string s) {
            this->s.push_back(s);
            return;
        }

        string at(int i) const {
            unsigned int it = i;
            if(it >= s.size() || it < 0)
                return "";
            return s.at(i);
        }

        int size() const {
            return s.size();
        }

        virtual void display() const {
            cout << "OP: " << op << endl;

            if(s.size() != 0)
                for(unsigned int i = 0; i < s.size(); ++i)
                    cout << i+1 << ": " << s.at(i) << endl;
            else
                cout << "empty string vector" << endl;
        }

        void set_op(int i) {
            if(i < -1 || i == 0 || i > 3) {
                cout << "ERROR: out of range" << endl;
                return;
            }
            op = i;
            return;
        }

        int get_op() const {
            return op;
        }

        void clear() {
            s.clear();
            op = -1;
        }

        bool empty() {
            if(op == -1 && s.size() == 0)
                return true;
            return false;
        }
        
        vector<Command*> grab_vector() const {
            return v;
        }

        void set_res(bool res) {
            this->res = res;
        }

        bool get_res() const {
            return res;
        }

        virtual int type() = 0;

};

//contains just simple commands.
class Normal : public Command
{
    private:
               
    public:
        Normal() : Command() {}
        
        Normal(const Command &c) : Command(c) {}

        Normal(const vector<string> &s) : Command(s) {}

        Normal(int op) : Command(op) {}

        Normal(const vector<string> &s, int op) : Command(s,op) {}    
    
        int type() { return 1; }
};

//contains paranthesis.
class Multi : public Command
{
    private:

    public:
        Multi() : Command() {}
        
        Multi(const vector<Command*> &v) : Command(v) {}

        int type() { return 2; }

       void display() const {
            cout << "Command vector display" << endl;
            
            if(v.size() != 0)
                for(unsigned int i = 0; i < v.size(); ++i)
                    v.at(i)->display();
            else
                cout << "empty command vector" << endl;
        }
};

//contains class command.
class Test : public Command
{
    private:
    
    public:
        Test() : Command() {}
        
        Test(const vector<Command*> &v) : Command(v) {}
        
        Test(const vector<string> &s, int op) : Command(s,op) {}

        int type() { return 3; }
};

#endif
