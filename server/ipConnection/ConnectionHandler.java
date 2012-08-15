import java.io.*;
import java.net.*;

public class ConnectionHandler implements Runnable
{
	private ServerSocket socketserver = null;
	private Socket socket = null;

	public Thread t1;
	public ConnectionHandler(ServerSocket ss)
    {
	    socketserver = ss;
	}
	
	public void run() 
    {		
        boolean listening = true;
		try {
			while(listening) {
                socket = socketserver.accept();
                System.out.println("[SERVER] Client connection request");
                
                t1 = new Thread(new DistributedServer(socket));
                t1.start();
                listening = false;
			}
		} catch (IOException e) {
			System.err.println("[SERVER] ** Erreur serveur");
		}
	}
}
