import java.io.*;
import java.net.*;

public class Client 
{
    private static int clientPort = 2009;
    private static String clientIp = "127.0.0.1";
	public static Socket socket = null;
	public static Thread t1;
	
	public static void main(String[] args) 
    {
	    try 
        {	
            System.out.println("[CLIENT] Connection request");
            socket = new Socket(clientIp, clientPort);
            //System.out.println("Connected to server, authentification :");
            
            t1 = new Thread(new Connection(socket));
            t1.start();	
        } catch (UnknownHostException e) {
            System.err.println("[CLIENT] ** Unable to connect: "+socket.getLocalAddress());
	    } catch (IOException e) {
	        System.err.println("[CLIENT] ** No listening server on port "+socket.getLocalPort());
	    }
	}
}
