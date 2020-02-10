#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include "cxxopts.hpp"

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

cxxopts::ParseResult parse(int argc, char* argv[])
{
    try
    {
        cxxopts::Options options(argv[0], "A Vector Erase demonstration");

        options.add_options("Group")
            ("d, debug", "Enable debugging")
            ("h, help", "Print help")
            ;

        auto result = options.parse(argc, argv);

        if (result.count("help"))
        {
            cout << options.help({"Group"}) << endl;
            exit(0);
        }

        if (result.count("d"))
        {
            std::cout << "Saw option debug" << std::endl;
        }

        return result;
    }
    catch (const cxxopts::OptionException& e)
    {
        cout << "error parsing options: " << e.what() << endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    const string inputfilename = "inputfile";
    auto result = parse(argc, argv);
    auto arguments = result.arguments();
    cout << "Saw " << arguments.size() << " arguments" << endl;

    string n_temp;
    ifstream *pifs;
    ofstream debugfile;

    if (result.count("d"))
    {
        debugfile.open("debugfile");
    }

    try
    {
        pifs = new ifstream(inputfilename);

        if (pifs->is_open())
        {
            getline(*pifs, n_temp);
        }
        else
        {
            throw static_cast<string>(strerror(errno));
        }
    }
    catch (std::ios_base::failure& e)
    {
        cerr << e.what() << endl;
    }
    catch (const string& e)
    {
        cerr << "Catching an exception: " << e << endl;
        return errno;
    }

    int n = stoi(n_temp);
    set<int> stlset;

    while (getline(*pifs, n_temp))
    {
        vector<string> command = split(n_temp);
        if (command.size() == 2)
        {
            int query_type = stoi(command[0]);
            int query_value = stoi(command[1]);

            switch (query_type)
            {
              case 1:
                stlset.insert(query_value);
                break;
              case 2:
                stlset.erase(query_value);
                break;
              case 3:
                cout <<(stlset.find(query_value) != stlset.end() ? "Yes" : "No") << endl;
                break;
              default:
                break;
            }
        }
    }

    if (pifs->is_open())
    {
        pifs->close();
    }
    if (debugfile.is_open())
    {
        debugfile.close();
    }

    free(pifs);

    return 0;
}

string ltrim(const string &str)
{
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str)
{
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str)
{
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos)
    {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
