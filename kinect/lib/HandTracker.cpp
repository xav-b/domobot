/*******************************************************************************
*                                                                              *
*   PrimeSense NITE 1.3 - Point Viewer Sample                                  *
*   Copyright (C) 2010 PrimeSense Ltd.                                         *
*                                                                              *
*******************************************************************************/

#include "HandTracker.h"
#include "XnVDepthMessage.h"
#include <XnVHandPointContext.h>
#include <sstream>

#define MAX_DEPTH 10000

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

    //DEBUG(draw hand contour
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
    m_poignet.clear();
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

	// find interior angles of hull corners
	for (int j=0; j<hull.size(); j++) {
		int idx = hull[j]; // corner index
		int pdx = idx == 0 ? handContour.size() - 1 : idx - 1; //  predecessor of idx
		int sdx = idx == handContour.size() - 1 ? 0 : idx + 1; // successor of idx

		Point v1 = handContour[sdx] - handContour[idx];
		Point v2 = handContour[pdx] - handContour[idx];
		float angle = acos( (v1.x*v2.x + v1.y*v2.y) / (norm(v1) * norm(v2)) );

        //if ( j != 0 ) {
            //Point v3 = handContour[idx] - handContour[pdx];
            //Point v4 = handContour[hull[j-1]] - handContour[pdx];
            //float angle2 = acos( (v3.x*v4.x + v3.y*v4.y) / (norm(v3) * norm(v4)) );
            //std::cout << j << " - Angle between fingers: " << angle2;
            //std::cout << " - distance x: " << handContour[idx].x - handContour[hull[j-1]].x;
            //std::cout << j << " - distance y: " << handContour[idx].y - handContour[hull[j-1]].y << std::endl;
        //}

		// low interior angle + within upper 90% of region -> we got a finger
		if (angle < angleMax && handContour[idx].y < cutoff) {
            //std::cout << j << " - Finger, angle: " << angle;
            //std::cout << ", x: " << handContour[idx].x;
            //std::cout << ", y: " << handContour[idx].y << std::endl;
			int u = handContour[idx].x;
			int v = handContour[idx].y;

			fingerTips.push_back(Point2i(u,v));
			
            //if (debugFrame) { // draw fingertips
                //cv::circle(*debugFrame, handContour[idx], 10, debugFingerTipColor, -1);
            //}
		}
        else {
            if ( handContour[idx].y > m_pointTracked.Y ) {
                int u = handContour[idx].x;
                int v = handContour[idx].y;
                m_poignet.push_back(Point2i(u,v));
                //std::cout << j << " - Wrong angle under cutoff: " << angle;
                //std::cout << ", x: " << handContour[idx].x;
                //std::cout << ", y: " << handContour[idx].y << std::endl;
            }
        }
	}
    std::cout << std::endl;
	if (debugFrame) {
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

		// dr"w approxCurve hull
        //for (int j=0; j<hull.size(); j++) {
            //cv::circle(*debugFrame, handContour[hull[j]], 10, debugFingerTipColor, 3);
            //if(j == 0) {
                //cv::line(*debugFrame, handContour[hull[j]], handContour[hull[hull.size()-1]], debugFingerTipColor);
            //} else {
                //cv::line(*debugFrame, handContour[hull[j]], handContour[hull[j-1]], debugFingerTipColor);
            //}
        //}
	}
}

