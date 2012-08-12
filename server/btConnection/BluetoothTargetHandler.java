import java.io.*;
import javax.bluetooth.*;
import javax.microedition.io.*;

public class BluetoothTargetHandler implements Runnable
{
    // Attributes
    String deviceName;
    RemoteDevice rd;

    /** Creates a new thread handling connection with device */
    public BluetoothTargetHandler(RemoteDevice remote, String name) {
        rd = remote;
        deviceName = name;
        System.out.println("Handling connection with " + deviceName);
    }
    
    public void run() {
        try {
            String url = "btspp://" + rd.getBluetoothAddress() + ":1";
            System.out.println("Trying to reach url: " + url);
            StreamConnection con = (StreamConnection) Connector.open(url);

            System.out.println("Connected to server.");
            OutputStream os = con.openOutputStream();
            InputStream is = con.openInputStream();

            String greeting = "r";
            os.write( greeting.getBytes() );

            String truc = "";
            boolean listening = true;
            int i = 0;
            byte buffer[] = new byte[1024];
            while(i < 8) {
                int bytes_read = is.read( buffer );
                String received = new String(buffer, 0, bytes_read);
                truc = truc + received;
                System.out.println("received: " + received);
                listening = false;
                i++;
            }
            System.out.println(truc);
            
            con.close();
        } catch ( IOException e ) {
            System.err.print(e.toString());
        }
    }
}
