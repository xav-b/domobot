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

import java.util.*;
import TUIO.*;

/**
 * The SceneBlob class encapsulates Scene blobs. The center of mass position
 * and speed of the blob is contained in the TuioCursor member object. The bounding box
 * attributes and blob area are contained in the SceneTuioBlob member object. 
 * 
 *
 * @author Laurence Bender (based on the TUIO Java backend 1.4 by Martin Kaltenbrunner)
 * @version 1.0
 */ 
 
public class SceneBlob {

	/**
	 * The TuioCursor containing the center of mass position and velocities of the blob.
	 */ 
	protected TuioCursor tuio_cursor;
	
	/**
	 * The SceneTuioBlob containing the bounding box attributes and the area of the blob.
	 */ 
	protected SceneTuioBlob tuio_blob;
	
	/**
	 * This constructor takes a TuioCursor argument and a SceneTuioBlob argument 
	 * with ths same sesssion ID, and assigns them to the SceneBlob.
	 *
	 * @param	tcur	the TuioCursor to assign
	 * @param	tblb	the SceneTuioBlob to assign
	 */ 
	public SceneBlob (TuioCursor tcur, SceneTuioBlob tblb) {
	
		tuio_cursor = tcur;	
		tuio_blob = tblb;
		
	}
	
	/**
	 * Returns the blob ID of this SceneBlob.
	 * @return	the blob ID of this SceneBlob
	 */
	public int getBlobID() {
	
		return tuio_blob.getBlobID();
		
	}
	
	/**
	 * Returns the Session ID of this SceneBlob.
	 * @return	the Session ID of this SceneBlob
	 */
	public long getSessionID() {
	
		return tuio_blob.getSessionID();
		
	}
	
	/**
	 * Returns the center of mass X position of this SceneBlob.
	 * @return	the center of mass X position of this SceneBlob
	 */
	public float getCenterOfMassX() {
	
		return tuio_cursor.getX();
		
	}
	
	/**
	 * Returns the center of mass y position of this SceneBlob.
	 * @return	the center of mass Y position of this SceneBlob
	 */
	public float getCenterOfMassY() {
	
		return tuio_cursor.getY();
		
	}
	
	/**
	 * Returns the center of mass X coordinate in pixels relative to the provided screen width. 
	 *
	 * @param	width	the screen width
	 * @return	the X coordinate of the center of mass in pixels relative to the provided screen width
	 */
	public float getCenterOfMassScreenX(int width) {
	
		return tuio_cursor.getScreenX(width);
		
	}
	
	/**
	 * Returns the center of mass Y coordinate in pixels relative to the provided screen height. 
	 *
	 * @param	height	the screen height
	 * @return	the Y coordinate of the center of mass in pixels relative to the provided
	 *			screen height
	 */
	public float getCenterOfMassScreenY(int width) {
	
		return tuio_cursor.getScreenY(width);
		
	}
	
	/**
	 * Returns the center of mass X velocity of this SceneBlob.
	 * @return	the center of mass X velocity of this SceneBlob
	 */
	public float getCenterOfMassXSpeed() {
	
		return tuio_cursor.getXSpeed();
		
	}
	
	/**
	 * Returns the center of mass Y velocity of this SceneBlob.
	 * @return	the center of mass X velocity of this SceneBlob
	 */
	public float getCenterOfMassYSpeed() {
	
		return tuio_cursor.getYSpeed();
		
	}
	
	/**
	 * Returns the center of mass position of this SceneBlob.
	 * @return	the center of mass position of this SceneBlob
	 */
	public TuioPoint getCenterOfMassPosition() {
	
		return new TuioPoint(tuio_cursor.getX(),tuio_cursor.getY());
		
	}
	
	/**
	 * Returns the center of mass path of this SceneBlob.
	 * @return	the center of mass path of this SceneBlob
	 */
	public Vector<TuioPoint> getCenterOfMassPath() {
	
		return tuio_cursor.getPath();
		
	}
	
	/**
	 * Returns the center of mass motion speed of this SceneBlob.
	 * @return	the center of mass motion speed of this SceneBlob
	 */
	public float getCenterOfMassMotionSpeed() {
	
		return tuio_cursor.getMotionSpeed();
		
	}
	
	/**
	 * Returns the center of mass motion acceleration of this SceneBlob.
	 * @return	the center of mass motion acceleration of this SceneBlob
	 */
	public float getCenterOfMassMotionAccel() {
	
		return tuio_cursor.getMotionAccel();
		
	}
	
	/**
	 * Returns the X position of this SceneBlob.
	 * @return	the X position of this SceneBlob
	 */
	public float getBlobX() {
	
		return tuio_blob.getX();
		
	}
	
	/**
	 * Returns the X position of this SceneBlob.
	 * @return	the X position of this SceneBlob
	 */
	public float getBlobY() {
	
		return tuio_blob.getY();
		
	}
	
	/**
	 * Returns the blob X coordinate in pixels relative to the provided screen width. 
	 *
	 * @param	width	the screen width
	 * @return	the X coordinate of this blob in pixels relative to the provided screen width
	 */
	public float getBlobScreenX(int width) {
	
		return tuio_blob.getScreenX(width);
		
	}
	
	/**
	 * Returns the blob Y coordinate in pixels relative to the provided screen height. 
	 *
	 * @param	height	the screen height
	 * @return	the Y coordinate of this blob in pixels relative to the provided screen height
	 */
	public float getBlobScreenY(int height) {
	
		return tuio_blob.getScreenY(height);
		
	}
	
