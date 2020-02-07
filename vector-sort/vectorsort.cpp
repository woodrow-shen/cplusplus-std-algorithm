#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include "cxxopts.hpp"

using namespace std;

cxxopts::ParseResult parse(int argc, char* argv[])
{
    try
    {
        cxxopts::Options options(argv[0], "A Vector Sort demonstration");

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
    vector<int> arr;
    cout << arr.size() << endl;

    while ( *pifs >> n )
    {
        if (n >= 1 && n <= 1000000000)
            arr.push_back(n);
    }
    cout << arr.size() << endl;
    sort(arr.begin(), arr.end());

    for (auto iter = arr.cbegin(); iter != arr.cend(); ++iter)
    {
        cout << *iter << " ";
    }
    cout << endl;
    
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
