import java.io.*;
import java.net.*;

public class ProcessThread implements Runnable
{
    private OutputStream stderr = System.out;
    private OutputStream stdout = System.out;
    private InputStream stdin = System.in;
    private String workingDir;
    private String[] process = {""};
    private String[] env = {""};
    private int timeout = 0;

    public ProcessThread(String prog, String wd) {
        this.workingDir = wd;
        this.process[0] = prog;
    }

    /*
     *Thread handling server disponibility
     */
    public void run() 
    {		
        File dir = new File(workingDir);
	    try {
			System.out.println("Running process: " + process[0]);
			ShellProcess Kinect = new ShellProcess(stdout, stderr, stdin, timeout);
            int valR = Kinect.exec(process, env, dir);
            if ( Kinect.isFinished() ) {
                System.out.println("Finished !");
                System.out.println("Return value: " + valR);
            }
		} catch(IOException e) {
            System.err.println("** Error running external process " + process[0]);
        }
    }
}
