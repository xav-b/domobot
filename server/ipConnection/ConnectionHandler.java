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
		try {
			while(true) {
                socket = socketserver.accept();
                System.out.println("[SERVER] Client connection request");
                
                t1 = new Thread(new Authentification(socket));
                t1.start();
			}
		} catch (IOException e) {
			System.err.println("[SERVER] ** Erreur serveur");
		}
	}
}

