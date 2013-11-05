#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#include<exception>
#include<sstream> 
#include<stdexcept> 
#include<typeinfo>

using namespace std;
#define sz(a) ((int)(a).size())
class my_exception : public exception
{
public:
  my_exception(string m="Exception!") : msg(m) {}
  ~my_exception() throw() {}
  const char* what() const throw() { return msg.c_str(); }

private:
  string msg;
};

class length_exception : public my_exception
{
public:
  length_exception(string m="Exception!") : my_exception(m) {}
  ~length_exception() throw() {}
};

class convertion_exception : public my_exception
{
public:
  convertion_exception(string m="Exception!") : my_exception(m) {}
  ~convertion_exception() throw() {}
};



class table {
    private:
        int prec;
        vector<vector<double> > data;
        vector<string> names;

        static vector<string> get_tokens(const string& line) {
            vector<string> tokens;
            stringstream ss(line);
            string s;
            while (ss >> s) {
                tokens.push_back(s);
            }
            return tokens;
        }

        static string int_to_string(const int x) {
            stringstream ss;
            ss << x;
            return ss.str();
        }

        static double string_to_double(const string& s) {
            stringstream ss(s);
            double x;
            if (!(ss >> x)) {
                throw convertion_exception("Trying convert " + s + " to double!");
            }
            string rest;
            getline(ss, rest);
            if (rest != "") {
                throw convertion_exception("Trying convert " + s + " to double!");
            }
            return x;
        }

        static bool is_double(const string& s) {
            try {
                string_to_double(s);
            } catch (exception e) {
                return false;
            }
            return true;
        }

        static bool is_header(vector<string>& h) {
            for (int i = 0; i < sz(h); i++) {
                if (!is_double(h[i])) {
                    return true;
                }
            }
            return false;
        }

        void add_line(vector<string>& s) {
            vector<double> v;
            for (int i = 0; i < sz(s); i++) {
                v.push_back(string_to_double(s[i]));
            }
            data.push_back(v);
        }
    public:
        table() {
            prec = 3;
        }
        void set_prec(int k) {
            prec = k;
        }
        friend istream& operator >>(istream&, table&);
        friend ostream& operator <<(ostream&, const table&);
};
istream& operator >>(istream& i, table& t) {
    string line;
    getline(i, line);
    vector<string> first = table::get_tokens(line);
    if (sz(first) == 0) {
        throw length_exception("Zero length!");
    }
    if (table::is_header(first)) {
        t.names = first;
    } else {
        for (int i = 1; i <= sz(first); i++) {
            t.names.push_back("column " + table::int_to_string(i));
        }
        t.add_line(first);
    }
    while (getline(i, line)) {
        first = table::get_tokens(line);
        if (sz(first) == 0) {
            break;
        }
        if (sz(first) != sz(t.names)) {
            throw length_exception("Length is " + table::int_to_string(sz(first)) + ", but " + table::int_to_string(sz(t.names)) + " needed!");
        }
        t.add_line(first);        
    }
    return i;
}
ostream& operator <<(ostream& o, const table& t) {
    int old_prec = o.precision();
    ios::fmtflags flags(o.flags());
    o<<fixed;
    o.precision(t.prec);
    vector<int> len(sz(t.names));
    for (int i = 0; i < sz(t.names); i++) {
        len[i] = sz(t.names[i]);
        for (int j = 0; j < sz(t.data); j++) {
            stringstream ss;
            ss.flags(o.flags());
            ss.precision(t.prec);
            ss << t.data[j][i];
            cerr<<ss.str()<<endl;
            len[i] = max(len[i], sz(ss.str()));
        }
        cerr<<len[i]<<endl;
    }
    for (int i = 0; i < sz(t.names); i++) {
        if (i > 0)
            len[i]++;
        o.width(len[i]);
        o<<t.names[i];
    }
    o<<endl;

    for (int j = 0; j < sz(t.data); j++) {
        for (int i = 0; i < sz(t.names); i++) {
            o.width(len[i]);
            o << t.data[j][i];
        }
        o<<endl;
    }
    o.flags(flags);
    o.precision(old_prec);
    return o;    
}
int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    table t;
    cin >> t;
    cout << t;
    return 0;
}