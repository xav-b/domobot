/*
    Scene Processing Library 1.0
    http://scene.sourceforge.net/
 
    Copyright (c) 2011 Laurence Bender <lbender@untref.edu.ar>
 
    Based on the TUIO Processing Library - part of the reacTIVision project
    http://reactivision.sourceforge.net/
    Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

package scene.tuio;

import java.awt.event.*;
import java.lang.reflect.*;
import processing.core.*;
import java.util.*;
import TUIO.*;

/**
 * The Processing wrapper class for the Scene Java Library.  
 * 
 * @author Laurence Bender (based on the TUIO Processing Library by Martin Kaltenbrunner)
 * @version 1.0
 */ 

public class Scene implements SceneTuioListener {
		
	private PApplet parent;
	private Method addTuioObject, removeTuioObject, updateTuioObject; 
    private Method addTuioCursor, removeTuioCursor, updateTuioCursor;
	private Method addSceneTuioBlob, removeSceneTuioBlob, updateSceneTuioBlob, refresh;
	private Method addSceneBlob, removeSceneBlob, updateSceneBlob;
	private SceneTuioClient client;
	
	/**
	 * This constructor takes a PApplet argument and creates a client 
	 * that listens to the default TUIO port 3333.
	 *
	 * @param	parent	the PApplet to assign
	 */ 		
	public Scene(PApplet parent) {
	
		this(parent,3333);
		
	}
	
