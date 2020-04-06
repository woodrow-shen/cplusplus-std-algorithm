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
        cxxopts::Options options(argv[0], "A LCS demonstration");

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

// 遞迴版本
void print_LCS(vector<vector<int>>& lut, vector<int>& s1, int i, int j)
{
    // 第一個或第二個序列為空集合就停止
    if (i == 0 || j == 0) return;

    if (lut[i][j] == 0)        // 左上方
    {
        print_LCS(lut, s1, i-1, j-1);
        cout << s1[i] << " ";  // 印出LCS的元素
    }
    else if (lut[i][j] == 1)   // 左方
        print_LCS(lut, s1, i, j-1);
    else if (lut[i][j] == 2)   // 上方
        print_LCS(lut, s1, i-1, j);
}

void LCS_dp(vector<vector<int>>& lut, vector<int>& s1, vector<int>& s2)
{
    vector<vector<int>> lcs (s1.size(), vector<int>(s2.size()));

    for (int i = 1; i < s1.size(); i++)
    {
        for (int j = 1; j < s2.size(); j++)
        {
            if (s1[i] == s2[j])
            {
                lcs[i][j] = lcs[i-1][j-1] + 1;
                lut[i][j] = 0; // 左上方
            }
            else
            {
                if (lcs[i-1][j] < lcs[i][j-1])
                {
                    lcs[i][j] = lcs[i][j-1];
                    lut[i][j] = 1; // 左方
                }
                else
                {
                    lcs[i][j] = lcs[i-1][j];
                    lut[i][j] = 2; // 上方
                }
            }
        }
    }
    print_LCS(lut, s1, s1.size()-1, s2.size()-1);
    cout << endl;
}

int compute_LCS_dp(vector<int>& s1, vector<int>& s2)
{
    vector<vector<int>> lcs (s1.size(), vector<int>(s2.size()));

    for (int i = 1; i < s1.size(); i++)
    {
        for (int j = 1; j < s2.size(); j++)
        {
            if (s1[i] == s2[j])
            {
                lcs[i][j] = lcs[i-1][j-1] + 1;
            }
            else
            {
                lcs[i][j] = std::max(lcs[i-1][j], lcs[i][j-1]);
            }
        }
    }
    return lcs[s1.size()-1][s2.size()-1];
}

int main(int argc, char* argv[])
{
    auto result = parse(argc, argv);
    auto arguments = result.arguments();
    cout << "Saw " << arguments.size() << " arguments" << endl;

    vector<int> s1 {0, 2, 5, 7, 9, 3, 1, 2};
    vector<int> s2 {0, 3, 5, 3, 2, 8};
    vector<vector<int>> lookuptable (s1.size(), vector<int>(s2.size()));

    int lcs = compute_LCS_dp(s1, s2);
    cout << "LCS = " << lcs << endl;
    LCS_dp(lookuptable, s1, s2);

    return 0;
}
