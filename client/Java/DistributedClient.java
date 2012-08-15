import java.net.*;
import java.util.Scanner;
import java.io.*;

public class DistributedClient implements Runnable 
{
	private Socket socket = null;
	public static String identity = null;
	private PrintWriter out = null;
	private BufferedReader in = null;
	private Scanner sc = null;
	private boolean connect = false;
	
	public DistributedClient(Socket s)
    {
		socket = s;
	}
	
	public void run() {
		try {
            out = new PrintWriter(socket.getOutputStream());
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));	
            //sc = new Scanner(System.in);
		
            while(!connect ){
                // Send a code for identity authentification (or command)
                //System.out.println(in.readLine());
                //identity = sc.nextLine();
                identity = "test";
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
			Thread t1 = new Thread(new Emission(out));
            t1.start();
            Thread t2 = new Thread(new Reception(in));
            t2.start();
		} catch (IOException e) {	
			System.err.println("[CLIENT] ** No server responding ");
		}
	}
}

