import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Macro {
    List<NodeMNT> mntList;
    int mdtIndex;
    List<NodeMDT> mdtList;

    public Macro() {
        mntList = new ArrayList<>();
        mdtIndex = 21;  // Starting MDT index
        mdtList = new ArrayList<>();
    }

    public void insertMNT(String name) {
        mntList.add(new NodeMNT(mntList.size() + 1, name, mdtIndex));
    }

    public void insertMDT(String card) {
        mdtList.add(new NodeMDT(mdtIndex++, card));
    }

    public void displayMNT() {
        System.out.println("MNT: ");
        System.out.println("index   name    MDT index");
        for (NodeMNT node : mntList) {
            System.out.println(node.index + "       " + node.name + "         " + node.mdt);
        }
    }

    public void displayMDT() {
        System.out.println("MDT:");
        System.out.println("index   card");
        for (NodeMDT node : mdtList) {
            System.out.println(node.mdt + "      " + node.card);
        }
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

public class Macro1 {
    public static void main(String[] args) throws IOException {
        File myObj = new File("input4.txt");
        Scanner myReader = new Scanner(myObj);
        Macro macro = new Macro();

        while (myReader.hasNextLine()) {
            String data = myReader.nextLine();
            if (data.startsWith("macro")) {
                String[] m = data.split(" ");
                macro.insertMNT(m[1]);
                data = myReader.nextLine();
                while (!data.equals("mend")) {
                    macro.insertMDT(data.trim());
                    data = myReader.nextLine();
                }
                macro.insertMDT(data.trim()); // mend
            }
            // Removed the code section processing part
        }
        macro.displayMNT();
        macro.displayMDT();
        myReader.close();
    }
}
