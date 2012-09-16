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
 * The SceneTuioBlob class encapsulates /tuio/2Dblb TUIO blobs.
 *
 * @author Laurence Bender (based on the TUIO Java backend 1.4 by Martin Kaltenbrunner)
 * @version 1.0
 */ 
 
public class SceneTuioBlob extends TuioPoint {

	/**
	 * The unique session ID number that is assigned to each TUIO blob.
	 */ 
	protected long session_id;
	
	/**
	 * The X-axis velocity value.
	 */ 
	protected float x_speed;
	
	/**
	 * The Y-axis velocity value.
	 */ 
	protected float y_speed;
	
	/**
	 * The motion speed value.
	 */ 
	protected float motion_speed;
		
	/**
	 * The motion acceleration value.
	 */ 
	protected float motion_accel;
			
	/**
	 * A Vector of TuioPoints containing all the previous positions of the TUIO component.
	 */ 
	protected Vector<TuioPoint> path;
	
	/**
	 * Defines the ADDED state.
	 */ 
	public static final int TUIO_ADDED = 0;
	
	/**
	 * Defines the ACCELERATING state.
	 */ 
	public static final int TUIO_ACCELERATING = 1;
	
	/**
	 * Defines the DECELERATING state.
	 */ 
	public static final int TUIO_DECELERATING = 2;
	
	/**
	 * Defines the STOPPED state.
	 */ 
	public static final int TUIO_STOPPED = 3;
	
	/**
	 * Defines the REMOVED state.
	 */ 
	public static final int TUIO_REMOVED = 4;
	
	/**
	 * Reflects the current state of the Tuio blob
	 */ 
	protected int state;
	
	/**
	 * The individual blob ID number that is assigned to each Tuio blob.
	 */ 
	protected int blob_id;	
	
	/**
	 * The blob angle value.
	 */ 
	protected float angle;
	
	/**
	 * The blob width value.
	 */ 
	protected float width;
	
	/**
	 * The blob height value.
	 */ 
	protected float height;
	
	/**
	 * The blob area value.
	 */ 
	protected float area;
			
	/**
	 * The rotation speed value.
	 */ 
	protected float rotation_speed;
	
	/**
	 * The rotation acceleration value.
	 */ 
	protected float rotation_accel;
	
	/**
	 * Defines the ROTATING state.
	 */ 
	public static final int TUIO_ROTATING = 5;
	
	/**
	 * This constructor takes a TuioTime argument and assigns it along with the provided 
	 * Session ID, Blob ID, X and Y coordinates, angle, width, height and area, and assigns 
     * these values to the newly created Tuio blob.
	 *
	 * @param	ttime	the TuioTime to assign
	 * @param	si	the Session ID  to assign
	 * @param	bi	the Blob ID  to assign
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 * @param	a	the angle to assign
     * @param	w	the width to assign
     * @param	h	the height to assign
     * @param	f	the area to assign
	 */ 
	public SceneTuioBlob (TuioTime ttime, long si, int bi, float xp, float yp, float a, float w, float h, float f) {
	
		super(ttime,xp,yp);
		
		session_id = si;
		x_speed = 0.0f;
		y_speed = 0.0f;
		motion_speed = 0.0f;
		motion_accel = 0.0f;
		
		blob_id = bi;
		angle = a;
		width = w;
		height = h;
		area = f;
		
		rotation_speed = 0.0f;
		rotation_accel = 0.0f;
		
		path = new Vector<TuioPoint>();
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		state = TUIO_ADDED;
		
	}

