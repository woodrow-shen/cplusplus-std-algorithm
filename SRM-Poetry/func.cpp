#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;

class Poetry
{
    private:
        string tolowercase(string line)
        {
            int i=0;
            string result="";
            while (line[i])
            {
                result+=std::tolower(line[i]);
                i++;
            }
            return result;
        }

    public:
        string rhymeScheme(vector<string> poem)
        {
            string result;
            vector<pair <char, string>> rhymes;
            char letter = 'a';

            for (int i=0; i<poem.size(); i++)
            {
                // convert uppercase to lowercase
                string lowerstring = tolowercase(poem[i]);
                istringstream in(lowerstring);
                vector<string> split_words;
                copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(split_words));
                //cout << i << " round:" << endl;

                // no string, probably it's space text
                if (split_words.empty())
                {
                    result += " ";
                    continue;
                }

                string pattern; 
                bool vowels=false;
                string lastword = split_words.back();
                for(int i = lastword.size() - 1 ; i >= 0; i--)
                {
                    if (lastword.size() == 1) { pattern = lastword; break; }
                    if (lastword[i]=='a' || lastword[i]=='e' || lastword[i]=='i' || lastword[i]=='o' || lastword[i]=='u'
                            || (lastword[i]=='y' && (i != 0 && i != lastword.size()-1)))
                    {
                        vowels = true; 
                    } 
                    else
                    {
                        if (vowels == true) break;
                    }
                    pattern += lastword[i];
                }

                std:reverse(pattern.begin(), pattern.end());
                //cout << pattern << " " << letter << endl;
                string curr_letter;

                if (rhymes.empty())
                {
                    rhymes.push_back(std::make_pair(letter, pattern));
                    curr_letter = letter;
                    // move on next letter
                    letter = (letter == 'z') ? 'A' : letter+1;
                }
                else
                {
                    // check if pattern in rhymes
                    bool is_pattern_rhyme = false;
                    for (auto it = rhymes.begin(); it != rhymes.end(); it++)
                    {
                        if (it->second == pattern)
                        {
                            curr_letter = it->first;
                            is_pattern_rhyme = true;
                            break;
                        }
                    }

                    // no match, need to move on next finding
                    if (!is_pattern_rhyme)
                    {
                        rhymes.push_back(std::make_pair(letter, pattern));
                        curr_letter = letter;
                        // move on next letter
                        letter = (letter == 'z') ? 'A' : letter+1;
                    }
                }

                result += curr_letter;
            }
            return result;
        }
};

int main(int argc, char *argv[])
{
    vector<string> poem1 = {
        "     ",
        "Measure your height",
        "AND WEIGHT      ",
        "said the doctor",
        "",
        "And make sure to take your pills",
        "   to   cure   your    ills",
        "Every",
        "DAY"
    };
    vector<string> poem2 = {
        "",
        "",
        "",
        ""
    };
    vector<string> poem3 = {
        "This poem has uppercase letters",
        "In its rhyme scheme",
        "Alpha", "Blaster", "Cat", "Desert", "Elephant", "Frog", "Gulch", 
        "Horse", "Ireland", "Jam", "Krispy Kreme", "Loofah", "Moo", "Narf",
        "Old", "Pink", "Quash", "Rainbow", "Star", "Tour", "Uvula", "Very",
        "Will", "Xmas", "Young", "Zed", "deception", "comic", "grout",
        "oval", "cable", "rob", "steal", "steel", "weak"
    };
    vector<string> poem4 = {
        "too bad   your",
        "     solution went   sour"
    };
    vector<string> poem5 = {
        "One bright day in the middle of the night",
        "Two dead boys got up to fight",
        "Back to back they faced each other",
        "Drew their swords and shot each other",
        "",
        "A deaf policeman heard the noise",
        "And came to arrest the two dead boys",
        "And if you dont believe this lie is true",
        "Ask the blind man he saw it too"
    };

    Poetry p;
    cout << "\"" << p.rhymeScheme(poem3) << "\""<< endl;
    return 0;
}
