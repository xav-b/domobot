import java.awt.*;
import java.awt.event.*;
import TUIO.*;

public class TuioMouse implements TuioListener {
	
	private Robot robot = null;
	private int width = 0;
	private int height = 0;
	private long mouse = -1;
	private float handDepth = 0;

	public void addTuioObject(TuioObject tobj) {
		long tobjId = tobj.getSymbolID();
		if (mouse<0) {
			mouse = tobjId;
			handDepth = tobj.getAngle();
			System.out.println("handDepth " + handDepth);
		if (robot!=null) robot.mouseMove(tobj.getScreenX(width),tobj.getScreenY(height));
		
	}
}
	public void updateTuioObject(TuioObject tobj) {
		long tobjId = tobj.getSymbolID();
		float tobjDepth = tobj.getAngle();
		if (tobjId == mouse) {
			if (robot!=null) robot.mouseMove(tobj.getScreenX(width),tobj.getScreenY(height));
		} 
	}	

	public void removeTuioObject(TuioObject tobj) {
		if (mouse==tobj.getSessionID()) {
			mouse=-1;
		}
	}

	public void refresh(TuioTime bundleTime) {}
	
	public void addTuioCursor(TuioCursor tcur) {}
	public void updateTuioCursor(TuioCursor tcur) {}
	public void removeTuioCursor(TuioCursor tcur) {}
	
	public TuioMouse() {
		try { robot = new Robot(); }
		catch (Exception e) {
			System.out.println("failed to initialize mouse robot");
			System.exit(0);
		}
		
		width  = (int)Toolkit.getDefaultToolkit().getScreenSize().getWidth();
		height = (int)Toolkit.getDefaultToolkit().getScreenSize().getHeight();
	}

	public static void main(String argv[]) {
	
		int port = 3333;
 
		if (argv.length==1) {
			try { port = Integer.parseInt(argv[1]); }
			catch (Exception e) { System.out.println("usage: java TuioMouse [port]"); }
		}

 		TuioMouse mouse = new TuioMouse();
		TuioClient client = new TuioClient(port);

		System.out.println("listening to TUIO messages at port "+port);
		client.addTuioListener(mouse);
		client.connect();
	}
}
