import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class MacroExpander {
    List<NodeMNT> mntList;
    List<NodeMDT> mdtList;

    public MacroExpander() {
        mntList = new ArrayList<>();
        mdtList = new ArrayList<>();
    }

    public void readMNTAndMDT() throws IOException {
        Scanner mntReader = new Scanner(new File("mnt.txt"));
        System.out.println("Reading MNT:");
        while (mntReader.hasNextLine()) {
            String line = mntReader.nextLine().trim();
            if (!line.isEmpty()) {
                String[] mntData = line.split("\\s+");
                if (mntData.length == 3) {
                    mntList.add(new NodeMNT(Integer.parseInt(mntData[0]), mntData[1], Integer.parseInt(mntData[2])));
                    System.out.println("Read MNT: " + line);
                } else {
                    System.err.println("Invalid MNT line: " + line);
                }
            }
        }
        mntReader.close();

        Scanner mdtReader = new Scanner(new File("mdt.txt"));
        System.out.println("Reading MDT:");
        while (mdtReader.hasNextLine()) {
            String line = mdtReader.nextLine().trim();
            if (!line.isEmpty()) {
                String[] mdtData = line.split("\\s+", 2);
                if (mdtData.length == 2) {
                    mdtList.add(new NodeMDT(Integer.parseInt(mdtData[0]), mdtData[1]));
                    System.out.println("Read MDT: " + line);
                } else {
                    System.err.println("Invalid MDT line: " + line);
                }
            }
        }
        mdtReader.close();
    }

    public void expandMacro(String name) throws IOException {
        int index = -1;
        FileWriter fw = new FileWriter("output1.txt", true);
        for (NodeMNT node : mntList) {
            if (node.name.equals(name)) {
                index = node.mdt;
                break;
            }
        }
        if (index == -1) {
            fw.write(name + "\n");
        } else {
            for (NodeMDT node : mdtList) {
                if (node.mdt >= index) {
                    if (node.card.equals("mend")) break;
                    fw.write(node.card + "\n");
                }
            }
        }
        fw.flush();
        fw.close();
    }

    private class NodeMDT {
        int mdt;
        String card;

        NodeMDT(int mdt, String card) {
            this.mdt = mdt;
            this.card = card;
        }
    }

    private class NodeMNT {
        String name;
        int index;
        int mdt;

        NodeMNT(int index, String name, int mdt) {
            this.index = index;
            this.name = name;
            this.mdt = mdt;
        }
    }
}

public class MacroMain {
    public static void main(String[] args) throws IOException {
        MacroExpander macroExpander = new MacroExpander();
        macroExpander.readMNTAndMDT();

        File codeFile = new File("code.txt");
        Scanner codeReader = new Scanner(codeFile);
        System.out.println("Expanding macros in code:");
        while (codeReader.hasNextLine()) {
            String data = codeReader.nextLine().trim();
            if (!data.isEmpty()) {
                System.out.println("Expanding macro: " + data);
                macroExpander.expandMacro(data);
            }
        }
        codeReader.close();
        System.out.println("Macro expansion complete. Check output.txt for results.");
    }
}
