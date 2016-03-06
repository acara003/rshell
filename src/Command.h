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

        virtual vector<string> get_vector() const {
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

        void display() const {
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

class Multi : public Command
{
    private:

    public:
        Multi() : Command() {}
        
        Multi(const vector<Command*> &v) : Command(v) {}

        int type() { return 2; }
};

class Test : public Command
{
    private:
    
    public:
        Test() : Command() {}
        
        Test(const vector<Command*> &v) : Command(v) {}

        int type() { return 3; }
};

/*
class Command
{
    protected:
        vector<string> vs;
        int op;
    public:
        Command() : op(-1) {};
    
        Command(const vector<string> &v) : op(-1)
        {
            for(unsigned int i = 0; i < v.size(); ++i)
                vs.push_back(v.at(i));
        }

        Command(const vector<string> &v, int op) : op(op)
        {
            for(unsigned int i = 0; i < v.size(); ++i)
                vs.push_back(v.at(i));
        }

        Command(const Command &c) : op(c.get_op())
        {
            vs = c.get_vector();
        }
    
        ~Command() {}

        //add to vector of strings.
        void push_back(string s)
        {
            vs.push_back(s);
            return;
        }

        //access vs but does not modify.
        string at(int i) const
        {
            unsigned int it = i;
            if(it >= vs.size() || it < 0)
                return "ERROR: out of range";
            return vs.at(i);
        }

        //look up size of vs.
        int size() const
        {
            return vs.size();
        }

        vector<string> get_vector() const
        {
            return vs;
        }

        //display vs and op.
        void display() const
        {
            cout << "OP: " << op << endl;
            
            if(vs.size() != 0)
                for(unsigned int i = 0; i < vs.size(); ++i)
                    cout << i+1 << ": " << vs.at(i) << endl;
            else
                cout << "VS: empty" << endl;
        }

        //change op.
        void set_op(int i)
        {
            if(i < -1 || i == 0 || i > 3)
            {
                cout << "ERROR: invalid operator" << endl;
                return;
            }
            op = i;
            return;    
        }

        //return op but dont modify.
        int get_op() const
        {
            return op;
        }

        //empty vs.
        void clear()
        {
            vs.clear();
            op = -1;
        }

        //see if object is empty.
        bool empty() const
        {
            if(op == -1 && vs.size() == 0)
                return true;
            return false;
        }

        int type()
        {
            return 1;
        }
        
        virtual vector<Command*> grab_vector() const;

};

class Pcommand : public Command
{
    private:
        vector<Command*> vc;
        bool lastResult;
    public:
        Pcommand() : Command(), lastResult(true) {}

        Pcommand(const vector<Command*> &v) : Command(), lastResult(true)
        {
            for(unsigned int i = 0; i < v.size(); ++i)
                vc.push_back(v.at(i));
        }

        Pcommand(const vector<Command*> &v, bool res) : Command(), lastResult(res)
        {
            for(unsigned int i = 0; i < v.size(); ++i)
                vc.push_back(v.at(i));
        }

        Pcommand(const Pcommand &p) : Command(p), lastResult(p.get_result()) {}
        
        ~Pcommand() {}
        
        bool get_result() const
        {
            return lastResult;
        }
        
        void set_result(bool res)
        {
            lastResult = res;
            return;
        }

        void push_back(Command com)
        {
            vc.push_back(new Command(com));
            return;
        }
        
        Command at(int pos) const
        {
            unsigned int i = pos;
            if(i >= vc.size())
            {
                cout << "ERROR: out of range" << endl;
                return *vc.at(i);
            }
        }

        bool is_empty()
        {
            if(vc.size() == 0)
                return true;
            return false;
        }

        vector<Command*> grab_vector() const
        {
            return vc;
        }
        
        int type()
        {
            return 2;
        }

};  
*/


#endif
