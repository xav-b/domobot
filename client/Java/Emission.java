import java.io.*;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.NoSuchElementException;


public class Emission implements Runnable 
{
	private PrintWriter out;
	private String login = null, message = null;
	private Scanner sc = null;
	
	public Emission(PrintWriter out) {
		this.out = out;
	}

	public void run() 
    {
        boolean listening = true;
		sc = new Scanner(System.in);
        //String message = 
            //"{"
            //+ "'exec' : '../kinect/Tracker/KinectTuio',"
            //+ "'id' : 2"
            //+ "}";
		
	    while(listening){
            //System.out.print("[Client] exec: ");
            //String exec = sc.nextLine();
            //System.out.print("[Client] process: ");
            //String process = sc.nextLine();
            //System.out.print("[Client] wd: ");
            //String wd = sc.nextLine();
            //String message = 
                //"{'exec' : '"
                //+ exec
                //+ "', 'process' : '"
                //+ process
                //+ "', 'wd' : '"
                //+ wd
                //+ "' }";
            String message = getCommand("command.json");
            System.out.println("Message: " + message);
            out.println(message);
            out.flush();
            message = sc.nextLine();
            out.println(message);
            out.flush();
            //if ( process.equals("end") )
            listening = false;
		}
        System.out.println("Ending conversation.");
	}

    private static String getCommand(String cmdFile)
    {
        String message = "";
        try {
            Scanner sc = new Scanner(new File(cmdFile));
            while ( sc.hasNext() )
                message += sc.nextLine();
        } catch(FileNotFoundException e) {
            System.err.println("[Client] ** Command file not found");
        }
        return message;
    }
}
