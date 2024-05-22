#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

class Assembler
{
    unordered_map<string, int *> mot;
    unordered_map<string, int> pot;
    unordered_map<string, int> symbolTable;
    int locationCounter = 0;

public:
    void initializeMOT()
    {
        mot["add"] = new int[2]{1, 1};
        mot["sub"] = new int[2]{2, 1};
        mot["mult"] = new int[2]{3, 1};
        mot["jmp"] = new int[2]{4, 1};
        mot["jneg"] = new int[2]{5, 1};
        mot["jpos"] = new int[2]{6, 1};
        mot["jz"] = new int[2]{7, 1};
        mot["load"] = new int[2]{8, 1};
        mot["store"] = new int[2]{9, 1};
        mot["read"] = new int[2]{10, 1};
        mot["write"] = new int[2]{11, 1};
        mot["stop"] = new int[2]{12, 0};
    }

    void initializePOT()
    {
        pot["db"] = 1;
        pot["dw"] = 1;
        pot["org"] = 1;
        pot["endp"] = 0;
        pot["const"] = 1;
        pot["end"] = 0;
    }

    void firstPass()
    {
        ifstream infile("input4.txt");
        ofstream outfile("output4.txt", ios_base::app);

        string line;
        getline(infile, line);
        outfile << line << endl;

        while (getline(infile, line))
        {
            istringstream iss(line);
            string tokens[5];
            for (int i = 0; i < 5; ++i)
            {
                iss >> tokens[i];
            }

            if (tokens[2] == "endp")
            {
                break;
            }

            if (tokens[3] != "-" && symbolTable.find(tokens[3]) == symbolTable.end())
            {
                symbolTable[tokens[3]] = -1; // equivalent to null in Java
            }

            if (tokens[1] != "-")
            {
                symbolTable[tokens[1]] = locationCounter;
            }

            int *array = mot[tokens[2]];
            outfile << line << "\t\t" << array[0] << endl;
            locationCounter += array[1] + 1;
        }
        cout << "Symbol Table in the first pass" << endl;
        displaySBT();

        secondPass(outfile, infile);

        cout << "\nSymbol Table in the second pass" << endl;
        displaySBT();
        infile.close();
        outfile.close();
    }

    void secondPass(ofstream &outfile, ifstream &infile)
    {
        string line;
        while (getline(infile, line))
        {
            if (line.find("end") != string::npos)
            {
                break;
            }
            istringstream iss(line);
            string tokens[5];
            for (int i = 0; i < 5; ++i)
            {
                iss >> tokens[i];
            }
            symbolTable[tokens[2]] = locationCounter;
            locationCounter += pot[tokens[3]];
        }
    }

    void displaySBT()
    {
        for (const auto &mapElement : symbolTable)
        {
            cout << mapElement.first << " -> " << mapElement.second << endl;
        }
    }

    void display()
    {
        ifstream infile("output4.txt");
        ofstream outfile("output5.txt");

        string line;
        getline(infile, line);
        outfile << line << "\t" << "output" << endl;

        while (getline(infile, line))
        {
            if (line.find("endp") == string::npos)
            {
                istringstream iss(line);
                string tokens[5];
                for (int i = 0; i < 5; ++i)
                {
                    iss >> tokens[i];
                }
                outfile << line << "\t" << symbolTable[tokens[3]] << endl;
            }
        }
        infile.close();
        outfile.close();
    }
};

int main()
{
    Assembler assembler;
    assembler.initializeMOT();
    assembler.initializePOT();
    assembler.firstPass();
    assembler.display();
    return 0;
}
