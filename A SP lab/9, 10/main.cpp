#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class MNT
{
public:
    int index;
    string m_name;
    int MDT_index;

    MNT(int index, string m_name, int MDT_index)
    {
        this->index = index;
        this->m_name = m_name;
        this->MDT_index = MDT_index;
    }
};

class MDT
{
public:
    int index;
    string card;

    MDT(int index, string card)
    {
        this->index = index;
        this->card = card;
    }
};

class ALA
{
public:
    string m_name;
    string formal_param;
    string positional_param;

    ALA(string m_name, string formal_param, string positional_param)
    {
        this->m_name = m_name;
        this->formal_param = formal_param;
        this->positional_param = positional_param;
    }
};

class AP
{
public:
    string macro_name;
    int actual_parameter;
    string positional_parameter;

    AP(string macro_name, int actual_parameter, string positional_parameter)
    {
        this->macro_name = macro_name;
        this->actual_parameter = actual_parameter;
        this->positional_parameter = positional_parameter;
    }
};

vector<MNT> mnt;
vector<MDT> mdt;
vector<ALA> ala;
vector<AP> ap;

int mntCount = 1;
int mdtCount = 20;

void printMDTTable()
{
    cout << "MDT table is" << endl;
    cout << "Index\tCard" << endl;
    for (const auto &pair : mdt)
    {
        cout << pair.index << "\t" << pair.card << endl;
    }
}

void printALATable()
{
    if (ala.empty())
        return; // Don't print if ALA is empty
    cout << "ALA table is:" << endl;
    cout << "Macro Name\t\t\t\t Formal Parameter\t\t\t\t Positional Parameter" << endl;
    for (const auto &pair : ala)
    {
        cout << pair.m_name << "\t\t\t\t" << pair.formal_param << "\t\t\t\t" << pair.positional_param << endl;
    }
}

void printMNTTable()
{
    cout << "MNT table is:" << endl;
    cout << "Index\tCard\tMDT Index" << endl;
    for (const auto &pair : mnt)
    {
        cout << pair.index << "\t" << pair.m_name << "\t" << pair.MDT_index << endl;
    }
}

void printAPTable()
{
    if (ap.empty())
        return; // Don't print if AP is empty
    cout << "Actual vs Positional table:" << endl;
    cout << "Macro Name\t\t\t\tActual\t\t\t\tPositional" << endl;
    for (const auto &pair : ap)
    {
        cout << pair.macro_name << "\t\t\t\t" << pair.actual_parameter << "\t\t\t\t" << pair.positional_parameter << endl;
    }
}

bool isSubstringPresent(const string &mainString, const string &substring)
{
    return mainString.find(substring) != string::npos;
}

string getPositionalParam(string macro_name, string parameter)
{
    for (const auto &pair : ala)
    {
        if (pair.m_name == macro_name && pair.formal_param == parameter)
        {
            return pair.positional_param;
        }
    }
    return "not found";
}

pair<string, int> isValuePresent(const vector<MNT> &myMap, const string &valueToFind)
{
    pair<string, int> ans;
    for (const auto &pair : myMap)
    {
        if (pair.m_name == valueToFind)
        {
            ans = make_pair(pair.m_name, pair.MDT_index);
            break;
        }
    }
    return ans;
}

void takeInput(string &filename)
{
    ifstream file(filename);
    string line;
    string macro_name;
    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        while (getline(iss, token))
        {
            if (isSubstringPresent(token, "MACRO"))
            {
                istringstream mnamestream(token);
                string temp;

                while (mnamestream >> temp)
                {
                    if (temp != "MACRO")
                    {
                        macro_name = temp;
                        mnt.push_back(MNT(mntCount++, macro_name, mdtCount));
                        int count = 1;
                        while (mnamestream >> temp)
                        {
                            ala.push_back(ALA(macro_name, temp, "#" + to_string(count++)));
                        }
                        break;
                    }
                }
            }
            else
            {
                string nextWord;
                istringstream mdtiss(token);
                string combine;

                mdtiss >> nextWord;
                string parameter;
                mdtiss >> parameter;

                string positional_param = getPositionalParam(macro_name, parameter);

                if (positional_param != "not found")
                {
                    mdt.push_back(MDT(mdtCount++, nextWord + " " + positional_param));
                }
                else
                {
                    mdt.push_back(MDT(mdtCount++, nextWord + " " + parameter));
                }
            }
        }
    }
}

int getActualValue(string macro_name, string argument)
{
    int ans = 0;
    for (auto &i : ap)
    {
        if (i.macro_name == macro_name && i.positional_parameter == argument)
        {
            ans = i.actual_parameter;
            break;
        }
    }
    return ans;
}

void createAP()
{
    ofstream outfile("output3.txt");
    ifstream file("code.txt");
    string line;

    while (getline(file, line))
    {
        istringstream iss(line);
        string token;
        iss >> token;
        pair<string, int> macro_name = isValuePresent(mnt, token);
        int count = 1;
        int val = macro_name.second;

        if (val != 0)
        {
            while (iss >> token)
            {
                ap.push_back(AP(macro_name.first, stoi(token), "#" + to_string(count++)));
            }

            string card;
            for (const auto &pair : mdt)
            {
                if (pair.index == val && !isSubstringPresent(pair.card, "mend"))
                {
                    istringstream cardIss(pair.card);
                    string instruction, argument;
                    cardIss >> instruction >> argument;

                    int actual_value = getActualValue(macro_name.first, argument);
                    if (actual_value != 0)
                    {
                        outfile << instruction << " " << actual_value << "\n";
                    }
                    else
                    {
                        outfile << instruction << " " << argument << "\n";
                    }
                    val++;
                }
            }
        }
        else
        {
            outfile << line << "\n";
        }
    }
}

int main()
{
    string filename = "input.txt";
    takeInput(filename);

    printMNTTable();

    printALATable();

    printMDTTable();

    createAP();

    printAPTable();
}
