#include "HandTracker.h"
#include "XnVDepthMessage.h"
#include <XnVHandPointContext.h>
#include <sstream>
#include <fstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_DEPTH 10000

int XnVHandTracker::writeSVMFormat(Mat* debugFrame, float diSymetrie) {
    int probableId(0);
    //const int lengthNominal = 120;
    //const float angleNominal = 1.7;
    //const int lengthMire1Nominal = 120;
    //const float angleMire1Nominal = 2;
    //const int lengthMire2Nominal = 150;
    //const float angleMire2Nominal = 0.5;
    const int lengthNominal = 1;
    const float angleNominal = 1;
    const int lengthMire1Nominal = 1;
    const float angleMire1Nominal = 1;
    const int lengthMire2Nominal = 1;
    const float angleMire2Nominal = 1;
    std::ofstream SVMFlux("./pySVM/SVMTest.txt");
    diSymetrie = 0;
    //std::ofstream SVMFlux("./pySVM/SVMTrain.txt", std::ios::app);
    // Saving data
    for (int j = 0; j < Fingers.size(); j++) 
        SVMFlux << j+1 << " 1:" << (Fingers[j].angle+(diSymetrie/2))/angleNominal << " 2:" << (Fingers[j].lengthMire1)/lengthMire1Nominal << " 3:" << (Fingers[j].lengthMire2)/lengthMire2Nominal << " 4:" << (Fingers[j].angleMire1)/angleMire1Nominal << " 5:" << (Fingers[j].angleMire2)/angleMire2Nominal << std::endl;
    //return 0;
    //* Computing prediction
    const char* const socket_name = "/tmp/socket";
    const char* const message = "do";
    int socket_fd;
    struct sockaddr_un name;

    socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);
    name.sun_family = AF_UNIX;
    strcpy (name.sun_path, socket_name);

    connect (socket_fd, (struct sockaddr*) &name, SUN_LEN (&name));
    int length = strlen (message) + 1;
    write (socket_fd, message, length);
    for ( int i=0; i < Fingers.size(); i++ ) {
        char buf[3];
        int recv = 0;
        recv = read(socket_fd, buf, sizeof(buf));
        if ( recv <= 1 ) {
            std::cout << "** Error reading socket\n";
            return -1;
        }
        buf[recv] = '\0';
        //std::cout << "Recieved " << buf << " (" << recv << ")\n";
        if ( i > 0 ) {
            probableId = Fingers[i-1].id + round(Fingers[i].lastGap/48);
            if ( atof(buf) != probableId ) {
                //std::cout << "** Not the probable Id\n";
                return -2;
            }
        }
        Fingers[i].id = atof(buf);
        putText(*debugFrame, buf, Point(Fingers[i].coordinates.X, Fingers[i].coordinates.Y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0, 0), 2);
        cv::circle(*debugFrame, Point(Fingers[i].coordinates.X, Fingers[i].coordinates.Y), 10, Scalar(0,0,255), 3);
    }
    close (socket_fd);
    //*/
    return 0;
}

// Colors for the points
XnFloat Colors[][3] =
{
	{0.5,0.5,0.5},	// Grey
	{0,1,0},	// Green
	{0,0.5,1},	// Light blue
	{1,1,0},	// Yellow
	{1,0.5,0},	// Orange
	{1,0,1},	// Purple
	{1,1,1}		// White. reserved for the primary point
};
XnUInt32 nColors = 6;

const Size frameSize(640, 480);
const unsigned int maxUsers = 20;
//const Mat emptyMat();

// Frame buffers
Mat depthMat(frameSize, CV_16UC1);
Mat depthMat8(frameSize, CV_8UC1);
Mat depthMatBgr(frameSize, CV_8UC3);

// Constructor. Receives the number of previous positions to store per hand,
// and a source for depth map
XnVHandTracker::XnVHandTracker(xn::DepthGenerator depthGenerator) :
	XnVPointControl("XnVHandTracker"), m_DepthGenerator(depthGenerator) {
}

