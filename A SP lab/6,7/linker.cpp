#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class Linker
{
public:
    static int addr;

    struct Node
    {
        std::string identifier;
        std::string dataType;
        int size;
        int addr;

        Node(std::string identifier, std::string dataType)
            : identifier(identifier), dataType(dataType), size(0), addr(-1) {}

        Node(std::string identifier, std::string dataType, int size, int addr)
            : identifier(identifier), dataType(dataType), size(size), addr(addr) {}
    };

    std::unordered_map<std::string, int> sizes;
    std::vector<Node> sbt1, et1, sbt2, et2, gvt;

    void initialiseSizes()
    {
        sizes["int"] = 4;
        sizes["float"] = 4;
        sizes["char"] = 2;
        sizes["long"] = 8;
        sizes["double"] = 8;
    }

    void read(const std::vector<std::string> &array, std::vector<Node> &list, int flag)
    {
        std::string dataType;
        int address;
        if (flag == 1)
        {
            dataType = array[1];
            address = -1;
        }
        else
        {
            dataType = array[0];
            address = addr;
        }

        for (size_t i = flag == 1 ? 2 : 1; i < array.size(); ++i)
        {
            std::string identifier = array[i];
            bool arr = false;
            int num = 0;
            if (identifier.find("[") != std::string::npos)
            {
                size_t j = identifier.find("[") + 1;
                size_t k = identifier.find("]");
                num = std::stoi(identifier.substr(j, k - j));
                identifier = identifier.substr(0, identifier.find("["));
                arr = true;
            }
            Node node(identifier, dataType);
            node.addr = address;
            int size;
            if (arr)
            {
                size = sizes[dataType] * num;
            }
            else
            {
                size = sizes[dataType];
            }
            node.size = size;
            if (address != -1)
            {
                address += size;
                addr = address;
            }
            list.push_back(node);
        }
    }

    void readFile(std::ifstream &file, int f)
    {
        std::string data;
        std::vector<Node> &sbt = (f == 1) ? sbt1 : sbt2;
        std::vector<Node> &et = (f == 1) ? et1 : et2;
        while (getline(file, data))
        {
            if (data.find("#") == std::string::npos &&
                data.find("main()") == std::string::npos &&
                data.find("{") == std::string::npos &&
                data.find("}") == std::string::npos)
            {

                std::stringstream ss(data);
                std::string token;
                std::vector<std::string> tokens;
                while (ss >> token)
                {
                    size_t pos;
                    while ((pos = token.find(',')) != std::string::npos)
                    {
                        tokens.push_back(token.substr(0, pos));
                        token.erase(0, pos + 1);
                    }
                    if (!token.empty())
                    {
                        tokens.push_back(token);
                    }
                }
                if (tokens[0] == "extern")
                {
                    read(tokens, et, 1);
                }
                else
                {
                    read(tokens, sbt, 0);
                }
            }
        }
    }

    int findST(const std::string &identifier, const std::vector<Node> &list)
    {
        for (const Node &i : list)
        {
            if (i.identifier == identifier)
            {
                return i.addr;
            }
        }
        return -1;
    }

    void globalTable()
    {
        int addr;
        for (Node &i : et1)
        {
            addr = findST(i.identifier, sbt2);
            if (addr != -1)
            {
                gvt.push_back(Node(i.identifier, i.dataType, i.size, addr));
            }
        }
        for (Node &i : et2)
        {
            addr = findST(i.identifier, sbt1);
            if (addr != -1)
            {
                gvt.push_back(Node(i.identifier, i.dataType, i.size, addr));
            }
        }
    }

    void display(const std::vector<Node> &list)
    {
        std::cout << "Var \ttype\tsize\taddress" << std::endl;
        for (const Node &i : list)
        {
            std::cout << i.identifier << "\t\t" << i.dataType << "\t" << i.size << "\t\t" << i.addr << std::endl;
        }
        std::cout << std::endl;
    }
};

int Linker::addr = 100;

int main()
{
    Linker linker;
    linker.initialiseSizes();

    std::ifstream file1("test1.c");
    std::ifstream file2("test2.c");

    linker.readFile(file1, 1);
    std::cout << "Symbol Table for file 1" << std::endl;
    linker.display(linker.sbt1);
    std::cout << "Extern Table for file 1" << std::endl;
    linker.display(linker.et1);

    linker.readFile(file2, 2);
    std::cout << "Symbol Table for file 2" << std::endl;
    linker.display(linker.sbt2);
    std::cout << "Extern Table for file 2" << std::endl;
    linker.display(linker.et2);

    if (!linker.et1.empty() || !linker.et2.empty())
    {
        linker.globalTable();
        std::cout << "Global Table" << std::endl;
        linker.display(linker.gvt);
    }
    return 0;
}