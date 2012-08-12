import java.net.*;
import java.util.Scanner;
import java.io.*;

public class Connection implements Runnable 
{
	private Socket socket = null;
	public static Thread t2;
	public static String identity = null;
	private PrintWriter out = null;
	private BufferedReader in = null;
	private Scanner sc = null;
	private boolean connect = false;
	
	public Connection(Socket s)
    {
		socket = s;
	}
	
	public void run() {
		try {
            out = new PrintWriter(socket.getOutputStream());
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));	
            sc = new Scanner(System.in);
		
            while(!connect ){
                // Send a code for identity authentification (or command)
                System.out.println(in.readLine());
                identity = sc.nextLine();
                out.println(identity);
                out.flush();
                
                if(in.readLine().equals("ok")){
                    System.out.println("[CLIENT] Access granted, connected."); 
                    connect = true;
                }
		        else {
			        System.err.println("[CLIENT] ** Access denied"); 
		        }
		    }
			t2 = new Thread(new DistributedStream(socket));
			t2.start();
		} catch (IOException e) {	
			System.err.println("[CLIENT] ** No server responding ");
		}
	}
}

