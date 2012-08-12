import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;


public class DistributedExchange implements Runnable 
{
	private Socket socket = null;
	private BufferedReader in = null;
	private PrintWriter out = null;
	private String identity = "zero";
	private Thread t3, t4;	
	
	public DistributedExchange(Socket s, String log)
    {
		socket = s;
		identity = log;
	}
	
    public void run() 
    {	
		try {
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            out = new PrintWriter(socket.getOutputStream());
            
            Thread t3 = new Thread(new Reception(in,identity));
            t3.start();
            Thread t4 = new Thread(new Emission(out));
            t4.start();
		} catch (IOException e) {
			System.err.println("[SERVER] ** " + identity + " no longer connected to server");
		}
    }
}