	/**
	 * This constructor takes a PApplet argument and creates a client that listens 
	 * to the provided TUIO port.
	 *
	 * @param	parent	the PApplet to assign
	 * @param	port	the TUIO port to assign
	 */
	public Scene(PApplet parent, int port) {
	
		this.parent = parent;
		parent.registerDispose(this);
		
		try { refresh = parent.getClass().getMethod("refresh",new Class[] { TuioTime.class } ); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'refresh(TuioTime bundleTime)' method implementation");
			refresh = null;
		}
		
		try { addTuioObject = parent.getClass().getMethod("addTuioObject", new Class[] { TuioObject.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'addTuioObject(TuioObject tobj)' method implementation");
			addTuioObject = null;
		}
		
		try { removeTuioObject = parent.getClass().getMethod("removeTuioObject", new Class[] { TuioObject.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'removeTuioObject(TuioObject tobj)' method implementation");
			removeTuioObject = null;
		}
		
		try { updateTuioObject = parent.getClass().getMethod("updateTuioObject", new Class[] { TuioObject.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'updateTuioObject(TuioObject tobj)' method implementation");
			updateTuioObject = null;
		}
		
		try { addTuioCursor = parent.getClass().getMethod("addTuioCursor", new Class[] { TuioCursor.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'addTuioCursor(TuioCursor tcur)' method implementation");
			addTuioCursor = null;
		}
		
		try { removeTuioCursor = parent.getClass().getMethod("removeTuioCursor", new Class[] { TuioCursor.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO:missing or wrong 'removeTuioCursor(TuioCursor tcur)' method implementation");
			removeTuioCursor = null;
		}
		
		try { updateTuioCursor = parent.getClass().getMethod("updateTuioCursor", new Class[] { TuioCursor.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'updateTuioCursor(TuioCursor tcur)' method implementation");
			updateTuioCursor = null;
		}
		
		try { addSceneTuioBlob = parent.getClass().getMethod("addSceneTuioBlob", new Class[] { SceneTuioBlob.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'addSceneTuioBlob(SceneTuioBlob tblb)' method implementation");
			addSceneTuioBlob = null;
		}
		
		try { removeSceneTuioBlob = parent.getClass().getMethod("removeSceneTuioBlob", new Class[] { SceneTuioBlob.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'removeSceneTuioBlob(SceneTuioBlob tblb)' method implementation");
			removeSceneTuioBlob = null;
		}
		
		try { updateSceneTuioBlob = parent.getClass().getMethod("updateSceneTuioBlob", new Class[] { SceneTuioBlob.class }); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'updateSceneTuioBlob(SceneTuioBlob tblb)' method implementation");
			updateSceneTuioBlob = null;
		}
        
        try { refresh = parent.getClass().getMethod("refresh",new Class[] { TuioTime.class } ); }
		catch (Exception e) { 
			//System.out.println("TUIO: missing or wrong 'refresh(TuioTime bundleTime)' method implementation");
			refresh = null;
		}
		
		try { addSceneBlob = parent.getClass().getMethod("addSceneBlob", new Class[] { SceneBlob.class }); }
		catch (Exception e) { 
			//System.out.println("Scene TUIO: missing or wrong 'addSceneBlob(SceneBlob tblb)' method implementation");
			addSceneBlob = null;
		}
		
		try { removeSceneBlob = parent.getClass().getMethod("removeSceneBlob", new Class[] { SceneBlob.class }); }
		catch (Exception e) { 
			//System.out.println("Scene TUIO: missing or wrong 'removeSceneBlob(SceneBlob tblb)' method implementation");
			removeSceneBlob = null;
		}
		
		try { updateSceneBlob = parent.getClass().getMethod("updateSceneBlob", new Class[] { SceneBlob.class }); }
		catch (Exception e) { 
			//System.out.println("Scene TUIO: missing or wrong 'updateSceneBlob(SceneBlob tblb)' method implementation");
			updateSceneBlob = null;
		}
		
		client = new SceneTuioClient(port);
		client.addSceneTuioListener(this);
		client.connect();
		
	}
	
	public void addTuioObject(TuioObject tobj) {
        
		if (addTuioObject!=null) {
			try { 
				addTuioObject.invoke(parent, new Object[] { tobj });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
        
	}
	
	public void updateTuioObject(TuioObject tobj) {
		
		if (updateTuioObject!=null) {
			try { 
				updateTuioObject.invoke(parent, new Object[] { tobj });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
	}
	
	public void removeTuioObject(TuioObject tobj) {
        
		if (removeTuioObject!=null) {
			try { 
				removeTuioObject.invoke(parent, new Object[] { tobj });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
        
	}
	
	public void addTuioCursor(TuioCursor tcur) {
        
		if (addTuioCursor!=null) {
			try { 
				addTuioCursor.invoke(parent, new Object[] { tcur });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
        
	}
	
	public void updateTuioCursor(TuioCursor tcur) {
        
		if (updateTuioCursor!=null) {
			try { 
				updateTuioCursor.invoke(parent, new Object[] { tcur });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
        
	}
	
	public void removeTuioCursor(TuioCursor tcur) {
        
		if (removeTuioCursor!=null) {
			try { 
				removeTuioCursor.invoke(parent, new Object[] { tcur });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
        
	}

	public void addSceneTuioBlob(SceneTuioBlob tblb) {
	
		if (addSceneTuioBlob!=null) {
			try { 
				addSceneTuioBlob.invoke(parent, new Object[] { tblb });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	public void updateSceneTuioBlob(SceneTuioBlob tblb) {
		
		if (updateSceneTuioBlob!=null) {
			try { 
				updateSceneTuioBlob.invoke(parent, new Object[] { tblb });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	public void removeSceneTuioBlob(SceneTuioBlob tblb) {
	
		if (removeSceneTuioBlob!=null) {
			try { 
				removeSceneTuioBlob.invoke(parent, new Object[] { tblb });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	public void refresh(TuioTime bundleTime) {
	
		if (refresh!=null) {
			try { 
				refresh.invoke(parent,new Object[] { bundleTime });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	public void addSceneBlob(SceneBlob tblb) {
	
		if (addSceneBlob!=null) {
			try { 
				addSceneBlob.invoke(parent, new Object[] { tblb });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	public void updateSceneBlob(SceneBlob tblb) {
		
		if (updateSceneBlob!=null) {
			try { 
				updateSceneBlob.invoke(parent, new Object[] { tblb });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	public void removeSceneBlob(SceneBlob tblb) {
	
		if (removeSceneBlob!=null) {
			try { 
				removeSceneBlob.invoke(parent, new Object[] { tblb });
			}
			catch (IllegalAccessException e) {}
			catch (IllegalArgumentException e) {}
			catch (InvocationTargetException e) {}
		}
		
	}
	
	/**
	 * Returns a vector of active TuioObjects.
	 * @return	a vector of active TuioObjects
	 */
	public Vector getTuioObjects() {
	
		return client.getTuioObjects();
		
	}
	
	/**
	 * Returns a vector of active TuioCursors.
	 * @return	a vector of active TuioCursors
	 */
	public Vector getTuioCursors() {
	
		return client.getTuioCursors();
		
	}
    
	/**
	 * Returns a vector of active SceneTuioBlobs.
	 * @return	a vector of active SceneTuioBlobs
	 */
    public Vector getSceneTuioBlobs() {
	
		return client.getSceneTuioBlobs();
		
	}
	
	/**
	 * Returns a vector of active SceneBlobs.
	 * @return	a vector of active SceneBlobs
	 */
	public Vector getSceneBlobs() {
	
		return client.getSceneBlobs();
		
	}
	
	/**
	 * Returns the TuioObject corresponding to the provided Session ID
	 * or NULL if the Session ID does not refer to an active TuioObject.
	 *
	 * @parameter	the Session ID of the TuioObject
	 * @return  an active TuioObject corresponding to the provided Session ID or NULL
	 */
	public TuioObject getTuioObject(long s_id) {
	
		return client.getTuioObject(s_id);
		
	}
	
	/**
	 * Returns the TuioCursor corresponding to the provided Session ID
	 * or NULL if the Session ID does not refer to an active TuioObject.
	 *
	 * @parameter	the Session ID of the TuioCursor
	 * @return  an active TuioCursor corresponding to the provided Session ID or NULL
	 */
	public TuioCursor getTuioCursor(long s_id) {
	
		return client.getTuioCursor(s_id);
		
	}	
	
	/**
	 * Returns the SceneTuioBlob corresponding to the provided Session ID
	 * or NULL if the Session ID does not refer to an active SceneTuioBlob.
	 *
	 * @parameter	the Session ID of the SceneTuioBlob
	 * @return  an active SceneTuioBlob corresponding to the provided Session ID or NULL
	 */
	public SceneTuioBlob getSceneTuioBlob(long s_id) {
	
		return client.getSceneTuioBlob(s_id);
		
	}
	
	/**
	 * Returns the SceneBlob corresponding to the provided Session ID
	 * or NULL if the Session ID does not refer to an active SceneBlob.
	 *
	 * @parameter	the Session ID of the SceneBlob
	 * @return  an active SceneBlob corresponding to the provided Session ID or NULL
	 */
	public SceneBlob getSceneBlob(long s_id) {
	
		return client.getSceneBlob(s_id);
		
	}
		
	public void pre() {
		//method that's called just after beginFrame(), meaning that it 
		//can affect drawing.
	}

	public void draw() {
		//method that's called at the end of draw(), but before endFrame().
	}
	
	public void mouseEvent(MouseEvent e) {
		//called when a mouse event occurs in the parent applet
	}
	
	public void keyEvent(KeyEvent e) {
		//called when a key event occurs in the parent applet
	}
	
	public void post() {
		//method called after draw has completed and the frame is done.
		//no drawing allowed.
	}
	
	public void size(int width, int height) {
		//this will be called the first time an applet sets its size, but
		//also any time that it's called while the PApplet is running.
	}
	
	public void stop() {
		//can be called by users, for instance movie.stop() will shut down
		//a movie that's being played, or camera.stop() stops capturing 
		//video. server.stop() will shut down the server and shut it down
		//completely, which is identical to its "dispose" function.
	}
	
	public void dispose() {
		//this should only be called by PApplet. dispose() is what gets 
		//called when the host applet is stopped, so this should shut down
		//any threads, disconnect from the net, unload memory, etc. 
		
		if (client.isConnected()) client.disconnect();
		
	}
	
}
