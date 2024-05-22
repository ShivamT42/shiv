import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

class Macro {
    int indexMNT = 1;
    int mdt = 4;
    ArrayList<NodeMNT> mntList = new ArrayList<>();
    ArrayList<NodeMDT> mdtList = new ArrayList<>();
    ArrayList<NodeActPar> actparList = new ArrayList<>();
    ArrayList<NodeForPar> forparList = new ArrayList<>();

    class NodeMNT {
        int index;
        String name;
        int mdt;

        NodeMNT(int index, String name, int mdt) {
            this.index = indexMNT;
            this.name = name;
            this.mdt = mdt;
        }
    }

    public void insertMDT(String card) {
        NodeMDT node = new NodeMDT(mdt, card);
        mdtList.add(node);
        mdt++;
    }

    public void insertMNT(String name) {
        NodeMNT node = new NodeMNT(indexMNT, name, mdt);
        mntList.add(node);
        indexMNT++;
    }

    public void insertFOR(String name, String formal, String position) {
        NodeForPar node = new NodeForPar(name, formal, position);
        forparList.add(node);
    }

    public void insertACT(String name, String actual, int position) {
        NodeActPar node = new NodeActPar(name, actual, position + "");
        actparList.add(node);
    }

    public String findPos(String arg) {
        for (NodeForPar i : forparList) {
            if (i.formal.equals(arg)) {
                return i.position;
            }
        }
        return null;
    }

    class NodeMDT {
        int mdt;
        String card;

        NodeMDT(int mdt, String card) {
            this.mdt = mdt;
            this.card = card;
        }
    }

    class NodeActPar {
        String name;
        String actual;
        String position;
        boolean flag = true;

        NodeActPar(String name, String actual, String position) {
            this.name = name;
            this.actual = actual;
            this.position = position;
        }
    }

    class NodeForPar {
        String name;
        String formal;
        String position;

        NodeForPar(String name, String formal, String position) {
            this.name = name;
            this.formal = formal;
            this.position = position;
        }
    }
}

public class ArgumentArrayPrinter {
    public static void main(String[] args) throws IOException {
        File myObj = new File("input3.txt");
        Scanner myReader = new Scanner(myObj);
        Macro macro = new Macro();
        while (myReader.hasNextLine()) {
            String data = myReader.nextLine();
            if (data.startsWith("macro")) {
                String[] tokens = data.split("[ ,]");
                String macroName = tokens[1];
                macro.insertMNT(macroName);
                for (int i = 2; i < tokens.length; i++) {
                    macro.insertFOR(macroName, tokens[i], i - 1 + "");
                }
                data = myReader.nextLine();
                while (!data.startsWith("mend")) {
                    tokens = data.split("[ ,]");
                    for (int i = 1; i < tokens.length; i++) {
                        String pos = macro.findPos(tokens[i]);
                        if (pos != null) {
                            tokens[i] = "#" + pos;
                        }
                    }
                    String card = "";
                    for (String i : tokens) {
                        card += (i + " ");
                    }
                    macro.insertMDT(card);
                    data = myReader.nextLine();
                }
                macro.insertMDT(data.trim());
            }
            if (data.startsWith(".code")) {
                while (myReader.hasNextLine()) {
                    data = myReader.nextLine();
                    String[] tokens = data.split("[ ,]");
                    String macroName = tokens[0];
                    for (int i = 1; i < tokens.length; i++) {
                        macro.insertACT(macroName, tokens[i], i);
                    }
                }
            }
        }
        myReader.close();

        System.out.println("\t\t\t\tArgument List Array");
        System.out.println("macroName\t\tformalParameter\t\tpositionalParameter");
        for (Macro.NodeForPar i : macro.forparList) {
            System.out.println(i.name + "\t\t\t" + i.formal + "\t\t\t\t#" + i.position);
        }
        System.out.println("macroName\t\tactualParameter\t\tpositionalParameter");
        for (Macro.NodeActPar i : macro.actparList) {
            System.out.println(i.name + "\t\t\t" + i.actual + "\t\t\t\t#" + i.position);
        }
    }
}
