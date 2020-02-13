#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <deque>
#include "cxxopts.hpp"

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

//Define the structs Workshops and Available_Workshops.
//Implement the functions initialize and CalculateMaxWorkshops
struct Workshop
{
    int start_time;
    int end_time;
    int duration;
};

struct Available_Workshops
{
    int size;
    vector<Workshop> vec_workshop;
};

struct comp
{
    bool operator()(const Workshop& l, const Workshop& r) const
    {
        return l.end_time < r.end_time;
    }
};

Available_Workshops* initialize(int* start_time, int* duration, int n)
{
    Available_Workshops* paws = new Available_Workshops();
    paws->vec_workshop.resize(n);
    paws->size = n;

    for (int i = 0; i < n; i++)
    {
        paws->vec_workshop[i].start_time = start_time[i];
        paws->vec_workshop[i].duration = duration[i];
        paws->vec_workshop[i].end_time = start_time[i]+duration[i];
    }
    return paws;
}

int CalculateMaxWorkshops(Available_Workshops* paws)
{
    int max = 0, current_index = 0;

    // sort workshop array
    //std::sort(paws->vec_workshop::begin(), paws->vec_workshop::end(), comp());
    std::sort(paws->vec_workshop.begin(), paws->vec_workshop.end(),
        [](const Workshop& l, Workshop& r) {
            return l.end_time < r.end_time;
        });

    cout << current_index << ",";
    max++;

    for (auto iter = paws->vec_workshop.cbegin()+1; iter != paws->vec_workshop.cend(); iter++)
    {
        if (iter->start_time >= paws->vec_workshop[current_index].end_time)
        {
            current_index = iter - paws->vec_workshop.cbegin();
            cout << current_index << ",";
            max++;
        }
    }
    cout << endl;
    return max;
}

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

    int testcase;
    ifstream *pifs;
    ofstream debugfile;

    if (result.count("d"))
    {
        debugfile.open("debugfile");
    }

    try
    {
        pifs = new ifstream(inputfilename);

        if (!pifs->is_open())
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

    int n; // number of workshops
    *pifs >> n;
    // create arrays of unknown size n
    int* start_time = new int[n];
    int* duration = new int[n];

    for(int i=0; i < n; i++){
        *pifs >> start_time[i];
    }
    for(int i = 0; i < n; i++){
        *pifs >> duration[i];
    }

    Available_Workshops * ptr;
    ptr = initialize(start_time,duration, n);
    cout << CalculateMaxWorkshops(ptr) << endl;

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