	/**
	 * This constructor takes the provided Session ID, Blob ID, X and Y coordinates, 
     * angle, width, height and area, and assigns these values to the newly created Tuio blob. 
	 *
	 * @param	si	the Session ID  to assign
	 * @param	bi	the Blob Symbol ID  to assign
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 * @param	a	the angle to assign
     * @param	w	the width to assign
     * @param	h	the height to assign
     * @param	f	the area to assign
	 */	
	public SceneTuioBlob(long si, int bi, float xp, float yp, float a, float w, float h, float f) {
	
		super(xp,yp);
		
		session_id = si;
		x_speed = 0.0f;
		y_speed = 0.0f;
		motion_speed = 0.0f;
		motion_accel = 0.0f;
		
		blob_id = bi;
		angle = a;
		width = w;
		height = h;
		area = f;
		
		rotation_speed = 0.0f;
		rotation_accel = 0.0f;
		
		path = new Vector<TuioPoint>();
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		state = TUIO_ADDED;
		
	}

	/**
	 * This constructor takes the attributes of the provided Tuio blob 
	 * and assigns these values to the newly created SceneTuioBlob.
	 *
	 * @param	tblb	the SceneTuioBlob to assign
	 */
	public SceneTuioBlob(SceneTuioBlob tblb) {
		
		super(tblb);
		
		session_id = tblb.getSessionID();
		x_speed = tblb.getXSpeed();
		y_speed = tblb.getYSpeed();
		motion_speed = tblb.getMotionSpeed();
		motion_accel = tblb.getMotionAccel();
		
		blob_id = tblb.getBlobID();
		angle = tblb.getAngle();
		width = tblb.getWidth();
		height = tblb.getHeight();
		area = tblb.getArea();
		rotation_speed = tblb.getRotationSpeed();
		rotation_accel = tblb.getRotationAccel();
		
		path = new Vector<TuioPoint>();
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		state = TUIO_ADDED;
		
	}

	/**
	 * Takes a TuioTime argument and assigns it along with the provided 
	 * X and Y coordinate, angle, width, height, area, 
	 * X and Y velocity, motion acceleration, 
	 * rotation speed and rotation acceleration to the private Tuio blob attributes.
	 *
	 * @param	ttime	the TuioTime to assign
	 * @param	xp		the X coordinate to assign
	 * @param	yp		the Y coordinate to assign
	 * @param	a	the angle to assign
     * @param	w	the width to assign
     * @param	h	the height to assign
     * @param	f	the area to assign
	 * @param	xs	the X velocity to assign
	 * @param	ys	the Y velocity to assign
	 * @param	rs	the rotation speed to assign
	 * @param	ma	the motion acceleration to assign
	 * @param	ra	the rotation acceleration to assign
	 */
	public void update(TuioTime ttime, float xp, float yp, float a, float w, float h, float f, float xs, float ys, float rs, float ma, float ra) {
	
		super.update(ttime,xp,yp);
		x_speed = xs;
		y_speed = ys;
		motion_speed = (float)Math.sqrt(x_speed*x_speed+y_speed*y_speed);
		motion_accel = ma;
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		if (motion_accel>0) state = TUIO_ACCELERATING;
		else if (motion_accel<0) state = TUIO_DECELERATING;
		else state = TUIO_STOPPED;
		
		angle = a;
		width = w;
		height = h;
		area = f;
		rotation_speed = rs;
		rotation_accel = ra;
		if ((rotation_accel!=0) && (state!=TUIO_STOPPED)) state = TUIO_ROTATING;
		
	}

	/**
	 * Assigns the provided 
	 * X and Y coordinate, angle, width, height, area,
	 * X and Y velocity, motion acceleration,
	 * rotation speed and rotation acceleration to the private Tuio blob attributes.
	 *
	 * @param	xp		the X coordinate to assign
	 * @param	yp		the Y coordinate to assign
	 * @@param	a	the angle to assign
     * @param	w	the width to assign
     * @param	h	the height to assign
     * @param	f	the area to assign
	 * @param	xs	the X velocity to assign
	 * @param	ys	the Y velocity to assign
	 * @param	rs	the rotation speed to assign
	 * @param	ma	the motion acceleration to assign
	 * @param	ra	the rotation acceleration to assign	 
     */
	public void update(float xp, float yp, float a, float w, float h, float f, float xs, float ys, float rs, float ma, float ra) {
	
		super.update(xp,yp);
		x_speed = xs;
		y_speed = ys;
		motion_speed = (float)Math.sqrt(x_speed*x_speed+y_speed*y_speed);
		motion_accel = ma;
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		if (motion_accel>0) state = TUIO_ACCELERATING;
		else if (motion_accel<0) state = TUIO_DECELERATING;
		else state = TUIO_STOPPED;
		
		angle = a;
		width = w;
		height = h;
		area = f;
		rotation_speed = rs;
		rotation_accel = ra;
		if ((rotation_accel!=0) && (state!=TUIO_STOPPED)) state = TUIO_ROTATING;
		
	}
	
