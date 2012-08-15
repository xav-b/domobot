import java.io.BufferedReader;
import java.io.IOException;


public class Reception implements Runnable 
{
	private BufferedReader in;
	private String message = null;
	
	public Reception(BufferedReader in) {		
		this.in = in;
	}
	
	public void run() 
    {	
        boolean listening = true;
		while(listening){
	        try 
            {
			    message = in.readLine();
			    System.out.println("Server > " + message);
                if ( message.equals("ended") )
			        listening = false;
		    } catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}

