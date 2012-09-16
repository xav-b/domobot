import java.awt.*;
import java.awt.event.*;
import TUIO.*;

public class TuioTouchpad implements TuioListener {
	
	private Robot robot = null;
	private int width = 0;
	private int height = 0;
	private long mouse = -1;
	private float handDepth = 0;

	public void addTuioObject(TuioObject tobj) {
		Point pos = MouseInfo.getPointerInfo().getLocation();
		int xpos = pos.x+(int)Math.round(tobj.getXSpeed()*Math.sqrt(width));
		if(xpos<0) xpos=0; if(xpos>width) xpos=width;
		int ypos = pos.y+(int)Math.round(tobj.getYSpeed()*Math.sqrt(height));
		if(ypos<0) ypos=0; if(ypos>height) ypos=height;
		
		long tobjId = tobj.getSymbolID();
		System.out.println("Create Symbol n° " + tobjId);
		if ( mouse<0 && tobjId == 0 ) {
            System.out.println("Creating Mouse controller");
			mouse = tobjId;
			handDepth = tobj.getAngle();
			System.out.println("Cursor creation handDepth: " + handDepth);
			if (robot!=null) robot.mouseMove(xpos,ypos);
		}
		else {
			if (tobjId == 5) {
                System.out.println("Releasing left button");
				if (robot!=null) robot.mouseRelease(InputEvent.BUTTON1_MASK);
			}
			else if (tobjId == 4) {
                System.out.println("Releasing right button");
				if (robot!=null) robot.mouseRelease(InputEvent.BUTTON3_MASK);
			}
		}
	}

	public void updateTuioObject(TuioObject tobj) {
		Point pos = MouseInfo.getPointerInfo().getLocation();
		int xpos = pos.x+(int)Math.round(tobj.getXSpeed()*Math.sqrt(width));
		if(xpos<0) xpos=0; if(xpos>width) xpos=width;
		int ypos = pos.y+(int)Math.round(tobj.getYSpeed()*Math.sqrt(height));
		if(ypos<0) ypos=0; if(ypos>height) ypos=height;

		long tobjId = tobj.getSymbolID();
		float tobjDepth = tobj.getAngle();
		if (tobjId == mouse) {
			if (robot!=null) robot.mouseMove(xpos,ypos);
		} 
		/*else if (tobjId == 1) {
			if ( handDepth - tobjDepth > 0.3) 
				if (robot!=null) robot.mousePress(InputEvent.BUTTON1_MASK);
			else	
				if (robot!=null) robot.mouseRelease(InputEvent.BUTTON1_MASK);
		}*/
	}	

	public void removeTuioObject(TuioObject tobj) {
		long tobjId = tobj.getSymbolID();
		if (tobjId == mouse) {
			mouse = -1;
            System.out.println("Removing mouse control");
		} 
		else if (tobjId == 5) {
                System.out.println("Left click !");
			if (robot!=null) robot.mousePress(InputEvent.BUTTON1_MASK);
		}
		else if (tobjId == 4) {
                System.out.println("Right click !");
			if (robot!=null) robot.mousePress(InputEvent.BUTTON3_MASK);
		}
        else
            System.out.println("Removing Symbol " + tobjId);
		
	}
	
	public TuioTouchpad() {
        //try { robot = new Robot(); }
        //catch (Exception e) {
            //System.out.println("failed to initialize mouse robot");
            //System.exit(0);
        //}
		
		width  = (int)Toolkit.getDefaultToolkit().getScreenSize().getWidth();
		height = (int)Toolkit.getDefaultToolkit().getScreenSize().getHeight();
	}


	public void addTuioCursor(TuioCursor tcur) {}
	public void updateTuioCursor(TuioCursor tcur) {}
	public void removeTuioCursor(TuioCursor tcur) {}
	public void refresh(TuioTime bundleTime) {}


	public static void main(String argv[]) 
    {
		int port = 3333;
 
		if (argv.length==1) {
			try { port = Integer.parseInt(argv[1]); }
			catch (Exception e) { System.out.println("usage: java TuioTouchpad [port]"); }
		}

 		TuioTouchpad trackpad = new TuioTouchpad();
		TuioClient client = new TuioClient(port);

		System.out.println("listening to TUIO messages at port " + port);
		client.addTuioListener(trackpad);
		client.connect();
	}
}
