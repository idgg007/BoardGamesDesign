#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
struct Buff
{
    char type = 3; // where
    int tryCount = 0;
    int add = 0;
    int l = -10000, r = 10000;
};

vector<string> tag;
vector<string> ruleFrom;
vector<std::vector<Buff>> buff;
vector<vector<pair<int, int>>> man;
vector<int> sum_1;
vector<int> sum_2;

void init()
{
    ifstream readMan("org.ini");
    int count_country;
    readMan >> count_country;
    tag.resize(count_country);
    man.assign(count_country, vector<pair<int, int>>());
    sum_1.assign(count_country, 0);
    sum_2.assign(count_country, 0);
    ruleFrom.assign(count_country, "rule.ini");
    for (int i = 0; i < count_country; i++)
    {
        int count_man;
        cin >> tag[i];
        cin >> count_man;
        int f, s;
        while (count_man)
        {
            cin >> f >> s;
            if (s == 0)
            {
                sum_1[i] += f;
            }
            else if (f == 0)
            {
                sum_2[i] += s;
            }
            else
            {
                man[i].push_back(make_pair(f, s));
            }
            count_man--;
        }
    }
    ifstream readExtra("extra.ini");
    string str_in;
    str_in.clear();
    while (str_in != "end")
    {
        readExtra >> str_in;
        int it = 0;
        while (str_in != tag[it] && it < tag.size())
        {
            it++;
        }
        if (it == tag.size())
        {
            cout << "Error:" << str_in << " not found(extra.ini)\n";
        }
        str_in.clear();
        while (str_in != "end" && str_in != "next")
        {
            readExtra >> str_in;
            if (str_in == "AT")
            {
                Buff newBuff;
                str_in.clear();
                while (str_in != "out")
                {
                    readExtra >> str_in;
                    if (str_in == "limitAD")
                    {
                        readExtra >> newBuff.l >> newBuff.r;
                    }
                    else if (str_in == "limitT")
                    {
                        readExtra >> newBuff.type;
                    }
                    else if (str_in == "add")
                    {
                        readExtra >> newBuff.add;
                    }
                    else if (str_in == "try")
                    {
                        readExtra >> newBuff.tryCount;
                    }
                }
                buff[it].push_back(newBuff);
            }
            else if (str_in == "rule")
            {
                readExtra >> ruleFrom[it];
            }
        }
    }
}
int main()
{
    init();
}
