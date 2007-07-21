#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
using namespace std;

//declaracoes
void execute();
string pattern(string::iterator eof, string::iterator &dna, string rna, bool &done);
string templat(string::iterator eof, string::iterator &dna, string rna, bool &done);
void matchreplace(string strDna, string::iterator eof, string::iterator &dna, 
                    string p, string t, bool &done);

int nat(string::iterator eof, string::iterator &dna);
void consts(string::iterator eof, string::iterator &dna, string p);
int doswitch(string::iterator dna);


void replace (string::iterator &dna, string tpl, vector<string> e);
string protect (int count, string str);
string quote (string str);
string asnat (int len);


const int C = 1;
const int F = 2;
const int P = 3;
const int IC = 4;
const int IP = 5;
const int IF = 6;
const int IIP = 7;
const int IIC = 8;
const int IIF = 9;
const int III = 10;


int doswitch(string::iterator dna)
{
    if (*dna == 'C')
        return C;
    else if (*dna == 'F')
        return F;
    else if (*dna == 'P')
        return P;
    else if ((*dna == 'I') && (*(dna+1) == 'C'))
        return IC;
    else if ((*dna == 'I') && (*(dna+1) == 'P'))
        return IP;
    else if ((*dna == 'I') && (*(dna+1) == 'F'))
        return IF;
    else if ((*dna == 'I') && (*(dna+1) == 'I') && (*(dna+2) == 'P'))
        return IIP;
    else if ((*dna == 'I') && (*(dna+1) == 'I') && (*(dna+2) == 'C'))
        return IIC;
    else if ((*dna == 'I') && (*(dna+1) == 'I') && (*(dna+2) == 'F'))
        return IIF;
    else if ((*dna == 'I') && (*(dna+1) == 'I') && (*(dna+2) == 'I'))
        return III;
    return 0;
}

string pattern(string::iterator eof, string::iterator &dna, string rna, bool &done)
{
    string p;
    int lvl = 0;

    while (dna != eof)
    {
        int ret = doswitch(dna);
        switch (ret)
        {
            case C: 
            {
                dna++;
                p += 'I';
                break;
            }
            case F:
            {
                dna++;
                p += 'C';
                break;
            }
            case P:
            {
                dna++;
                p += 'F';
                break;
            }
            case IC:
            {
                dna += 2;
                p += 'P';
                break;
            }
            case IP:
            {
                dna += 2;
                p += '!';
                int n = nat(eof, dna);
                if (n < 0)
                {
                    done = true;
                    return p;
                }
                // convert int to string
                stringstream ss;
                ss << n;
                p += ss.str(); // get the string and append                
                break;
            }
            case IF:
            {
                dna += 3;
                p += '?';
                consts(eof, dna, p);
                p += '?';
                break;
            }
            case IIP:
            {
                dna += 3;
                lvl++;
                p += '(';
                break;
            }
            case IIC:
            case IIF:
            {
                dna += 3;
                if(lvl == 0)
                {
                    return p;
                }
                else
                {
                    lvl--;
                    p += ')';
                }
                break;
            }
            case III:
            {
                dna += 3;
                rna.append (dna, dna+7);
                dna += 7;
                break;
            }
            default:
                done = true;
                return p;
                break;
        }
        
        cout << p;
    }
}


int nat(string::iterator eof, string::iterator &dna)
{
    if (dna == eof)
        return -1;
        
    if(*dna == 'P')
    {
        dna++;
        return 0;
    }
    else if( (*dna == 'I') || (*dna == 'F') )
    {
        dna++;
        int n = nat(eof, dna);
        return 2*n;
    }
    else if (*dna == 'C')
    {
        dna++;
        int n = nat(eof, dna);
        return 2*n+1;
    }
    else
    {
        return -1;
    }
}

void consts(string::iterator eof, string::iterator &dna, string p)
{
    while (dna == eof)
    {
        int ret = doswitch(dna);
        switch (ret)
        {
            case C:
            {
                p += 'I';
                dna++;
                break;
            }
            case F:
            {
                p += 'C';
                dna++;
                break;
            }
            case P:
            {
                p += 'F';
                dna++;
                break;
            }
            case IC:
            {
                p += 'P';
                dna += 2;
                break;
            }
            default:
                return;
            break;
        }
    }
}

string templat(string::iterator eof, string::iterator &dna, string rna, bool &done)
{
    string t;
    
    int n, l;
    while (dna != eof)
    {
        int ret = doswitch(dna);
        switch (ret)
        {
            case C:
			{
                dna++;
                t += 'I';
                break;
			}
            case F:
			{
                dna++;
                t += 'C';
                break;
			}
            case P:
			{
                dna++;
                t += 'F';
                break;
			}
            case IC:
			{
                dna += 2;
                t += 'P';
                break;
			}
            case IF:
            case IP:
			{
                dna += 2;
                l = nat(eof, dna);
                if (l < 0)
                {
                    done = true;
                    return t;
                }               
                n = nat(eof, dna);
                if (n < 0)
                {
                    done = true;
                    return t;
                }   
				stringstream ss;
                ss << n;
                t += ss.str();
                t += '?';
				stringstream ss1;
                ss1 << l;
                t += ss1.str();
                t += '?';
                break;
			}
            case IIC:
            case IIF:
			{
                dna += 3;
                return t;
                break;
			}
            case IIP:
			{
                dna += 3;
                n = nat(eof, dna);
                if (n < 0)
                {
                    done = true;
                    return t;
                }               
				stringstream ss;
                ss << n;
                t += '|';
                t += ss.str();
                t += '|';
                break;
			}
            case III:
			{
                dna += 3;
                rna.append (dna, dna+7);
                dna += 7;
                break;
			}
            
            default:
			{
                done = true;
                return t;
                break;
			}
        }        
    }    
}


