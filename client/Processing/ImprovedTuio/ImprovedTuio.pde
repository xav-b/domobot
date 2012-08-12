import TUIO.*;
TuioProcessing tuioClient;

// these are some helper variables which are used
// to create scalable graphical feedback
float cursor_size = 15;
float object_size = 60;
float table_size = 760;
float scale_factor = 1;
PFont font;

void setup()
{
  //size(screen.width,screen.height);
  size(640,480);
  noStroke();
  fill(0);
  
  loop();
  frameRate(30);
  //noLoop();
  
  hint(ENABLE_NATIVE_FONTS);
  font = createFont("Arial", 18);
  scale_factor = height/table_size;
  
  tuioClient  = new TuioProcessing(this);
}

void draw()
{
  background(255);
  textFont(font,18*scale_factor);
  float obj_size = object_size*scale_factor; 
  float cur_size = cursor_size*scale_factor; 
  float x = 0;
  float y = 0;
   
  Vector tuioObjectList = tuioClient.getTuioObjects();
  for (int i=0;i<tuioObjectList.size();i++) {
     TuioObject tobj = (TuioObject)tuioObjectList.elementAt(i);
     Vector pointList = tobj.getPath();
     if (pointList.size()>8) {
        //TuioPoint start_point = (TuioPoint)pointList.firstElement();;
        for (int j=0;j<8;j++) {
          TuioPoint lastPoint = (TuioPoint)pointList.elementAt(pointList.size() - j - 1);
          x += lastPoint.getScreenX(width);
          y += lastPoint.getScreenY(height);
        }
        x /= 8;
        y /= 8;
     }
     stroke(0);
     fill(128);
     //ellipse( tobj.getScreenX(width), tobj.getScreenY(height),30, 30);
     ellipse( x, y, 30, 30);
     fill(0);
     //text(""+tobj.getSymbolID()+" "+tobj.getAngle(), tobj.getScreenX(width), tobj.getScreenY(height));
     text(""+tobj.getSymbolID()+" "+tobj.getAngle(), x, y);
  }
   return;
   
//   Vector tuioCursorList = tuioClient.getTuioCursors();
//   for (int i=0;i<tuioCursorList.size();i++) {
//      TuioCursor tcur = (TuioCursor)tuioCursorList.elementAt(i);
//        
//        stroke(192,192,192);
//        fill(192,192,192);
//        ellipse( tcur.getScreenX(width), tcur.getScreenY(height),30, 30);
//        fill(0);
//        text(""+ tcur.getCursorID(),  tcur.getScreenX(width)-5,  tcur.getScreenY(height)+5);
//   }
   
}

// called when an object is added to the scene
void addTuioObject(TuioObject tobj) {
  println("add object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle());
}

// called when an object is removed from the scene
void removeTuioObject(TuioObject tobj) {
  println("remove object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+")");
}

// called when an object is moved
void updateTuioObject (TuioObject tobj) {
  //println("update object "+tobj.getSymbolID()+" ("+tobj.getSessionID()+") "+tobj.getX()+" "+tobj.getY()+" "+tobj.getAngle()
  //        +" "+tobj.getMotionSpeed()+" "+tobj.getRotationSpeed()+" "+tobj.getMotionAccel()+" "+tobj.getRotationAccel());
}

// called when a cursor is added to the scene
void addTuioCursor(TuioCursor tcur) {
  println("add cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+ ") " +tcur.getX()+" "+tcur.getY());
}

// called when a cursor is moved
void updateTuioCursor (TuioCursor tcur) {
  println("update cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+ ") " +tcur.getX()+" "+tcur.getY()
          +" "+tcur.getMotionSpeed()+" "+tcur.getMotionAccel());
}

// called when a cursor is removed from the scene
void removeTuioCursor(TuioCursor tcur) {
  println("remove cursor "+tcur.getCursorID()+" ("+tcur.getSessionID()+")");
}

// called after each message bundle
// representing the end of an image frame
void refresh(TuioTime bundleTime) { 
  redraw();
}

