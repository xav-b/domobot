/* 
 * Scene Processing Library 1.0
 * by Laurence Bender (c) 2011
 *
 * SceneBlob graphic client example.
 * Draws the center of mass and bounding box of recieved SceneBlobs. 
 *
 */

import TUIO.*;
import scene.tuio.*;

Scene SceneClient;

PFont font;

void setup() {
  
  size(640,480);

  font = loadFont("ArialMT-14.vlw");
  textFont(font,14);
  
  rectMode(CENTER);

  SceneClient  = new Scene(this);
  
}

void draw() {
  
  background(0);

  Vector sceneBlobList = SceneClient.getSceneBlobs();
  
  for (int i=0;i<sceneBlobList.size();i++) {
    
    SceneBlob sblb = (SceneBlob)sceneBlobList.elementAt(i);
    
    noStroke();
    fill(255,0,0);
    rect(sblb.getBlobScreenX(width), sblb.getBlobScreenY(height),5,5);
    
    stroke(255,0,0);
    fill(255,0,0,100);
    rect(sblb.getBlobScreenX(width), sblb.getBlobScreenY(height),sblb.getBlobScreenWidth(width), sblb.getBlobScreenHeight(height));
    
    noStroke();
    fill(0,255,0);
    ellipse(sblb.getCenterOfMassScreenX(width), sblb.getCenterOfMassScreenY(height),5,5);
    text(str(sblb.getBlobID()),  sblb.getCenterOfMassScreenX(width) + 10,  sblb.getCenterOfMassScreenY(height) - 10);
  
  }
  
}