	/**
	 * Takes a TuioTime argument and assigns it along with the provided 
	 * X and Y coordinate, angle, width, height and area, to the private Tuio blob attributes.
	 * The speed and acceleration values are calculated accordingly.
	 *
	 * @param	ttime	the TuioTime to assign
	 * @param	xp	the X coordinate to assign
	 * @param	yp	the Y coordinate to assign
	 * @param	a	the angle to assign
     * @param	w	the width to assign
     * @param	h	the height to assign
     * @param	f	the area to assign
	 */
	public void update(TuioTime ttime, float xp, float yp, float a, float w, float h, float f) {
		
		TuioPoint lastPoint = path.lastElement();
		super.update(ttime,xp,yp);
		
		TuioTime diffTime = currentTime.subtract(lastPoint.getTuioTime());
		float dt = diffTime.getTotalMilliseconds()/1000.0f;
		float dx = this.xpos - lastPoint.getX();
		float dy = this.ypos - lastPoint.getY();
		float dist = (float)Math.sqrt(dx*dx+dy*dy);
		float last_motion_speed = this.motion_speed;
		
		this.x_speed = dx/dt;
		this.y_speed = dy/dt;
		this.motion_speed = dist/dt;
		this.motion_accel = (motion_speed - last_motion_speed)/dt;
		
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		if (motion_accel>0) state = TUIO_ACCELERATING;
		else if (motion_accel<0) state = TUIO_DECELERATING;
		else state = TUIO_STOPPED;

		float last_angle = angle;
		float last_rotation_speed = rotation_speed;
		angle = a;
		width = w;
		height = h;
		area = f;
		
		float da = (this.angle-last_angle)/(2.0f*(float)Math.PI);
		if (da>0.75f) da-=1.0f;
		else if (da<-0.75f) da+=1.0f;
		
		rotation_speed = da/dt;
		rotation_accel = (rotation_speed - last_rotation_speed)/dt;
		if ((rotation_accel!=0) && (state!=TUIO_STOPPED)) state = TUIO_ROTATING;
		
	}
	
	/**
	 * Takes the attributes of the provided SceneTuioBlob 
	 * and assigns these values to this SceneTuioBlob.
	 * The TuioTime time stamp remains unchanged.
	 *
	 * @param	tblb	the SceneTuioBlob to assign
	 */	
	public void update(SceneTuioBlob tblb) {
	
		super.update(tblb);	
		
		x_speed = tblb.getXSpeed();
		y_speed = tblb.getYSpeed();
		motion_speed = tblb.getMotionSpeed();
		motion_accel = tblb.getMotionAccel();
		path.addElement(new TuioPoint(currentTime,xpos,ypos));
		if (motion_accel>0) state = TUIO_ACCELERATING;
		else if (motion_accel<0) state = TUIO_DECELERATING;
		else state = TUIO_STOPPED;
			
		blob_id = tblb.getBlobID();
		angle = tblb.getAngle();
		width = tblb.getWidth();
		height = tblb.getHeight();
		area = tblb.getArea();
		rotation_speed = tblb.getRotationSpeed();
		rotation_accel = tblb.getRotationAccel();
		if ((rotation_accel!=0) && (state!=TUIO_STOPPED)) state = TUIO_ROTATING;
		
	}
	
	/**
	 * Assigns the REMOVE state to the SceneTuioBlob and sets
	 * its TuioTime time stamp to the provided TuioTime argument.
	 *
	 * @param	ttime	the TuioTime to assign
	 */
	public void remove(TuioTime ttime) {
	
		currentTime = new TuioTime(ttime);
		state = TUIO_REMOVED;
		
	}
	
