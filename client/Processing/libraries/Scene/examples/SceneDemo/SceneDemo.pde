/* 
 * Scene Processing Library 1.0
 * by Laurence Bender (c) 2011
 *
 * SceneBlob client example. 
 *
 */

import TUIO.*;
import scene.tuio.*;

Scene SceneClient;

void setup() {

  SceneClient  = new Scene(this);
  
}

void draw() {

  Vector sceneBlobList = SceneClient.getSceneBlobs();
  
  for (int i=0;i<sceneBlobList.size();i++) {

    SceneBlob sblb = (SceneBlob)sceneBlobList.elementAt(i);
    println("Blob: " + sblb.getBlobID() + " " +
                       sblb.getSessionID() + " " +
                       sblb.getCenterOfMassX() + " " +
                       sblb.getCenterOfMassY() + " " +
                       sblb.getCenterOfMassScreenX(width) + " " +
                       sblb.getCenterOfMassScreenY(height) + " " +
                       sblb.getCenterOfMassXSpeed() + " " +
                       sblb.getCenterOfMassYSpeed() + " " +
                       sblb.getCenterOfMassMotionAccel() + " " +
                       sblb.getBlobX() + " " +
                       sblb.getBlobY() + " " +
                       sblb.getBlobScreenX(width) + " " +
                       sblb.getBlobScreenY(height) + " " +
                       sblb.getBlobAngle() + " " + 
                       sblb.getBlobWidth() + " " +
                       sblb.getBlobHeight() + " " +
                       sblb.getBlobScreenWidth(width) + " " +
                       sblb.getBlobScreenHeight(height) + " " +
                       sblb.getBlobArea() + " " +
                       sblb.getBlobXSpeed() + " " +
                       sblb.getBlobYSpeed() + " " +
                       sblb.getBlobRotationSpeed() + " " +
                       sblb.getBlobMotionAccel() + " " +
                       sblb.getBlobRotationAccel() + " " +
                       sblb.getBlobAspectRatio() + " " +
                       sblb.getBlobContractionIndex() + " " +
                       sblb.getBlobEquivDiameter()
                       );
  
  }
  
}

void addSceneBlob(SceneBlob sblb) {
  
  println("SceneBlob added");
  
}

void removeSceneBlob(SceneBlob sblb) {
  
  println("SceneBlob removed");
  
}

void updateSceneBlob(SceneBlob sblb) {
  
  println("SceneBlob updated");
  
}

