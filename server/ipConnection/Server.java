import java.io.*;
import java.net.*;

public class Server 
{
    public static int port = 2009;
    public static ServerSocket ss = null;
    public static Thread t;

    public static void main(String[] args) 
    {		
        /*
         *Thread handling server disponibility
         */
	    try {
		    ss = new ServerSocket(port);
			System.out.println("[SERVER] Server listening on port " + ss.getLocalPort());
			t = new Thread(new ConnectionHandler(ss));
			t.start();
			
		} catch (IOException e) {
			System.err.println("[SERVER] ** Port " + ss.getLocalPort() + " busy.");
		}
	}
}
