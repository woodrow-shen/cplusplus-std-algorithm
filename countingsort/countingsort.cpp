#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include "cxxopts.hpp"

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

// Complete the countSort function below.
void countSort(vector<vector<string>> arr, ofstream &debugfile)
{
    auto n = arr.size();
    vector<vector<pair<int, string>>> countsortlist(n);
    // initialize counting sort list
    for (int i=0; i < n; i++)
    {
        int index = std::stoi (arr[i][0], nullptr, 10);
        pair<int, string> temp(i, arr[i][1]);
        countsortlist[index].push_back(temp);
    }

    for (auto itr = countsortlist.cbegin(); itr != countsortlist.cend(); ++itr)
    {
    //for (vector<vector<pair<int, string>>>::const_iterator itr = countsortlist.cbegin(); itr != countsortlist.cend(); ++itr) {
        for (auto e = (*itr).cbegin(); e != (*itr).cend(); ++e)
        {
        //for (vector<pair<int, string>>::const_iterator e = (*itr).cbegin(); e != (*itr).cend(); ++e) {
            if ((*e).first < n/2)
            {
                cout << "-" << " ";
            }
            else
            {
                cout << (*e).second << " ";
            }
            if (debugfile.is_open())
            {
                debugfile << "(" << (*e).first;
                debugfile << "," << (*e).second << ")" << endl;
            }
        }
    }
    cout << endl;
}

cxxopts::ParseResult parse(int argc, char* argv[])
{
    try
    {
        cxxopts::Options options(argv[0], "A Counting Sort demonstration");

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
    auto result = parse(argc, argv);
    auto arguments = result.arguments();
    cout << "Saw " << arguments.size() << " arguments" << endl;

    string n_temp;
    ifstream inputfile ("inputfile");
    ofstream debugfile;

    if (inputfile.is_open())
    {
        getline(inputfile, n_temp);
    }

    if (result.count("d"))
    {
        debugfile.open("debugfile");
    }

    int n = stoi(ltrim(rtrim(n_temp)));

    vector<vector<string>> arr(n);

    for (int i = 0; i < n; i++)
    {
        arr[i].resize(2);

        string arr_row_temp_temp;
        getline(inputfile, arr_row_temp_temp);

        vector<string> arr_row_temp = split(rtrim(arr_row_temp_temp));

        for (int j = 0; j < 2; j++)
        {
            string arr_row_item = arr_row_temp[j];

            arr[i][j] = arr_row_item;
        }
    }

    countSort(arr, debugfile);
    if (inputfile.is_open())
    {
        inputfile.close();
    }
    if (debugfile.is_open())
    {
        debugfile.close();
    }

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
