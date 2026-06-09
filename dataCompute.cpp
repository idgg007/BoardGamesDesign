#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
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
vector<std::vector<double>> chance;
vector<vector<pair<int, int>>> man;
vector<int> gate;
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
    chance.resize(count_country);
    buff.assign(count_country, vector<Buff>());
    gate.assign(count_country, 5);
    for (int i = 0; i < count_country; i++)
    {
        int count_man;
        readMan >> tag[i];
        readMan >> count_man;
        int f, s;
        while (count_man)
        {
            readMan >> f >> s;
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
        readMan >> gate[i];
    }
    readMan.close();
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
    readExtra.close();
    for (int i = 0; i < count_country; i++)
    {
        ifstream readRule(ruleFrom[i]);
        chance[i].assign(5, 0.0);
        for (int j = 0; j < 5; j++)
        {
            readRule >> chance[i][j];
        }
    }
}

vector<double> allDivide;

set<double> all;

double divide[5];

double ans = 1.0;

int ansCount = 0; // ans更新的次数

double Judge(const int &d, const int &f, const int &fAdd, const int &dAdd, const int &c)
{
    double f = sum_1[f] + fAdd;
    int tryCount = 1;
    for (size_t i = 0; i < buff[f].size(); i++)
    {
        if ((buff[f][i].type & c) && buff[f][i].l <= fAdd && buff[f][i].r >= fAdd)
        {
            tryCount = max(tryCount,buff[f][i].tryCount);
            f += buff[f][i].add;
        }
        
    }
}

double JudgeRule()
{
    for (size_t i = 0; i < tag.size(); i++)
    {
        for (size_t j = i + 1; j < tag.size(); j++)
        {
            //
            double expected_i = 0; // i防守i的收益
            double expected_j = 0; // j防守j的收益
            unsigned int limit_i = (1 << (man[i].size()));
            unsigned int limit_j = (1 << (man[j].size()));
            for (unsigned int k = 0; k < limit_i; k++)
            {
                double newAns = 0;
                int secondAdd = 0;
                int firstAdd = 0;
                for (unsigned int l = 0, c = 1; l < man.size(); l++, (c <<= 1))
                {
                    if (c & k)
                    {
                        firstAdd += man[i][l].first;
                    }
                    else
                    {
                        secondAdd += man[i][l].second;
                    }
                }
            }
        }
    }
}

void DFS(const int &count, const size_t &it) // 创建枚举比例
{
    if (count == 5)
    {
        double newAns = JudgeRule();
        if (newAns < ans)
        {
            ansCount++;
            ans = newAns;
            char name[20];
            sprintf(name, "ans%d.txt", ansCount);
            ofstream writeAns(name);
            for (int i = 0; i < 5; i++)
            {
                writeAns << divide[i] << "\n";
            }
            writeAns.close();
        }
    }
    for (size_t i = it + 1; i < allDivide.size(); i++)
    {
        divide[count] = allDivide[i];
        DFS(count + 1, i);
    }
}

int main()
{
    for (double i = 1.0; i <= 11.0; i += 1.0)
    {
        if (!all.count(i / 4.0 - 0.001))
        {
            allDivide.push_back(i / 4.0 - 0.001);
            all.insert(i / 4.0 - 0.001);
        }
        if (!all.count(i / 5.0 - 0.001))
        {
            allDivide.push_back(i / 5.0 - 0.001);
            all.insert(i / 5.0 - 0.001);
        }
        if (!all.count(i / 6.0 - 0.001))
        {
            allDivide.push_back(i / 6.0 - 0.001);
            all.insert(i / 6.0 - 0.001);
        }
    }
    sort(allDivide.begin(), allDivide.end());
    init();
    DFS(0, 0);
}
