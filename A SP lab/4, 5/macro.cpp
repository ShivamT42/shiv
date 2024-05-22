#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Macro
{
public:
    struct MacroDefinition
    {
        vector<string> lines;
    };

private:
    struct MNTEntry
    {
        string name;
        int index;
        int mdtIndex;
    };

    struct MDTEntry
    {
        int index;
        string card;
    };

    unordered_map<string, MacroDefinition> macroTable;
    vector<string> codeLines;
    vector<MNTEntry> mntTable;
    vector<MDTEntry> mdtTable;

public:
    void insertMacro(const string &name, const MacroDefinition &definition)
    {
        macroTable[name] = definition;
    }

    void insertCodeLine(const string &line)
    {
        codeLines.push_back(line);
    }

    void insertMNT(const string &name, int index, int mdtIndex)
    {
        mntTable.push_back({name, index, mdtIndex});
    }

    void insertMDT(int index, const string &card)
    {
        mdtTable.push_back({index, card});
    }

    void expandMacros()
    {
        ofstream outputFile("output.txt");
        if (!outputFile.is_open())
        {
            cerr << "Unable to open output file." << endl;
            return;
        }

        for (const auto &line : codeLines)
        {
            if (macroTable.find(line) != macroTable.end())
            {
                const auto &macro = macroTable[line];
                for (const auto &macroLine : macro.lines)
                {
                    outputFile << macroLine << endl;
                }
            }
            else
            {
                outputFile << line << endl;
            }
        }

        outputFile.close();
    }

    void displayMNT()
    {
        cout << "Macro Name Table (MNT):" << endl;
        cout << "Index\tName\tMDT Index" << endl;
        for (const auto &entry : mntTable)
        {
            cout << entry.index << "\t" << entry.name << "\t" << entry.mdtIndex << endl;
        }
    }

    void displayMDT()
    {
        cout << "Macro Definition Table (MDT):" << endl;
        cout << "Index\tCard" << endl;
        for (const auto &entry : mdtTable)
        {
            cout << entry.index << "\t" << entry.card << endl;
        }
    }
};

int main()
{
    Macro macro;
    ifstream inputFile("input3.txt");
    if (inputFile.is_open())
    {
        string line;
        int mntIndex = 1;
        int mdtIndex = 21;
        while (getline(inputFile, line))
        {
            if (line.find("macro") == 0)
            {
                istringstream iss(line);
                string m, name;
                iss >> m >> name;
                Macro::MacroDefinition definition;
                macro.insertMNT(name, mntIndex, mdtIndex);
                while (getline(inputFile, line) && line != "mend")
                {
                    definition.lines.push_back(line);
                    macro.insertMDT(mdtIndex, line);
                    mdtIndex++;
                }
                macro.insertMacro(name, definition);
                mntIndex++;
            }
            else if (line == ".code")
            {
                while (getline(inputFile, line))
                {
                    if (line == "endp")
                        break;
                    macro.insertCodeLine(line);
                }
            }
        }
        macro.expandMacros();
        macro.displayMNT();
        macro.displayMDT();
        inputFile.close();
    }
    else
    {
        cerr << "Unable to open input file." << endl;
    }

    return 0;
}