// Destructor. Clear all data structures
XnVHandTracker::~XnVHandTracker() {
    std::map<XnUInt32, std::list<XnPoint3D> >::iterator iter;
    m_handsPosition.clear();
}

// Handle creation of a new hand
void XnVHandTracker::OnPointCreate(const XnVHandPointContext* cxt)
{
	printf("** Created: %d\n", cxt->nID);
	// Create entry for the hand
    //m_handsPosition[cxt->nID] = XnPoint3D Init(0, 0, 0);
	OnPointUpdate(cxt);
}
// Handle new position of an existing hand
void XnVHandTracker::OnPointUpdate(const XnVHandPointContext* cxt)
{
	// positions are kept in projective coordinates, since they are only used for drawing
	XnPoint3D ptProjective(cxt->ptPosition);
    
    //printf("Point (%f,%f,%f)", ptProjective.X, ptProjective.Y, ptProjective.Z);
	m_DepthGenerator.ConvertRealWorldToProjective(1, &ptProjective, &ptProjective);
	//printf(" -> (%f,%f,%f)\n", ptProjective.X, ptProjective.Y, ptProjective.Z);
    m_handsPosition[cxt->nID] = ptProjective;

}

// Handle destruction of an existing hand
void XnVHandTracker::OnPointDestroy(XnUInt32 nID)
{
    printf("** Destroyed: %d\n", nID);
}

XnBool XnVHandTracker::IsTouching(XnUInt32 id) const
{
	for (std::list<XnUInt32>::const_iterator iter = m_TouchingFOVEdge.begin(); iter != m_TouchingFOVEdge.end(); ++iter)
	{
		if (*iter == id)
			return TRUE;
	}
	return FALSE;
}

void XnVHandTracker::SetTouchingFOVEdge(XnUInt32 nID)
{
	m_TouchingFOVEdge.push_front(nID);
}

// Handle a new Message
void XnVHandTracker::Update(XnVMessage* pMessage)
{
	// PointControl's Update calls all callbacks for each hand
	XnVPointControl::Update(pMessage);

    std::map<XnUInt32, XnPoint3D>::const_iterator PointIterator;
    // Handle each hand
    for ( PointIterator = m_handsPosition.begin();
       PointIterator != m_handsPosition.end();
      ++PointIterator ) {
        XnUInt32 Id = PointIterator->first;
        XnUInt32 nSingle = GetPrimaryID();
        //if (Id == GetPrimaryID())
            //printf("Primary Point is: %d\n", Id);
        if (IsTouching(Id)) 
            printf("Touched ! (%d)\n", Id);
    }
	m_TouchingFOVEdge.clear();
}

void XnVHandTracker::getPosition(float *v, XnUInt32 Id) {
    std::map<XnUInt32, XnPoint3D>::const_iterator PointIterator;
    if ( Id == -1 ) {   // Second Hand
        // Handle each hand
        for ( PointIterator = m_handsPosition.begin();
           PointIterator != m_handsPosition.end();
          ++PointIterator ) {
            XnUInt32 handId = PointIterator->first;
            if ( handId != GetPrimaryID() ) {
                v[0] = PointIterator->second.X;
                v[1] = PointIterator->second.Y;
                v[2] = PointIterator->second.Z / 1000.0f;
            }
        }
    }
    else {      
        if ( Id == 0 )  // Primary hand
            Id = GetPrimaryID();
        for ( PointIterator = m_handsPosition.begin();
           PointIterator != m_handsPosition.end();
          ++PointIterator ) {
            XnUInt32 handId = PointIterator->first;
            if ( Id == handId ) {
                v[0] = PointIterator->second.X;
                v[1] = PointIterator->second.Y;
                v[2] = PointIterator->second.Z / 1000.0f;
            }
        }
    }
}

std::string PrintSessionState(SessionState eState)
{
	switch (eState)
	{
	case IN_SESSION:
		return "Tracking hands";
	case NOT_IN_SESSION:
		return "Perform click or wave gestures to track hand";
	case QUICK_REFOCUS:
		return "Raise your hand for it to be identified, or perform click or wave gestures";
	}
    return "failed...";
}


