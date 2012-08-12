/* 
 * Scene Processing Library 1.0
 * by Laurence Bender (c) 2011
 *
 * TUIO Cursor and TUIO Blob client example. 
 *
 */

import TUIO.*;
import scene.tuio.*;

Scene SceneClient;

void setup() {

  SceneClient  = new Scene(this);
  
}

void draw() {

  Vector tuioCursorList = SceneClient.getTuioCursors();
  
  for (int i=0;i<tuioCursorList.size();i++) {

    TuioCursor tcur = (TuioCursor)tuioCursorList.elementAt(i);
    println("Cursor: " + tcur.getCursorID() + " " +
                       tcur.getSessionID() + " " +
                       tcur.getX() + " " +
                       tcur.getY() + " " + 
                       tcur.getXSpeed() + " " +
                       tcur.getYSpeed() + " " +
                       tcur.getMotionAccel()
                       );
  
  }

  Vector tuioBlobList = SceneClient.getSceneTuioBlobs();

  for (int i=0;i<tuioBlobList.size();i++) {

    SceneTuioBlob tblb = (SceneTuioBlob)tuioBlobList.elementAt(i);
    println("Blob: " + tblb.getBlobID() + " " +
                       tblb.getSessionID() + " " +
                       tblb.getX() + " " +
                       tblb.getY() + " " + 
                       tblb.getAngle() + " " +
                       tblb.getWidth() + " " +
                       tblb.getHeight() + " " +
                       tblb.getArea() + " " +
                       tblb.getXSpeed() + " " +
                       tblb.getYSpeed() + " " +
                       tblb.getRotationSpeed() + " " +
                       tblb.getMotionAccel() + " " +
                       tblb.getRotationAccel() 
                       );
    
  }
  
}

void addSceneTuioBlob(SceneTuioBlob tblb) {
  
  println("SceneTuioBlob added");
  
}

void removeSceneTuioBlob(SceneTuioBlob tblb) {
  
  println("SceneTuioBlob removed");
  
}

void updateSceneTuioBlob(SceneTuioBlob tblb) {
  
    println("SceneTuioBlob updated");
    
}

