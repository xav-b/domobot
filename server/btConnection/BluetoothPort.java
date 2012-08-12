import java.io.*;

public class BluetoothPort 
{
    public static Thread t;

    public static void main(String[] args) 
    {		
        /*
         *Thread handling server bluetooth listening on movement detector
         */
        BluetoothThread bt = new BluetoothThread();
        bt.process("linvor");

        //t = new Thread(new ThreadBtClient());
        //t.start();
	}
}