bool XnVHandTracker::getContour(const Mat mat, const float *v, vector<Point> &handContour, bool debug = false, const double epsilon = 17.5, const int maxHandRadius = 128, int distance = 100, float maxDepth = 3000.0f) {
    m_pointTracked.X = v[0];
    m_pointTracked.Y = v[1];
    m_pointTracked.Z = v[2]*1000.0f;

    mat.copyTo(depthMat);
    depthMat.convertTo(depthMat8, CV_8UC1, 255.0f / maxDepth);
    cvtColor(depthMat8, depthMatBgr, CV_GRAY2BGR);
    char shade = 255 - (unsigned char)(v[2] * 128.0f);
    m_color = Scalar(0, 0, shade);
    handContour.clear();

	//const int maxHandRadius = 128; // in px
	//const short handDepthRange = 200; // in mm
	//const double epsilon = 17.5; // approximation accuracy (maximum distance between the original hand contour and its approximation)

	unsigned short depth = (unsigned short) (v[2] * 1000.0f); // hand depth
	unsigned short near = depth - distance; // near clipping plane
	unsigned short far = depth + distance; // far clipping plane

	static Mat mask(frameSize, CV_8UC1);
	mask.setTo(0);

	// extract hand region	
	circle(mask, Point(v[0], v[1]), maxHandRadius, 255, CV_FILLED);
	mask = mask & depthMat > near & depthMat < far;

	// DEBUG(show mask)
    if ( debug )
	    imshow("mask1", mask);

	// assume largest contour in hand region to be the hand contour
	vector<vector<Point> > contours;
	findContours(mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	int n = contours.size();
	int maxI = -1;
	int maxSize = -1;
	for (int i=0; i<n; i++) {
		int size  = contours[i].size();
		if (size > maxSize) {
			maxSize = size;
			maxI = i;
		}
	}

	bool handContourFound = (maxI >= 0);

	if (handContourFound) {
		approxPolyDP( Mat(contours[maxI]), handContour, epsilon, true );
		//handContour = contours[maxI];
	}

    //DEBUG draw hand contour
    if ( debug ) {
        mask.setTo(0);	
        if (maxI >= 0) {
            vector<vector<Point> > contours2;
            contours2.push_back(contours[maxI]);
            drawContours(mask, contours2, -1, 255);
            imshow("mask2", mask);
        }
    }
	return maxI >= 0;
}

void XnVHandTracker::detectFingerTips(const vector<Point> &handContour, vector<Point> &fingerTips, Mat *debugFrame = NULL, float angleMax = 1, float cutoffCoeff = 0.1f) {
    fingerTips.clear();
    //m_poignet.clear();
	Mat handContourMat(handContour);
	double area = cv::contourArea(handContourMat);

	const Scalar debugFingerTipColor(255,0,0);

	vector<int> hull;
	cv::convexHull(handContourMat, hull);

	// find upper and lower bounds of the hand and define cutoff threshold (don't consider lower vertices as fingers)
	int upper = 640, lower = 0;
	for (int j=0; j<hull.size(); j++) {
		int idx = hull[j]; // corner index
		if (handContour[idx].y < upper) upper = handContour[idx].y;
		if (handContour[idx].y > lower) lower = handContour[idx].y;
	}
	float cutoff = lower - (lower - upper) * cutoffCoeff;

    int firstIdx(0);
	// find interior angles of hull corners
	for (int j=0; j<hull.size(); j++) {
		int idx = hull[j]; // corner index
		int pdx = idx == 0 ? handContour.size() - 1 : idx - 1; //  predecessor of idx
		int sdx = idx == handContour.size() - 1 ? 0 : idx + 1; // successor of idx

		Point v1 = handContour[sdx] - handContour[idx];
		Point v2 = handContour[pdx] - handContour[idx];
		float angle = acos( (v1.x*v2.x + v1.y*v2.y) / (norm(v1) * norm(v2)) );

		// low interior angle + within upper 90% of region -> we got a finger
		if (angle < angleMax && handContour[idx].y < cutoff) {
			int u = handContour[idx].x;
			int v = handContour[idx].y;
			fingerTips.push_back(Point2i(u,v));
		}
        //else {
            //if ( handContour[idx].y > m_pointTracked.Y+100 ) {
                //if ( m_poignet.empty() )
                    //firstIdx = idx-1;
                //int u = handContour[idx].x;
                //int v = handContour[idx].y;
                //m_poignet.push_back(Point2i(u,v));
            //}
        //}
    }
    //int size(0);
    //if ( firstIdx >= 0 && m_poignet.size() != 0) 
        //cv::line(*debugFrame, Point(handContour[firstIdx].x, handContour[firstIdx].y), Point(handContour[firstIdx+1].x, handContour[firstIdx+1].y), debugFingerTipColor);

    if (debugFrame) {
    bool debug = false;
    //if (debug) {
		// draw cutoff threshold
        cv::line(*debugFrame, Point(0, cutoff), Point(640, cutoff), debugFingerTipColor);

		// draw approxCurve
        //for (int j=0; j<handContour.size(); j++) {
            //cv::circle(*debugFrame, handContour[j], 10, debugFingerTipColor);
            //if (j != 0) {
                //cv::line(*debugFrame, handContour[j], handContour[j-1], debugFingerTipColor);
            //} else {
                //cv::line(*debugFrame, handContour[0], handContour[handContour.size()-1], debugFingerTipColor);
            //}
        //}

		// draw approxCurve hull
        for (int j=0; j<hull.size(); j++) {
            cv::circle(*debugFrame, handContour[hull[j]], 10, debugFingerTipColor, 3);
            if(j == 0) {
                cv::line(*debugFrame, handContour[hull[j]], handContour[hull[hull.size()-1]], debugFingerTipColor);
            } else {
                cv::line(*debugFrame, handContour[hull[j]], handContour[hull[j-1]], debugFingerTipColor);
            }
        }
	}
}

double XnVHandTracker::computeConvex(const vector<Point> &contour, Mat *debugFrame = NULL) {
	Mat contourMat = Mat(contour);

	vector<int> hull;
	convexHull(contourMat, hull);

	int n = hull.size();
	vector<Point> hullContour;

	for (int i=0; i<n; i++) {
		hullContour.push_back(contour[hull[i]]);
	}

    Mat hullContourMat(hullContour);

	return (contourArea(contourMat) / contourArea(hullContourMat));
}

int XnVHandTracker::fingerTipsIdentification(vector<Point> &fingerTips, Point centroid, Mat *debugFrame) {
	const Scalar debugFingerTipColor(255,0,0);
    float diSymetrie= 0;
    Fingers.clear();

    if ( fingerTips.size() < 1 || fingerTips.size() > 5) 
        return 2;
    
    Point stableRef;
    stableRef.x = m_pointTracked.X;
    stableRef.y = m_pointTracked.Y;

    Point axeX;
    Point firstMire;
    Point secondMire;
    axeX.x = m_pointTracked.X + 60;
    axeX.y = m_pointTracked.Y;
    Point w2 = axeX - stableRef;
    Point w1 = centroid - stableRef;
    float angleRepere = acos( ( w1.x*w2.x + w1.y*w2.y ) / (norm(w1) * norm(w2)) );
    float secondAngle = angleRepere + (3.14/2);
    float firstAngle = angleRepere - (3.14/2);
    secondMire.x = (axeX.x - stableRef.x)*cos(secondAngle) - (axeX.y - stableRef.y)*sin(secondAngle) + stableRef.x;
    secondMire.y = (axeX.x - stableRef.x)*sin(secondAngle) + (axeX.y - stableRef.y)*cos(secondAngle) + stableRef.y;
    firstMire.x = (axeX.x - stableRef.x)*cos(firstAngle) - (axeX.y - stableRef.y)*sin(firstAngle) + stableRef.x;
    firstMire.y = (axeX.x - stableRef.x)*sin(firstAngle) + (axeX.y - stableRef.y)*cos(firstAngle) + stableRef.y;
    //float coeffDir = (float)(float((centroid.x - stableRef.x)) / float((centroid.y - stableRef.y)));
    //int t = 5;
    //secondMire.x = round(stableRef.x + t*(centroid.y - stableRef.y));
    //secondMire.y = round(stableRef.y + t*(centroid.x - stableRef.x));
    cv::circle(*debugFrame, firstMire, 10, Scalar(0,255,0), 3);
    cv::circle(*debugFrame, secondMire, 10, Scalar(0,255,0), 3);
    cv::line(*debugFrame, axeX, stableRef, debugFingerTipColor);
    cv::line(*debugFrame, secondMire, stableRef, debugFingerTipColor);
    cv::line(*debugFrame, firstMire, stableRef, debugFingerTipColor);

    // Replace last point, in order to order (haha) fingerTips
    while ( fingerTips[0].x > fingerTips[fingerTips.size()-1].x ) {
        fingerTips.push_back(fingerTips[0]);
        fingerTips.erase(fingerTips.begin());
    }

    cv::circle(*debugFrame, centroid, 10, debugFingerTipColor, 3);
    cv::line(*debugFrame, centroid, stableRef, debugFingerTipColor);
    for ( int i=0; i < fingerTips.size(); i++ ) {
        // Calculate angle between secondMire and finger
        Point v12 = fingerTips[i] - secondMire;
        Point v22 = stableRef - secondMire;
        float angle2 =  asin( ( v12.x*v22.y - v12.y*v22.x ) / (norm(v12) * norm(v22)) );
        float length2 = sqrt(pow((fingerTips[i].x - secondMire.x), 2) + pow((fingerTips[i].y - secondMire.y), 2));
        //std::cout << i+1 << " - angle2: " << angle2 << ", length2: " << length2 << std::endl;
        // Calculate angle between secondMire and finger
        Point v11 = fingerTips[i] - firstMire;
        Point v21 = stableRef - firstMire;
        float angle1 =  asin( ( v11.x*v21.y - v11.y*v21.x ) / (norm(v11) * norm(v21)) );
        float length1 = sqrt(pow((fingerTips[i].x - firstMire.x), 2) + pow((fingerTips[i].y - firstMire.y), 2));
        //std::cout << i+1 << " - angle1: " << angle1 << ", length1: " << length1 << std::endl;

        // Calculate angle between new axe and finger
        Point v1 = fingerTips[i] - stableRef;
        Point v2 = centroid - stableRef;
        float angle =  asin( ( v1.x*v2.y - v1.y*v2.x ) / (norm(v1) * norm(v2)) );
        diSymetrie += angle;
        float gap(0);
        float length = sqrt(pow((fingerTips[i].x - centroid.x), 2) + pow((fingerTips[i].y - centroid.y), 2));
        if ( i != 0 ) 
            gap = sqrt(pow((fingerTips[i].x - fingerTips[i-1].x), 2) + pow((fingerTips[i].y - fingerTips[i-1].y), 2));
        //std::cout << i+1 << " - angle: " << angle << ", length: " << length << ", gap: " << gap << std::endl;

        // Identifying each finger
        Blob blob;
        blob.hand = 1;  //ie left hand
        blob.coordinates.X = fingerTips[i].x;
        blob.coordinates.Y = fingerTips[i].y;
        blob.coordinates.Z = depthMat.at<float>(fingerTips[i].x, fingerTips[i].y);
        blob.length = length;
        blob.angle = angle;
        blob.lengthMire1 = length1;
        blob.angleMire1 = angle1;
        blob.lengthMire2 = length2;
        blob.angleMire2 = angle2;
        blob.lastGap = gap;

        //if ( (blob.id = getFingerId(blob, lastId, probableId)) > 0 ) {
        Fingers.push_back(blob);
    }
    //std::cout << "Disymetrie meter: " << diSymetrie << std::endl;
    if ( writeSVMFormat(debugFrame, diSymetrie) < 0 )
        return -1;
    return 0;
}
