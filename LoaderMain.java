import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

class Loader {
    int size;

    Loader(int size) {
        this.size = size;
    }

    public boolean memoryAvailable(File file) {
        System.out.println("File size: " + file.length());
        if (file.length() < size) {
            size -= (int) file.length();
            System.out.println("Available Memory: " + size);
            return true;
        }
        System.out.println("Insufficient memory");
        return false;
    }
}

public class LoaderMain {
    public static void main(String[] args) throws FileNotFoundException {
        File f1 = new File("test1.c");
        File f2 = new File("test2.c");
        Scanner in = new Scanner(System.in);
        System.out.println("Enter file size: ");
        int size = in.nextInt();

        Loader loader = new Loader(size);
        boolean flag = false;

        if (loader.memoryAvailable(f1)) {
            System.out.println("Memory allocated for file 1.");
            flag = true;
        }
        if (loader.memoryAvailable(f2)) {
            System.out.println("Memory allocated for file 2.");
            flag = true;
        }

        if (!flag) {
            System.out.println("Memory allocation failed for both files.");
        }

        in.close();
    }
}