double XnVHandTracker::computeConvex(const vector<Point> &contour) {
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

int XnVHandTracker::getFingerId(Blob blob, int lastId, int probableId) {
    float angleRef;
    float lengthRef;
    float angleError;
    float lengthError;
    float bestScore(1000);
    int id(-1);
    //bool flags[] = {false, false, false, false, false};
    Point pointTracked2D;
    pointTracked2D.x = m_pointTracked.X;
    pointTracked2D.y = m_pointTracked.Y;
    if ( probableId > 4 )
        probableId = 4;

    std::cout << "LastId: " << lastId << " - Probable Id: " << probableId << " (" << blob.lastGap << ")\n";
    // Here for left hand, to manage later
    for (int i = lastId+1; i <= probableId+1; i++) {
        switch(i) {
            case 1:
                angleRef = -1.43;
                lengthRef = 137;
                break;
            case 2:
                angleRef = -0.70;
                lengthRef = 165;
                break;
            case 3:
                angleRef = -0.18;
                lengthRef = 186;
                break;
            case 4:
                angleRef = 0.62;
                lengthRef = 179;
                break;
            case 5:
                angleRef = 1.6;
                lengthRef = 145;
                break;
        }
        angleError = fabs(blob.angle - angleRef);
        lengthError = fabs(blob.length - lengthRef);
        //float score = (angleError*20) + (lengthError/4);
        float score = angleError;
        if ( score < bestScore ) {
            bestScore = score;
            id = i;
        }
        //if ( angleError < 0.4 && lengthError < 20) {
            //std::cout << i << " - Found a finger: " << angleError << "  " << lengthError << " / " << blob.angle << "  " << blob.length << std::endl;
            //return i;
        //} else
            //std::cout << i << " - Nothing: " << angleError << "  " << lengthError << "  " << blob.angle << "  " << blob.length << std::endl;
    }
    return id;
}

int XnVHandTracker::fingerTipsIdentification(vector<Point> &fingerTips, Mat *debugFrame) {
	const Scalar debugFingerTipColor(255,0,0);
    Point pointTracked2D;
    pointTracked2D.x = m_pointTracked.X;
    pointTracked2D.y = m_pointTracked.Y;
    //bool flags[] = {false, false, false, false, false};
    int probableId(0);
    int lastId(0);

    Fingers.clear();

    Point axeBase = m_poignet[0];
    if ( m_poignet.size() == 0 ) {
        std::cout << "** Cannot compute Axebase: " << m_poignet.size() << std::endl;
        return 1;
    }
    else if ( m_poignet.size() > 1 ) {
        axeBase.x = ( axeBase.x + m_poignet[1].x ) / 2;
        axeBase.y = ( axeBase.y + m_poignet[1].y ) / 2;
    }
    // Replace last point, in order to order (haha) fingerTips
    while ( fingerTips[0].x > fingerTips[fingerTips.size()-1].x ) {
        fingerTips.push_back(fingerTips[0]);
        fingerTips.erase(fingerTips.begin());
    }

    cv::circle(*debugFrame, axeBase, 10, debugFingerTipColor, 3);
    cv::line(*debugFrame, axeBase, pointTracked2D, debugFingerTipColor);
    for ( int i=0; i < fingerTips.size(); i++ ) {
        // Calculate angle between new axe and finger
        Point v1 = fingerTips[i] - pointTracked2D;
        Point v2 = axeBase - pointTracked2D;
        float angle =  asin( ( v1.x*v2.y - v1.y*v2.x ) / (norm(v1) * norm(v2)) );
        float gap(0);
        float length = sqrt(pow((fingerTips[i].x - axeBase.x), 2) + pow((fingerTips[i].y - axeBase.y), 2));
        if ( i != 0 ) {
            gap = sqrt(pow((fingerTips[i].x - fingerTips[i-1].x), 2) + pow((fingerTips[i].y - fingerTips[i-1].y), 2));
            probableId = lastId + round(gap/50);
        } else
            probableId = 4;
        std::cout << i+1 << " - angle: " << angle << ", length: " << length << ", gap: " << gap << std::endl;

        // Identifying each finger
        Blob blob;
        blob.hand = 1;  //ie left hand
        blob.coordinates.X = fingerTips[i].x;
        blob.coordinates.Y = fingerTips[i].y;
        blob.coordinates.Z = depthMat.at<float>(fingerTips[i].x, fingerTips[i].y);
        blob.length = length;
        blob.lastGap = gap;
        blob.angle = angle;
        if ( (blob.id = getFingerId(blob, lastId, probableId)) > 0 ) {
            Fingers.push_back(blob);
            lastId = blob.id;
            std::ostringstream ostr;
            ostr << blob.id;
            putText(*debugFrame, ostr.str(), Point(blob.coordinates.X, blob.coordinates.Y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0, 0), 2);
            cv::circle(*debugFrame, fingerTips[i], 10, debugFingerTipColor, 3);
            cv::line(*debugFrame, pointTracked2D, fingerTips[i], debugFingerTipColor);
        }
    }
}
