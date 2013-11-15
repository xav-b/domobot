package voice.commands;

//import java.net.*;
//import java.util.Scanner;
//import java.io.*;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * ShellProcess allows User to run external process
 * managing input and ouputs streams in separated threads
 *
 * @author Xavier Bruhiere
 */
public class ShellProcess 
{
	private OutputStream out = null;
	private OutputStream err = null;
	private InputStream in = null;
	private Process process;
	private long timeout = 0L;
	private boolean finished = false;

	/** Constructors */
	public ShellProcess() {
		this(null, null, null, 0L);
	}

	public ShellProcess(OutputStream out, OutputStream err) {
		this(out, err, null, 0L);
	}

	public ShellProcess(OutputStream out, OutputStream err, InputStream in) {
		this(out, err, in, 0L);
	}

	public ShellProcess(OutputStream out, OutputStream err, long timeout) {
		this(out, err, null, timeout);
	}
	
	 /** @param out Outputstream vers lequel sera redirige la sortie standard (null pour ne pas rediriger).
	 *   @param err	Outputstream vers lequel sera redirige la sortie d'erreur
	 *   @param in InputStream vers lequel sera redirige l'entree standard
	 *   @param timeout	temps en millisecondes avant de forcer l'arret (0 pour ne jamais forcer l'arret).*/  	
	public ShellProcess(OutputStream out, OutputStream err, InputStream in, long timeout) {
		this.out = out;
		this.err = err;
		this.in = in;
		this.timeout = timeout < 0 ? 0L : timeout;
	}

	public int exec(String command) throws IOException {
		process = Runtime.getRuntime().exec(command);
		return execute();
	}
	public int exec(String[] cmdarray) throws IOException {
		process = Runtime.getRuntime().exec(cmdarray);
		return execute();
	}
	public int exec(String[] cmdarray, String[] envp) throws IOException {
		process = Runtime.getRuntime().exec(cmdarray, envp);
		return execute();
	}	
	public int exec(String[] cmdarray, String[] envp, File dir) throws IOException {
		process = Runtime.getRuntime().exec(cmdarray, envp, dir);
		return execute();
	}
	public int exec(String command, String[] envp) throws IOException {
		process = Runtime.getRuntime().exec(command, envp);
		return execute();
	}	
	/**
	 * Execute une ligne de commande dans un processus separe en specifiant des 
	 *    variables d'environnement et le repertoire de travail.
	 *    	 
	 * @param command ligne de commande
	 * @param envp variables d'environnement   	 
	 * @param dir repertoire de travail	 
	 * @return valeur de retour du processus
	 */   	
	public int exec(String command, String[] envp, File dir) throws IOException  {
		process = Runtime.getRuntime().exec(command, envp, dir);
		return execute();
	}

	/** So told execution part */
    private int execute() throws IOException {
		int status = -1;
        
		// Consommation des fluxs de sortie standard et d'erreur dans des threads separes. 
        if(err == null) {
            process.getErrorStream().close();
        } else {
            createStreamThread(process.getErrorStream(), err);
        }
        if(out == null) {
            process.getInputStream().close();
        } else {
            createStreamThread(process.getInputStream(), out);
        }
		
		
		// Mapping de l'entree standard de l'application si besoin est.
		if(in != null) {
			createStreamThread(in, process.getOutputStream());
		}
		
		if (timeout > 0L) {
			Thread processThread = createProcessThread(process);
			processThread.start();
			try {
				processThread.join(timeout);
				try {
					status = process.exitValue();
				} catch (IllegalThreadStateException itse) {
					process.destroy();
					status = process.exitValue();
				}
			} catch (InterruptedException ie) {
				ie.printStackTrace();
			}
		} else if (timeout == 0L) {
			try {
				status = process.waitFor();
			} catch (InterruptedException ie) {
				ie.printStackTrace();
			}
		}
		finished = true;
		return status;
	}
	
	private void createStreamThread(final InputStream is, final OutputStream os) {
		new Thread(new Runnable() {
			public void run() {
				BufferedInputStream bis = new BufferedInputStream(is);
				BufferedOutputStream bos = null;
				if(os != null) {
					bos = new BufferedOutputStream(os);
				}
				byte[] data = new byte[2048];
				int nbRead = 0;
				try {
					while((nbRead = bis.read(data)) > 0) {
						if(bos != null) {
							if(finished) {
						    	break;
						  	}
							bos.write(data, 0, nbRead);
							bos.flush();
						}
					}
				} catch(IOException ioe) {
					ioe.printStackTrace();
				}
			}
		}).start();
	}
	
    private Thread createProcessThread(final Process process) {
        return new Thread() {
            public void run() {
                try {
                    process.waitFor();
                } catch(InterruptedException ie) {
                    ie.printStackTrace();
                }
            }
        };
    }
	
	/** Accesseurs */	 
	public OutputStream getErrorStream() {
		return err;
	}  	 
	public InputStream getInputStream() {
		return in;
	} 
	public OutputStream getOutputStream() {
		return out;
	} 
	public long getTimeout() {
		return timeout;
	}	
	public void setErrorStream(OutputStream err) {
		this.err = err;
	}
	public void setInputStream(InputStream in) {
		this.in = in;
	}
	public void setOutputStream(OutputStream out) {
		this.out = out;
	}
	public void setTimeout(long timeout) {
		this.timeout = timeout;
	}

    /** Others */
    public boolean isFinished() {
	    return finished;
    }
}
