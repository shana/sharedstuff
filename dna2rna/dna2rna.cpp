#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <stack>
#include <vector>
using namespace std;

//declaracoes
void execute(string dna);
string pattern(string strDna, string::iterator eof, string::iterator &dna, int &dnaPos, string &rna, bool &done);
string templat(string::iterator eof, string::iterator &dna, int &dnaPos, string &rna, bool &done);
void matchreplace(string &strDna, string::iterator &eof, string::iterator &dna, int &dnaPos, 
                    string &p, string &t, bool &done);

int nat(string::iterator eof, string::iterator &dna, int &dnaPos);
void consts(string::iterator eof, string::iterator &dna, int &dnaPos, string &p);
int doswitch(string::iterator dna);


string replace (string::iterator &dna, string &tpl, vector<string> e);
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


ofstream logfile;
int rnacommands;



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

string pattern(string strDna, string::iterator eof, string::iterator &dna, int &dnaPos, string &rna, bool &done)
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
				dnaPos++;
                p += 'I';
                break;
            }
            case F:
            {
                dna++;
				dnaPos++;
                p += 'C';
                break;
            }
            case P:
            {
                dna++;
				dnaPos++;
                p += 'F';
                break;
            }
            case IC:
            {
                dna += 2;
				dnaPos += 2;
                p += 'P';
                break;
            }
            case IP:
            {
                dna += 2;
				dnaPos += 2;
                p += '!';
                int n = nat(eof, dna, dnaPos);
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
				dnaPos += 3;
                p += '?';
                consts(eof, dna, dnaPos, p);
                p += '?';
                break;
            }
            case IIP:
            {
                dna += 3;
				dnaPos += 3;
                lvl++;
                p += '(';
                break;
            }
            case IIC:
            case IIF:
            {
                dna += 3;
				dnaPos += 3;
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
				dnaPos += 3;
				rna += strDna.substr (dnaPos, 7);
                dna += 7;
				dnaPos += 7;
				rnacommands++;
                break;
            }
            default:
                done = true;
                return p;
                break;
        }
        
    }
    return p;
}


int nat(string::iterator eof, string::iterator &dna, int &dnaPos)
{
    if (dna == eof)
        return -1;
        
    if(*dna == 'P')
    {
        dna++;
		dnaPos++;
        return 0;
    }
    else if( (*dna == 'I') || (*dna == 'F') )
    {
        dna++;
		dnaPos++;
        int n = nat(eof, dna, dnaPos);
        return 2*n;
    }
    else if (*dna == 'C')
    {
        dna++;
        dnaPos++;
		int n = nat(eof, dna, dnaPos);
        return 2*n+1;
    }
    else
    {
        return -1;
    }
}

void consts(string::iterator eof, string::iterator &dna, int &dnaPos, string &p)
{
    while (dna != eof)
    {
        int ret = doswitch(dna);
        switch (ret)
        {
            case C:
            {
				p.append ("I");
                dna++;
				dnaPos++;
                break;
            }
            case F:
            {
				p.append ("C");
                dna++;
				dnaPos++;
                break;
            }
            case P:
            {
				p.append ("F");
                dna++;
				dnaPos++;
                break;
            }
            case IC:
            {
				p.append ("P");
                dna += 2;
				dnaPos += 2;
                break;
            }
            default:
                return;
            break;
        }
    }
}

