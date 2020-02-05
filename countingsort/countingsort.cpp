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

enum Direction
{
    INC, DEC
};

void countSort(Direction direction, vector<int> arr, ofstream &debugfile)
{
    auto n = arr.size();
    vector<int> countsortlist(n), sortedlist(n);

    cout << "[";
    for (auto iter = arr.cbegin(); iter != arr.cend(); ++iter)
    {
        cout << *iter << " ";
    }
    cout << "]" << endl;

    // count each number to treat them as index
    for (int i = 0; i < n; i++)
    {
        int index = arr[i];
        countsortlist[index]++;
    }

    if (direction == Direction::INC)
    {
        // accumlate the counter of each number
        // this is about relative position comparison
        for (int i = 1; i < n; i++)
        {
            countsortlist[i] += countsortlist[i-1];
        }
    }
    else if (direction == Direction::DEC)
    {
        // accumlate the counter of each number
        // this is about relative position comparison
        for (int i = n - 1; i > 0; i--)
        {
            countsortlist[i-1] += countsortlist[i];
        }
    }

    // reverse-iterate original list to put each number according to its position
    for (int i = n; i > 0; i--)
    {
        sortedlist[--countsortlist[arr[i-1]]] = arr[i-1];
    }

    cout << "[";
    for (auto iter = sortedlist.cbegin(); iter != sortedlist.cend(); ++iter)
    {
        cout << *iter << " ";
    }
    cout << "]" << endl;

}

// Complete the countSort function below.
void countSort(vector<vector<string>> arr, ofstream &debugfile)
{
    auto n = arr.size();
    vector<vector<pair<int, string>>> countsortlist(n);
    // initialize counting sort list
    for (int i = 0; i < n; i++)
    {
        int index = std::stoi (arr[i][0], nullptr, 10);
        pair<int, string> temp(i, arr[i][1]);
        countsortlist[index].push_back(temp);
    }

    //for (vector<vector<pair<int, string>>>::const_iterator itr = countsortlist.cbegin(); itr != countsortlist.cend(); ++itr) {
    for (auto itr = countsortlist.cbegin(); itr != countsortlist.cend(); ++itr)
    {
        //for (vector<pair<int, string>>::const_iterator e = (*itr).cbegin(); e != (*itr).cend(); ++e) {
        for (auto e = (*itr).cbegin(); e != (*itr).cend(); ++e)
        {
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
            ("n, num", "Demostrate number version")
            ("r, reverse", "Print reverse sorting")
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

        if (result.count("n"))
        {
            std::cout << "Saw option num: demostrate number version" << std::endl;
        }

        if (result.count("r"))
        {
            std::cout << "Saw option reverse: Print reverse sorting" << std::endl;
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
    const string dictinputfile = "inputfile";
    const string numinputfile = "inputfile_number";
    auto result = parse(argc, argv);
    auto arguments = result.arguments();
    cout << "Saw " << arguments.size() << " arguments" << endl;

    string n_temp;
    ifstream *inputfile;
    ofstream debugfile;
    Direction dir = Direction::INC;

    if (result.count("d"))
    {
        debugfile.open("debugfile");
    }

    if (result.count("r"))
    {
        dir = Direction::DEC;
    }

    if (result.count("n"))
    {
        try
        {
            inputfile = new ifstream(numinputfile);

            if (inputfile->is_open())
            {
                getline(*inputfile, n_temp);
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

        int n = stoi(ltrim(rtrim(n_temp)));

        vector<int> arr(n);

        for (int i = 0; i < n; i++)
        {
            getline(*inputfile, n_temp);
            int n = stoi(ltrim(rtrim(n_temp)));

            arr[i] = n;
        }
        countSort(dir, arr, debugfile);
    }
    else
    {
        try
        {
            inputfile = new ifstream(dictinputfile);

            if (inputfile->is_open())
            {
                getline(*inputfile, n_temp);
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

        int n = stoi(ltrim(rtrim(n_temp)));

        vector<vector<string>> arr(n);

        for (int i = 0; i < n; i++)
        {
            arr[i].resize(2);

            string arr_row_temp_temp;
            getline(*inputfile, arr_row_temp_temp);

            vector<string> arr_row_temp = split(rtrim(arr_row_temp_temp));

            for (int j = 0; j < 2; j++)
            {
                string arr_row_item = arr_row_temp[j];

                arr[i][j] = arr_row_item;
            }
        }
        countSort(arr, debugfile);
    }

    if (inputfile->is_open())
    {
        inputfile->close();
    }
    if (debugfile.is_open())
    {
        debugfile.close();
    }

    free(inputfile);

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