	/**
	 * This method is used to calculate the speed and acceleration values of a
	 * Tuio blob with unchanged position, angle, width, height and area.
	 *
	 * @param	ttime	the TuioTime to assign
	 */
	public void stop(TuioTime ttime) {
	
		update(ttime,xpos,ypos,angle,width,height,area);
		
	}
	
	/**
	 * Returns the Session ID of this Tuio blob.
	 * @return	the Session ID of this Tuio blob
	 */
	public long getSessionID() {
	
		return session_id;
		
	}
	
	/**
	 * Returns the X velocity of this Tuio blob.
	 * @return	the X velocity of this Tuio blob
	 */
	public float getXSpeed() {
	
		return x_speed;
		
	}
	
	/**
	 * Returns the Y velocity of this Tuio blob.
	 * @return	the Y velocity of this Tuio blob
	 */
	public float getYSpeed() {
	
		return y_speed;
		
	}
	
	/**
	 * Returns the position of this Tuio blob.
	 * @return	the position of this Tuio blob
	 */
	public TuioPoint getPosition() {
	
		return new TuioPoint(xpos,ypos);
		
	}
	
	/**
	 * Returns the path of this Tuio blob.
	 * @return	the path of this Tuio blob
	 */
	public Vector<TuioPoint> getPath() {
	
		return path;
		
	}
	
	/**
	 * Returns the motion speed of this Tuio blob.
	 * @return	the motion speed of this Tuio blob
	 */
	public float getMotionSpeed() {
	
		return motion_speed;
		
	}
	
	/**
	 * Returns the motion acceleration of this Tuio blob.
	 * @return	the motion acceleration of this Tuio blob
	 */
	public float getMotionAccel() {
        
		return motion_accel;
        
	}
	
	/**
	 * Returns the TUIO state of this Tuio blob.
	 * @return	the TUIO state of this Tuio blob
	 */
	public int getTuioState() {
	
		return state;
		
	}
	
	/**
	 * Returns the blob ID of this Tuio blob.
	 * @return	the blob ID of this Tuio blob
	 */
	public int getBlobID() {
	
		return blob_id;
		
	}
	
	/**
	 * Returns the angle of this Tuio blob.
	 * @return	the angle of this Tuio blob
	 */
	public float getAngle() {
		return angle;
	}

	/**
	 * Returns the width of this Tuio blob.
	 * @return	the width of this Tuio blob
	 */
	public float getWidth() {
	
		return width;
		
	}

	/**
	 * Returns the height of this Tuio blob.
	 * @return	the height of this Tuio blob
	 */
	public float getHeight() {
	
		return height;
		
	}
	
	/**
	 * Returns the area of this Tuio blob.
	 * @return	the area of this Tuio blob
	 */
	public float getArea() {
	
		return area;
		
	}
	
	/**
	 * Returns the angle in degrees of this Tuio blob.
	 * @return	the angle in degrees of this Tuio blob
	 */
	public float getAngleDegrees() {
	
		return angle/(float)Math.PI*180.0f;
		
	}
	
	/** 
	 * Returns the rotation speed of this Tuio blob.
	 * @return	the rotation speed of this Tuio blob
	 */
	public float getRotationSpeed() {
	
		return rotation_speed;
		
	}
		
	/**
	 * Returns the rotation acceleration of this Tuio blob.
	 * @return	the rotation acceleration of this Tuio blob
	 */
	public float getRotationAccel() {
	
		return rotation_accel;
		
	}

	/**
	 * Returns true of this Tuio blob is moving.
	 * @return	true of this Tuio blob is moving
	 */
	public boolean isMoving() { 
	
		if ((state==TUIO_ACCELERATING) || (state==TUIO_DECELERATING) || (state==TUIO_ROTATING)) {
			return true;
		} else {
			return false;
		}
		
	}
	
}
