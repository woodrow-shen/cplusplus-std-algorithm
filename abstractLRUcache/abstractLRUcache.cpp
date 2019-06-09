#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
#include "cxxopts.hpp"

using namespace std;

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

struct Node
{
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
    Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache
{   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};

class LRUCache : public Cache
{
    public:
    // constructor
    LRUCache(int capacity)
    {
        cp = capacity;
        head = tail = nullptr;
    }
    // destructor
    ~LRUCache()
    {
        for (; head != nullptr;)
        {
            Node *temp = head;
            head = head->next;
            if (temp != nullptr)
                delete temp;
        }    
    }

    void set(int key, int value)
    {
        if (head == nullptr)
        {
            Node *temp = new Node(key, value);
            head = temp;
            tail = temp;
            mp.insert(std::pair<int,Node*>(key, temp));
        }
        else
        {
            // need to consider cache hit
            auto it = mp.find(key);
            // cache hit
            if (it != mp.cend())
            {
                // traverse linked list to remove original node
                for (auto node_it = head; node_it != nullptr; node_it=node_it->next)
                {
                    //cout << "cur->key=" << node_it->key << endl; 
                    if (node_it->key == key)
                    {
                        if ((node_it == head) && (node_it == tail))
                        {
                           head = nullptr;
                           tail = nullptr;
                        }
                        else 
                        {
                            Node *temp1 = node_it->next;
                            if (temp1 == nullptr)
                            {
                                tail = node_it->prev;
                                tail->next = nullptr;
                            }
                            else
                            {
                                temp1->prev = node_it->prev;
                            }
                            Node *temp2 = node_it->prev;
                            if (temp2 == nullptr)
                            {
                                head = node_it->next;
                                head->prev = nullptr;
                            }
                            else
                            {
                                temp2->next = temp1;
                            }
                        }
                        delete node_it;
                        break;
                    }
                }
                // add new key as the most recently used key
                if (head == nullptr)
                {
                    Node *temp = new Node(key, value);
                    head = temp;
                    tail = temp;
                    mp.erase(it);
                    mp.insert(std::pair<int,Node*>(key, temp));
                }
                else
                {
                    Node *temp = new Node(nullptr, head, key, value);
                    head->prev = temp;
                    head = temp;
                    mp.erase(it);
                    mp.insert(std::pair<int,Node*>(key, temp));
                }
            }
            // cache miss, then add new key into map with discarding least element
            else
            {
                //cout << "mp.size()= " << mp.size() << endl;
                if (mp.size() < cp)
                {
                    Node *temp = new Node(nullptr, head, key, value);
                    head->prev = temp;
                    head = temp;
                    mp.insert(std::pair<int,Node*>(key, temp));
                }
                else
                {
                    // abandon least recent used key
                    Node *old_tail = tail;
                    auto it = mp.find(old_tail->key);
                    if (it != mp.cend())
                        mp.erase(it);
                    tail = tail->prev;

                    // add new key
                    Node *temp = new Node(nullptr, head, key, value);
                    head->prev = temp;
                    head = temp;
                    mp.insert(std::pair<int,Node*>(key, temp));
                    delete old_tail;
                }
            }
        }
    }

    int get(int key)
    {
        auto it = mp.find(key);
        if (it != mp.cend())
        {
            return it->second->value;
        }
        else
        {
            return -1;
        }
    }
};

int main(int argc, char *argv[]) 
{
    int n, capacity,i;
    auto result = parse(argc, argv);
    auto arguments = result.arguments();
    cout << "Saw " << arguments.size() << " arguments" << endl;

    ifstream inputfile ("inputfile");
    ofstream debugfile;
    ofstream outputfile ("output");
    
    if (result.count("d"))
    {
        debugfile.open("debugfile");
    }

    string line;
    if (inputfile.is_open()) 
    {
        getline(inputfile, line);
        {
            istringstream is(line);
            cout << line << endl;
            is >> n;
            is >> capacity;
        }
    }

    if (debugfile.is_open()) 
    {
        cout << "n=" << n << endl;
        cout << "capacity=" << capacity << endl;
    }

    LRUCache l(capacity);
    for(i=0;i<n;i++) 
    {
        string command;
        int key, value;
        getline(inputfile, line);
        {
            istringstream is(line);
            is >> command ;
            if(command == "get") 
            {
                is >> key;
            }
            else if(command == "set") 
            {
                is >> key;
                is >> value;
            }
        }
        if (debugfile.is_open()) 
        {
            if (command == "get") 
            {
                debugfile << "command=" << command << " " << key << endl;
            }
            else if (command == "set") 
            {
                debugfile << "command=" << command << " " << key << " " << value << endl;
            }
        }
        if (command == "get") 
        {
            outputfile << l.get(key) << endl;
        } 
        else if (command == "set") 
        {
            l.set(key,value);
        }
    }

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