bool isMatchOp (string::iterator p)
{
    return (*p == 'I' || 
            *p == 'C' || 
            *p == 'F' || 
            *p == 'P' ||
            *p == '!' ||
            *p == '?' ||
            *p == '(' ||
            *p == ')');
}

bool isReplaceOp (string::iterator p)
{
    return (*p == 'I' || 
            *p == 'C' || 
            *p == 'F' || 
            *p == 'P' ||
            isdigit(*p) ||
            *p == '|');
}

void matchreplace(string strDna, string::iterator eof, string::iterator &dna, 
                    string pat, string t, 
                    bool &done)
{
    int i = 0;
    int n;
    list <int> c;
    vector<string> e;
    int currentDnaPos = distance (strDna.begin(), dna);
    
    for (string::iterator p = pat.begin(); p != pat.end(); p++)
    {
        if (*p == 'I' || 
            *p == 'C' || 
            *p == 'F' || 
            *p == 'P')
        {
            if (*(dna + i) == *p)
                i++;
            else
                return;
        }
        else if (*p == '!')
        {
            p++;
            string str;
            while (!isMatchOp(p))
            {
                str += *p;
                p++;
            }
            istringstream iss(str);
            iss >> n;
            i += n;

            if (i > distance (p, eof))
                return;
        }
        else if (*p == '?')
        {
            p++;
            string str;
            while (*p != '?')
            {
                str += *p;
                p++;
            }
            string::size_type pos = strDna.find (str, currentDnaPos + i);
            if (pos != string::npos)
                i = pos;
            else
                return;
            
        }
        else if (*p == '(')
        {
            c.push_front (i);
        }
        else if (*p == ')')
        {
            
            int open = c.front();
            c.pop_front ();
            e.push_back (strDna.substr (currentDnaPos + open, currentDnaPos + i));
        }
    }
    
	dna += i;
    replace (dna, t, e);
}

void replace (string::iterator &dna, string tpl, vector<string> e)
{
    string r;
    for (string::iterator t = tpl.begin(); t != tpl.end(); t++)
    {
        if (*t == 'I' ||
            *t == 'C' ||
            *t == 'F' ||
            *t == 'P')
        {
            r += *t;
        }
        else if (isdigit(*t))
        {
            string str;
            while (*t != '?')
            {
                str += *t;
                t++;
            }

            
            istringstream iss(str);
            int n = 0;
            iss >> n;

            t++;
            str.clear();
            while (*t != '?')
            {
                str += *t;
                t++;
            }

            istringstream iss2(str);
            int count = 0;
            iss2 >> count;
			if (e.size () > n)
				r += protect (count, e[n]);
        }
        else if (*t == '|')
        {
            t++;
            string str;
            while (*t != '|')
            {
                str += *t;
                t++;
            }

            istringstream iss(str);
            int len = 0;
            iss >> len;
			if (e.size () > len)
	            r += asnat(e[len].length());
        }
    }    
}


string protect (int count, string str)
{
    if (count == 0)
        return str;
    
    return protect (count - 1, quote(str));
}

string quote (string str)
{
    string r;
    for (string::iterator t = str.begin(); t != str.end(); t++)
    {
        if      (*t == 'I')
            r += "C";
        else if (*t == 'C')
            r += "F";
        else if (*t == 'F')
            r += "P";
        else if (*t == 'P')
            r += "IC";
    }
    return str;
}

string asnat (int len)
{
    if (len == 0)
        return string("P");
    if (len % 2 == 0)
        return string("I") + asnat(len/2);
    return string("C") + asnat(len/2);
}


void execute ()
{
    ifstream fin("endo.dna");
    string dna;
    fin >> dna;
    fin.close();

    string rna;
    
    string::iterator dnaCurrent = dna.begin();
    string::iterator dnaEnd = dna.end();
    bool done = false;
    while (dnaCurrent != dnaEnd)
    {
        cout << "start pattern" << endl;
        string p = pattern(dnaEnd, dnaCurrent, rna, done);
        cout << "end pattern" << endl;
        if (done)
            break;

        cout << "start templat" << endl;
        string t = templat (dnaEnd, dnaCurrent, rna, done);
        cout << "end templat" << endl;

        cout << "start matchreplace" << endl;
        matchreplace (dna, dnaEnd, dnaCurrent, p, t, done);
        cout << "end matchreplace" << endl;
    }

    ofstream fout("endo.rna");
    fout << dna;
    fout.close();
}


int main(void)
{
    execute();

    return 0;
}
