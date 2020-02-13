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

void printKMax(int arr[], int n, int k)
{
	//Write your code ,here.
    deque<int> queue;

    for (int i = 0; i < k; i++)
    {
        // remove all useless elements present at the front of the list
        while (!queue.empty() && arr[queue.front()] < arr[i])
        {
            queue.pop_front();
        }

        // add index of current element at the back
        queue.push_back(i);
    }

    cout << "indices in the first window of k size => [";
    for (auto& q : queue)
        cout << q << ",";
    cout << "]" << endl;

    for (int i = k; i < n; i++)
    {
        // first element present in the list is the greatest element for the last 'k' sized sub-array
        cout << arr[queue.front()] << " ";

        // now remove all indices of elements from the list which do not belong to current window
        while (!queue.empty() && (queue.front() < (i-k+1)))
        {
            queue.pop_front();
        }

        // now again remove all useless elements present at the front of the list
        // remove all useless elements present at the front of the list
        while (!queue.empty() && arr[queue.front()] < arr[i])
        {
            queue.pop_front();
        }

        // and finally insert this new element at the back of the list
        queue.push_back(i);
    }

    // now print the largest element from the last sub-array(window)
    cout << arr[queue.front()] << endl;
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

        if (pifs->is_open())
        {
            *pifs >> testcase;
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

	while(testcase > 0) 
    {
		int n,k;
    	*pifs >> n >> k;
    	int i;
    	int arr[n];
    	for(i = 0; i < n; i++)
      		*pifs >> arr[i];
    	printKMax(arr, n, k);
    	testcase--;
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
