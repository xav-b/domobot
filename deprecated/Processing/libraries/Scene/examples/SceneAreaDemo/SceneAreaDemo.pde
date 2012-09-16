/* 
 * Scene Processing Library 1.0
 * by Laurence Bender (c) 2011
 *
 * Draws circles with sizes proportional to the blob areas.
 *
 */

import TUIO.*;
import scene.tuio.*;

Scene SceneClient;

void setup() {
  
  size(640,480);
  
  rectMode(CENTER);

  SceneClient  = new Scene(this);
  
}

void draw() {
  
  background(0);

  Vector sceneBlobList = SceneClient.getSceneBlobs();
  
  for (int i=0;i<sceneBlobList.size();i++) {
    
    SceneBlob sblb = (SceneBlob)sceneBlobList.elementAt(i);
    
    noStroke();
    fill(0,255,0);
    ellipse(sblb.getCenterOfMassScreenX(width), sblb.getCenterOfMassScreenY(height), height*sblb.getBlobEquivDiameter(),height*sblb.getBlobEquivDiameter());
 
  }
  
}