	/**
	 * Returns the X velocity of this SceneBlob.
	 * @return	the X velocity of this SceneBlob
	 */
	public float getBlobXSpeed() {
	
		return tuio_blob.getXSpeed();
		
	}
	
	/**
	 * Returns the Y velocity of this SceneBlob.
	 * @return	the Y velocity of this SceneBlob
	 */
	public float getBlobYSpeed() {
	
		return tuio_blob.getYSpeed();
		
	}
	
	/**
	 * Returns the position of this SceneBlob.
	 * @return	the position of this SceneBlob
	 */
	public TuioPoint getBlobPosition() {
	
		return new TuioPoint(tuio_blob.getX(),tuio_blob.getY());
		
	}
	
	/**
	 * Returns the path of this SceneBlob.
	 * @return	the path of this SceneBlob
	 */
	public Vector<TuioPoint> getBlobPath() {
	
		return tuio_blob.getPath();
		
	}
	
	/**
	 * Returns the motion speed of this SceneBlob.
	 * @return	the motion speed of this SceneBlob
	 */
	public float getBlobMotionSpeed() {
	
		return tuio_blob.getMotionSpeed();
		
	}
	
	/**
	 * Returns the motion acceleration of this SceneBlob.
	 * @return	the motion acceleration of this SceneBlob
	 */
	public float getBlobMotionAccel() {
        
		return tuio_blob.getMotionAccel();
        
	}
	
	/**
	 * Returns the angle of this SceneBlob.
	 * @return	the angle of this SceneBlob
	 */
	public float getBlobAngle() {
        
		return tuio_blob.getAngle();
        
	}

	/**
	 * Returns the width of this SceneBlob.
	 * @return	the width of this SceneBlob
	 */
	public float getBlobWidth() {
	
		return tuio_blob.getWidth();
		
	}

	/**
	 * Returns the height of this SceneBlob.
	 * @return	the height of this SceneBlob
	 */
	public float getBlobHeight() {
	
		return tuio_blob.getHeight();
		
	}
	
	/**
	 * Returns the blob width in pixels relative to the provided screen width. 
	 *
	 * @param	width	the screen width
	 * @return	the width of this blob in pixels relative to the provided screen width
	 */
	public float getBlobScreenWidth(int width) {
	
		return Math.round(tuio_blob.getWidth()*width);
		
	}

	/**
	 * Returns the blob height in pixels relative to the provided screen height. 
	 *
	 * @param	height	the screen height
	 * @return	the height of this blob in pixels relative to the provided screen height
	 */
	public float getBlobScreenHeight(int height) {
	
		return Math.round(tuio_blob.getHeight()*height);
		
	}
	
	/**
	 * Returns the area of this SceneBlob.
	 * @return	the area of this SceneBlob
	 */
	public float getBlobArea() {
	
		return tuio_blob.getArea();
		
	}
	
	/**
	 * Returns the blob area in pixels relative to the provided screen width and height. 
	 *
	 * @param	width	the screen width
	 * @param	height	the screen height
	 * @return	the area of this blob in pixels relative to the provided screen width and height
	 */
	public float getBlobScreenArea(int width, int height) {
	
		return tuio_blob.getArea()*width*height;
		
	}
	
	/**
	 * Returns the angle in degrees of this SceneBlob.
	 * @return	the angle in degrees of this SceneBlob
	 */
	public float getBlobAngleDegrees() {
	
		return tuio_blob.getAngle()/(float)Math.PI*180.0f;
		
	}
	
	/** 
	 * Returns the rotation speed of this SceneBlob.
	 * @return	the rotation speed of this SceneBlob
	 */
	public float getBlobRotationSpeed() {
	
		return tuio_blob.getRotationSpeed();
		
	}
		
	/**
	 * Returns the rotation acceleration of this SceneBlob.
	 * @return	the rotation acceleration of this SceneBlob
	 */
	public float getBlobRotationAccel() {
	
		return tuio_blob.getRotationAccel();
		
	}
	
	/**
	 * Returns the aspect ratio (width/height) of this SceneBlob.
	 * @return	the aspect ratio of this SceneBlob
	 */
	public float getBlobAspectRatio() {
	
		if (tuio_blob.getHeight()>0) 
			return tuio_blob.getWidth()/tuio_blob.getHeight();
		else
			return -1.0f;
		
	}
	
	/**
	 * Returns the contraction index (area/bounding box area) of this SceneBlob.
	 * @return	the contraction index of this SceneBlob
	 */
	public float getBlobContractionIndex() {
	
		if (tuio_blob.getWidth()*tuio_blob.getHeight()>0) 
			return tuio_blob.getArea()/(tuio_blob.getWidth()*tuio_blob.getHeight());
		else
			return -1.0f;
		
	}
	
	/**
	 * Returns the diameter of a circle with the same area of this SceneBlob.
	 * @return	the equivalent diameter of this SceneBlob
	 */
	public float getBlobEquivDiameter() {
	
			return (float)Math.sqrt(4.0f*tuio_blob.getArea()/(float)Math.PI);
		
	}

	/**
	 * Returns true of this SceneBlob is moving.
	 * @return	true of this SceneBlob is moving
	 */
	public boolean isMoving() { 
	
		if ((tuio_blob.getTuioState()==SceneTuioBlob.TUIO_ACCELERATING) || (tuio_blob.getTuioState()==SceneTuioBlob.TUIO_DECELERATING) || (tuio_blob.getTuioState()==SceneTuioBlob.TUIO_ROTATING)) {
			return true;
		} else {
			return false;
		}
		
	}
	
}
