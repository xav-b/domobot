import java.io.*;
import javax.bluetooth.*;
import javax.microedition.io.*;

public class BluetoothTargetHandler implements Runnable
{
    // Attributes
    String deviceName;
    RemoteDevice rd;

    /** Creates a new thread handling connection with device */
    public BluetoothTargetHandler(RemoteDevice remote, String name, String threadName) 
    {
        rd = remote;
        deviceName = name;
        System.out.println("Handling connection with " + deviceName);
        System.out.println("Starting a new thread (" + Thread.currentThread().getName() + ")");
        System.out.println("\tCurrent state: " + Thread.currentThread().getState());
    }

    public void run() 
    {
        try {
            String url = "btspp://" + rd.getBluetoothAddress() + ":1";
            System.out.println("Trying to reach url: " + url);
            StreamConnection con = (StreamConnection) Connector.open(url);

            System.out.println("Connected to server.");

            PrintWriter out = new PrintWriter(con.openOutputStream());
            BufferedReader in = new BufferedReader(new InputStreamReader(con.openInputStream()));

            /*
             * Report quering
             */
            out.write('r');
            out.flush();
            int i = 0;
            while (in.ready() || i < 2) {
                String truc = in.readLine();
                System.out.println("Answer: " + truc);
                i++;
            }
            
            out.close();
            in.close();
            con.close();
        } catch ( IOException e ) {
            System.err.print(e.toString());
        }
    }
}
