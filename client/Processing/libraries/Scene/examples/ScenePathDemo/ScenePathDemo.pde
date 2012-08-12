/* 
 * Scene Processing Library 1.0
 * by Laurence Bender (c) 2011
 *
 * SceneBlob graphic client example.
 * Draws the center of mass and blob paths of recieved SceneBlobs. 
 *
 */

import TUIO.*;
import scene.tuio.*;

Scene SceneClient;

PFont font;

void setup() {

  size(640, 480);

  font = loadFont("ArialMT-14.vlw");
  textFont(font, 14);

  rectMode(CENTER);

  SceneClient  = new Scene(this);
}

void draw() {

  background(0);

  Vector sceneBlobList = SceneClient.getSceneBlobs();

  for (int i=0;i<sceneBlobList.size();i++) {

    SceneBlob sblb = (SceneBlob)sceneBlobList.elementAt(i);

    noStroke();
    fill(0, 255, 0);
    ellipse(sblb.getCenterOfMassScreenX(width), sblb.getCenterOfMassScreenY(height), 5, 5);
    text(str(sblb.getBlobID()), sblb.getCenterOfMassScreenX(width) + 10, sblb.getCenterOfMassScreenY(height) - 10);

    noStroke();
    fill(255,0, 0);
    ellipse(sblb.getBlobScreenX(width), sblb.getBlobScreenY(height), 5, 5);
    text(str(sblb.getBlobID()), sblb.getBlobScreenX(width) + 10, sblb.getBlobScreenY(height) - 10);
  
    Vector cmpointList = sblb.getCenterOfMassPath();

    if (cmpointList.size()>0) {
      stroke(0,255,0);
      TuioPoint cmstart_point = (TuioPoint)cmpointList.firstElement();
      for (int j=0;j<cmpointList.size();j++) {
        TuioPoint cmend_point = (TuioPoint)cmpointList.elementAt(j);
        line(cmstart_point.getScreenX(width), cmstart_point.getScreenY(height), cmend_point.getScreenX(width), cmend_point.getScreenY(height));
        cmstart_point = cmend_point;
      }
    }
    
    Vector blobpointList = sblb.getBlobPath();

    if (blobpointList.size()>0) {
      stroke(255, 0, 0);
      TuioPoint blobstart_point = (TuioPoint)blobpointList.firstElement();
      for (int j=0;j<blobpointList.size();j++) {
        TuioPoint blobend_point = (TuioPoint)blobpointList.elementAt(j);
        line(blobstart_point.getScreenX(width), blobstart_point.getScreenY(height), blobend_point.getScreenX(width), blobend_point.getScreenY(height));
        blobstart_point = blobend_point;
      }
    }
    
  }

}

void refresh(TuioTime bundleTime) { 
  
  redraw();
  
}
