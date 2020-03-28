#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class MatchMaking
{
    private:
        static int getScore(string answerWomen, string answerMen)
        {
            int score = 0;

            for (int i = 0; i < answerWomen.size(); i++)
            {
                if (answerWomen[i] == answerMen[i])
                    score++;
            }
            return score;
        }
    public:
        string makeMatch(vector <string> namesWomen, vector <string> answersWomen, vector <string> namesMen, vector <string> answersMen, string queryWoman)
        {
            vector<pair <string, string>> MatchWomen;
            vector<pair <string, string>> MatchMen;

            for (int i = 0; i < namesWomen.size(); i++)
            {
                MatchWomen.push_back(std::make_pair(namesWomen[i], answersWomen[i]));
                MatchMen.push_back(std::make_pair(namesMen[i], answersMen[i]));
            }

            sort(MatchWomen.begin(), MatchWomen.end());
            sort(MatchMen.begin(), MatchMen.end());

            for (auto itw = MatchWomen.cbegin(); itw != MatchWomen.cend(); itw++)
            {
                int maxmatch = -1;
                vector<pair <string, string>>::iterator match;
                for (auto itm = MatchMen.begin(); itm != MatchMen.end(); itm++)
                {
                    int score = getScore(itw->second, itm->second);
                    if (score > maxmatch)
                    {
                        maxmatch = score;
                        match = itm;
                    }
                }

                if (queryWoman == itw->first)
                    return match->first;
                else
                    MatchMen.erase(match);
            }
            return "";
        }
};
