#ifndef Command_h
#define Command_h

using namespace std;

class Command
{
    private:
        vector<string> vs;
        int op;
    public:
        Command() : op(-1) {};
    
        Command(const vector<string> &v)
            : op(-1)
        {
            for(unsigned int i = 0; i < v.size(); ++i)
                vs.push_back(v.at(i));
        }

        Command(const vector<string> &v, int op)
            : op(op)
        {
            for(unsigned int i = 0; i < v.size(); ++i)
                vs.push_back(v.at(i));
        }

        //add to vector of strings.
        void push_back(string s)
        {
            vs.push_back(s);
            return;
        }

        //access vs but does not modify.
        string at(int i)
        {
            unsigned int it = i;
            if(it >= vs.size() || it < 0)
                return "ERROR: out of range";
            return vs.at(i);
        }

        //look up size of vs.
        int size()
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

};

#endif
