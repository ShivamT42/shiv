import java.io.FileWriter;
import java.io.IOException;
import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

class AssemblerSecondPass {
    HashMap<String, Integer> symbolTable = new HashMap<>();
    HashMap<String, Integer> pot = new HashMap<>();
    int locationCounter = 0;

    public void initializePOT() {
        pot.put("db", 1);
        pot.put("dw", 1);
        pot.put("org", 1);
        pot.put("endp", 0);
        pot.put("const", 1);
        pot.put("end", 0);
    }

    public void secondPass() throws IOException {
        File inputFile = new File("input1.txt");
        Scanner in = new Scanner(inputFile);
        FileWriter fw = new FileWriter("output2.txt");

        while (in.hasNextLine()) {
            String data = in.nextLine();
            if (data.contains("end")) {
                break;
            }
            String[] tokens = data.split("\\s+");
            symbolTable.put(tokens[2], locationCounter);
            locationCounter += pot.getOrDefault(tokens[3], 1); // Default to 1 if not found in POT
        }

        System.out.println("Symbol Table in the second pass:");
        displaySBT();

        in.close();
        fw.close();
    }

    public void displaySBT() {
        for (Map.Entry<String, Integer> mapElement : symbolTable.entrySet()) {
            System.out.println(mapElement.getKey().trim() + " -> " + mapElement.getValue());
        }
    }

    public static void main(String[] args) throws IOException {
        AssemblerSecondPass assembler = new AssemblerSecondPass();
        assembler.initializePOT();
        assembler.secondPass();
    }
}