string templat(string::iterator eof, string::iterator &dna, int &dnaPos, string &rna, bool &done)
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
				dnaPos++;
                t += 'I';
                break;
			}
            case F:
			{
                dna++;
				dnaPos++;
                t += 'C';
                break;
			}
            case P:
			{
                dna++;
				dnaPos++;
                t += 'F';
                break;
			}
            case IC:
			{
                dna += 2;
				dnaPos += 2;
                t += 'P';
                break;
			}
            case IF:
            case IP:
			{
                dna += 2;
				dnaPos += 2;
                l = nat(eof, dna, dnaPos);
                if (l < 0)
                {
                    done = true;
                    return t;
                }               
                n = nat(eof, dna, dnaPos);
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
				dnaPos += 3;
                return t;
                break;
			}
            case IIP:
			{
                dna += 3;
				dnaPos += 3;
                n = nat(eof, dna, dnaPos);
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
				dnaPos += 3;
                rna.append (dna, dna+7);
                dna += 7;
				dnaPos += 7;
				rnacommands++;
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

void matchreplace(string &strDna, string::iterator &eof, string::iterator &dna, int &dnaPos, 
                    string &pat, string &t, 
                    bool &done)
{
    int i = 0;
    int n;
    stack <int> c;
    vector<string> e;
    
	int len = strlen(strDna.c_str());
	strDna = strDna.erase (0, dnaPos);
	strDna.resize (len-dnaPos);
	len = strlen(strDna.c_str());
	dna = strDna.begin ();
	eof = strDna.end();
	dnaPos = 0;

	for (string::iterator p = pat.begin(); p < pat.end(); p++)
    {
        if (*p == 'I' || 
            *p == 'C' || 
            *p == 'F' || 
            *p == 'P')
        {
			char c = *(dna + i);
			char c2 = *p;
            if (c == c2)
                i++;
            else
			{
                return;
			}
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

            if (p + i >= eof)
			{
                return;
			}
			p--;
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
			
            string::size_type pos = strDna.find (str, i);
            if (pos != string::npos)
				i = pos + str.length ();
            else
			{
                return;
			}
        }
        else if (*p == '(')
        {
            c.push (i);
        }
        else if (*p == ')')
        {
            
            int open = c.top();
            c.pop ();
			if (strDna.length () < open || strDna.length () < i)
			{
				cout << "invalid string position" << endl;
				return;
			}
			string str = strDna.substr (open, i - open);
			logfile << str.substr (0, 10) << "... (" << str.length() << ")" << endl;
            e.push_back (str);
        }
    }

	strDna = strDna.erase (0, i);
	strDna.resize (len-i);
	dna = strDna.begin ();
    string r = replace (dna, t, e);
	strDna.insert (0, r);
	dna = strDna.begin ();
	eof = strDna.end();
	dnaPos = 0;
}

string replace (string::iterator &dna, string &tpl, vector<string> e)
{
    string r;
    for (string::iterator t = tpl.begin(); t != tpl.end(); t++)
    {
        if (*t == 'I' ||
            *t == 'C' ||
            *t == 'F' ||
            *t == 'P')
        {
			char c = *t;
            r += c;
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

	return r;
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


void execute (string dna)
{

	string rna;
    
    string::iterator dnaCurrent = dna.begin();
    string::iterator dnaEnd = dna.end();
    bool done = false;
	int dnaPos = 0;
	int iterations = 0;
    while (dnaCurrent != dnaEnd)
    {
		cout << "iteration " << iterations++ << endl;
		logfile << endl;
		logfile << "dna = " << dna.substr(dnaPos, 10) << "... (" << dna.size() << ")" << endl;
        string p = pattern(dna, dnaEnd, dnaCurrent, dnaPos, rna, done);
        if (done)
            break;
		
		logfile << "pattern = " << p << endl;

        string t = templat (dnaEnd, dnaCurrent, dnaPos, rna, done);

		logfile << "template = " << t << endl;

		matchreplace (dna, dnaEnd, dnaCurrent, dnaPos, p, t, done);

		logfile << "len(rna) = " << rna.length() << endl;
		
	}

	cout << "rna commands:" << rnacommands << endl;

    ofstream fout("endo.rna");
    fout << rna;
    fout.close();

	if (dnaCurrent != dnaEnd)
	{
		ofstream fout2("rest.dna");
		fout2 << dna.substr (distance(dna.begin (), dnaCurrent));
		fout2.close();
	}
	//string str;
	//cin >> str;
}


int main(int argc, char **argv)
{
	string prefix;
	string dna;

	string prefixFile;
	string endoFile;
	prefixFile = "prefix.dna";
	endoFile = "endo.dna";
	bool loadfile = false;

	if (argc == 2)
	{
		prefixFile = argv[1];
		endoFile = "";
		loadfile = true;
	}
	else if (argc == 3)
	{
		prefixFile = argv[1];
		endoFile = argv[2];
		loadfile = true;
	}
	else if (argc == 4)
	{
		string dna2;
		cin >> dna;
		cin >> dna2;
		dna.append (dna2);
		cout << dna << endl;
	}

	if (loadfile)
	{
		ifstream fin(prefixFile.c_str ());  
		fin >> dna;
		fin.close();

		cout << dna.length() << endl;
		if (endoFile.length() > 0)
		{
			string dna2;
			ifstream fin1(endoFile.c_str ());
			fin1 >> dna2;
			fin1.close();
			dna.append (dna2);
			cout << dna.length() << endl;
		}

	}

	//vector<string::size_type> a;
	//string str;
	//string f = "IIICCCCPIC";
	//string::size_type pos = dna.find (f, 0);
	//while (pos != string::npos)
	//{

	//	a.push_back (pos);		
	//	pos = dna.find (f, pos+1);
	//}

	//for (int i = 0; i < a.size(); i++)
	//{
	//	cout << a[i] << endl;
	//}

	logfile.open ("log");

	execute(dna);

	logfile.close();
	return 0;
}
