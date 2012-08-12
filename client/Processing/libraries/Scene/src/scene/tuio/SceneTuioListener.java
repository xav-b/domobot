/*
    Scene Tuio Java Client 1.0
    http://scene.sourceforge.net/
 
    Copyright (c) 2011 Laurence Bender <lbender@untref.edu.ar>
 
    Based on the TUIO Java backend - part of the reacTIVision project
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

import TUIO.*;

/**
 * The SceneTuioListener interface extends the TuioListener inteface, and provides a simple callback infrastructure which is used by the {@link SceneTuioClient} class 
 * to dispatch TUIO and Scene events to all registered instances of classes that implement the interface.<P> 
 * Any class that implements the SceneTuioListener interface is required to implement all of the callback methods defined here.
 * The {@link SceneTuioClient} makes use of these interface methods in order to dispatch TUIO and Scene events to all registered SceneTuioListener implementations.<P>
 * <code>
 * public class MySceneTuioListener implements SceneTuioListener<br/>
 * ...</code><p><code>
 * MySceneTuioListener listener = new MySceneTuioListener();<br/>
 * SceneTuioClient client = new SceneTuioClient();<br/>
 * client.addTuioListener(listener);<br/>
 * client.start();<br/>
 * </code>
 *
 * @author Laurence Bender (based on TUIO Java backend 1.4 by Martin Kaltenbrunner)
 * @version 1.0
 */
 
public interface SceneTuioListener extends TuioListener {
	
	/**
	 * This callback method is invoked by the SceneTuioClient when a new SceneTuioBlob is added to the session.   
	 *
	 * @param  tblb  the SceneTuioBlob reference associated to the addSceneTuioBlob event
	 */
	public void addSceneTuioBlob(SceneTuioBlob tblb);

	/**
	 * This callback method is invoked by the SceneTuioClient when an existing SceneTuioBlob is updated during the session.   
	 *
	 * @param  tblb  the SceneTuioBlob reference associated to the updateSceneTuioBlob event
	 */
	public void updateSceneTuioBlob(SceneTuioBlob tblb);

	/**
	 * This callback method is invoked by the SceneTuioClient when an existing SceneTuioBlob is removed from the session.   
	 *
	 * @param  tblb  the SceneTuioBlob reference associated to the removeSceneTuioBlob event
	 */
	public void removeSceneTuioBlob(SceneTuioBlob tblb);
	
	/**
	 * This callback method is invoked by the SceneTuioClient when an existing SceneBlob is added to the session.   
	 *
	 * @param  sblb  the SceneBlob reference associated to the addSceneBlob event
	 */
	public void addSceneBlob(SceneBlob sblb);
	
	/**
	 * This callback method is invoked by the SceneTuioClient when an existing SceneBlob is updated during the session.   
	 *
	 * @param  sblb  the TuioBlob reference associated to the updateSceneBlob event
	 */
	public void updateSceneBlob(SceneBlob sblb);
	
	/**
	 * This callback method is invoked by the SceneTuioClient when an existing SceneBlob is removed from the session.   
	 *
	 * @param  sblb  the TuioBlob reference associated to the removeSceneBlob event
	 */
	public void removeSceneBlob(SceneBlob sblb);
	
}
