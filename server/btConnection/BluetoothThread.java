import java.io.*;
import javax.microedition.io.*;
import javax.bluetooth.*;

public class BluetoothThread implements DiscoveryListener {
    static int SERVICE_NAME_ATTRID = 0x0100;
    RemoteDevice discovered[] = new RemoteDevice[255];
    int num_discovered;
    public static Thread t;

    public BluetoothThread() {
    }

    public void deviceDiscovered( RemoteDevice rd, DeviceClass cod ) {
        String addr = rd.getBluetoothAddress();
        String name = "";
        try {
            name = rd.getFriendlyName(true);
        } catch( java.io.IOException e ) {}

        this.discovered[this.num_discovered] = rd;
        this.num_discovered++;
        System.out.println("discovered " + addr + " - " + name);
    }

    public void inquiryCompleted( int status ) {
        System.out.println("device inquiry complete");
        synchronized(this) { 
            try { this.notifyAll(); } catch(Exception e) {} 
        }
    }

    public void servicesDiscovered( int transID, ServiceRecord[] rec ){
        for( int i=0; i<rec.length; i++ ) {
            DataElement d = 
                rec[i].getAttributeValue( SERVICE_NAME_ATTRID );
            if ( d != null ) System.out.println((String)d.getValue());
            else System.out.println("unnamed service");
           
            System.out.println(rec[i].getConnectionURL(
                    ServiceRecord.NOAUTHENTICATE_NOENCRYPT, false));
        }
    }

    public void serviceSearchCompleted( int transID, int respCode ) {
        System.out.println("service search complete");
        synchronized(this) { 
            try { this.notifyAll(); } catch(Exception e) {} 
        }
    }

    public void process(String deviceToProcess) {
        System.out.println("Bluetooth RFCOMM client running.");
        try {
            /*
             *Searching for devices and retieving available services
             */
            System.out.println("Getting local configuration...");
            BluetoothThread sb = new BluetoothThread();
            LocalDevice ld = LocalDevice.getLocalDevice();
            DiscoveryAgent da = ld.getDiscoveryAgent();

            // device inquiry
            System.out.println("Inquiring for devices... ");
            sb.num_discovered = 0;
            da.startInquiry( DiscoveryAgent.GIAC, sb );
            synchronized(sb) { 
                try { sb.wait(); } catch( Exception e ) {} 
            }

            // service search
            System.out.println("Quering for services.");
            UUID uuids[] = new UUID[1];
            uuids[0] = new UUID( 0x1002 );
            int attridset[] = new int[1];
            attridset[0] = SERVICE_NAME_ATTRID;

            for( int i=0; i<sb.num_discovered; i++ ) {
                RemoteDevice rd = sb.discovered[i];
                da.searchServices(attridset, uuids, rd, sb);
                synchronized(sb) { 
                    try { sb.wait(); } catch( Exception e ) {} 
                }
                /*
                 *Talking to this device
                 */
                System.out.println("Processing device: " + rd.getFriendlyName(true));
                if ( rd.getFriendlyName(true).equals(deviceToProcess) ) {
                    t = new Thread(new BluetoothTargetHandler(rd, deviceToProcess, "Movement detection"));
					t.start();
                }
            }
        } catch( BluetoothStateException e ) {
            System.out.print(e.toString());
        } catch ( IOException e ) {
            System.err.print(e.toString());
        }
    }
};
