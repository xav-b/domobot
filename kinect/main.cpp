#include <iostream>
#include <map>
#include <list>
// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>

// Header for NITE
#include "XnVNite.h"
// local headers
#include "lib/HandTracker.cpp"
#include "lib/XEventsEmulation.cpp"
// Boost headers
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include "/home/xavier/lib/boost_1_49_0/boost/chrono.hpp"
using namespace boost::property_tree;
// OpenCV headers
#include <cv.h>
#include <highgui.h>
using namespace cv;
// TUIO protocol
#include <TuioServer.h>
using namespace TUIO;

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
		printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
		return rc;													\
	}

#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
		XnChar strError[1024];				\
		errors.ToString(strError, 1024);	\
		printf("%s\n", strError);			\
		return (rc);						\
	}

// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::HandsGenerator g_HandsGenerator;
xn::GestureGenerator g_GestureGenerator;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVHandTracker* g_pHand;

SessionState g_SessionState = NOT_IN_SESSION;

void CleanupExit()
{
	g_ScriptNode.Release();
	g_DepthGenerator.Release();
	g_HandsGenerator.Release();
	g_GestureGenerator.Release();
	g_Context.Release();

	exit (1);
}

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
	printf("Session end...\n");
	g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}
void XN_CALLBACK_TYPE TouchingCallback(xn::HandTouchingFOVEdgeCapability& generator, XnUserID id, const XnPoint3D* pPosition, XnFloat fTime, XnDirection eDir, void* pCookie)
{
	g_pHand->SetTouchingFOVEdge(id);
    //printf("[IN CALLBACK] Touching (%d)!\n", id);
}

void XN_CALLBACK_TYPE GestureIntermediateStageCompletedHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureReadyForNextIntermediateStageHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureProgressHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	printf("Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}


// xml to initialize OpenNI
#define SAMPLE_XML_PATH "/home/xavier/dev/projects/Akane/kinect/data/Sample-Tracking.xml"
// json to configure the process and algo
#define CONFIG_JSON_PATH "/home/xavier/dev/projects/Akane/kinect/data/config.json"
typedef struct handTracked handTracked;
struct handTracked {
    XnVector3D position;
    int time;
    float confidence;
    int clicked;
} g_handR;

int main(int argc, char ** argv)
{
    //boost::chrono::thread_clock::time_point start=boost::chrono::thread_clock::now();

	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// Initialize OpenNI
	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_HANDS, g_HandsGenerator);
	CHECK_RC(rc, "Find hands generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_GESTURE, g_GestureGenerator);
	CHECK_RC(rc, "Find gesture generator");

    XnFloat fSmoothing = 0.1f;
	XnCallbackHandle h;
	if (g_HandsGenerator.IsCapabilitySupported(XN_CAPABILITY_HAND_TOUCHING_FOV_EDGE))
		g_HandsGenerator.GetHandTouchingFOVEdgeCap().RegisterToHandTouchingFOVEdge(TouchingCallback, NULL, h);
    g_HandsGenerator.SetSmoothing(fSmoothing);

	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
	g_GestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
	g_GestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
	g_GestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);

    // Optionnal device config
    XnMapOutputMode outputMode;
    outputMode.nXRes = 640;
    outputMode.nYRes = 480;
    outputMode.nFPS = 30;
    DepthMetaData depthMD;
    rc = g_DepthGenerator.SetMapOutputMode(outputMode);
    CHECK_RC(rc, "Set output mode");
    g_DepthGenerator.GetMetaData(depthMD);
    //const XnDepthPixel* pDepth = depthMD.Data();
    uint16_t maxDepth = depthMD.ZRes();
    printf("[DEBUG] Max depth: %d\n", maxDepth);  // Doesn't work !

	// Create NITE objects
	g_pSessionManager = new XnVSessionManager;
	rc = g_pSessionManager->Initialize(&g_Context, "Click,Wave", "RaiseHand");
	CHECK_RC(rc, "SessionManager::Initialize");

	g_pSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);

	g_pHand = new XnVHandTracker(g_DepthGenerator); 
	g_pFlowRouter = new XnVFlowRouter;
	g_pFlowRouter->SetActive(g_pHand);

	g_pSessionManager->AddListener(g_pFlowRouter);

	g_pHand->RegisterNoPoints(NULL, NoHands);

	// Initialization done. Start generating
	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

    // Environnement initialisation ----------------------------------------------------------
    printf("[DEBUG] Process configuration (%s)\n", CONFIG_JSON_PATH);
    map<string, bool> globalConf;
    map<string, float> detectConf;
    int handToTrack(-1);
    ptree pt;
    try {
        read_json(CONFIG_JSON_PATH, pt);
        globalConf["graphic"] = pt.get("global.graphic", true);
        globalConf["debug"] = pt.get("global.debug", false);
        detectConf["grabConvexity"] = pt.get("detect.grabConvexity", 0.8);
        detectConf["epsilon"] = pt.get("detect.epsilon", 17.5);
        detectConf["maxHandRadius"] = pt.get("detect.maxHandRadius", 128);
        detectConf["tolerance"] = pt.get("detect.tolerance", 100);
        detectConf["angleMax"] = pt.get("detect.angleMax", 1);
        detectConf["cutoffCoeff"] = pt.get("detect.cutoffCoeff", 0.1f);
        detectConf["cycle"] = pt.get("detect.cycle", 20);
        detectConf["fingerDistinct"] = pt.get("detect.fingerDistinct", 0.01);
        handToTrack = pt.get("detect.handToTrack", 0);
        printf("[DEBUG] Done.\n");
    }
    catch( exception &e ) {
        cout << "[ERROR] " << e.what() << "\n";
        return 1;
    }

    // **** TUIO Init
    const bool localClientMode = pt.get("global.localClient", true);
    string clientIp = pt.get("client.ip", "127.0.0.1");
    int clientPort = pt.get("client.port", 3333);
    TuioServer* tuio;
    if ( localClientMode )
        tuio = new TuioServer();
    else
        tuio = new TuioServer(clientIp.c_str(),clientPort,false);
    TuioTime time;
    //tuio->setVerbose(globalConf["debug"]);
    int xMin = 0;
    int xMax = 640;
    int yMin = 0;
    int yMax = 480;
    int id = 3;
    //----------------------------------------------------------------------------------------
    float rh[3];
    vector<Point> handContour, fingerTips;
    unsigned char shade;
    Scalar color;
    XEventsEmulation interface;
    string trackedInfos("Tracked hand [X: 532 Y:125]\nConfidence: 0.8\nTime: 5.6s");
	// Mainloop
	while ( !xnOSWasKeyboardHit() )
	{
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		g_pSessionManager->Update(&g_Context);
		trackedInfos = PrintSessionState(g_SessionState);
        // Prepare data for opencv
        g_pHand->getPosition(rh, handToTrack);
        Mat mat(frameSize, CV_16UC1, (unsigned char *)g_DepthGenerator.GetDepthMap());
        if ( g_pHand->getContour(mat, rh, handContour, globalConf["debug"], detectConf["epsilon"], detectConf["maxHandRadius"], detectConf["tolerance"]) ) {
            bool grasp = g_pHand->computeConvex(handContour) > detectConf["grabConvexity"];
            int thickness = grasp ? CV_FILLED : 3;
            circle(depthMatBgr, Point(rh[0], rh[1]), 10, color, thickness);
            // Detection
            g_pHand->detectFingerTips(handContour, fingerTips, &depthMatBgr, detectConf["angleMax"], detectConf["cutoffCoeff"]);

            // Using boost::asio network interface
            //if ( interface.processingUI(rh, fingerTips) != 0 )
                //printf("[ERROR] Processing\n");

            // ---- TUIO transaction ---------------------------------------------------------
            time = TuioTime::getSessionTime();
            tuio->initFrame(time);
            std::list<TuioObject*> objects = tuio->getTuioObjects();
            int objects_ptr = objects.size();
            for (unsigned int i = 0; i < fingerTips.size(); i++) {
                float cursorX = (float(fingerTips[i].x) - xMin) / (xMax - xMin);
                float cursorY = (float(fingerTips[i].y) - yMin) / (yMax - yMin);
                float cursorZ = depthMat.at<float>(fingerTips[i].x, fingerTips[i].y);
                TuioObject* cursor = tuio->getClosestTuioObject(cursorX, cursorY);
                if ( cursor == NULL || cursor->getTuioTime() == time || abs(cursorX - cursor->getX()) > detectConf["fingerDistinct"] ) {
                    if ( objects.size() < 6 ) {
                        tuio->addTuioObject(i+1, cursorX, cursorY, cursorZ);
                        cout << "Create: " << i+1 << "  " << cursorX << "  " << cursorY << "  " << cursorZ << endl;
                    }
                    else
                        cout << "Tried creation: " << i+1 << "  " << cursorX << "  " << cursorY << "  " << cursorZ << endl;
                }
                else  {
                    tuio->updateTuioObject(cursor, cursorX, cursorY, cursorZ);
                    cout << "Update: " << i+1 << "  " << cursorX << "  " << cursorY << "  " << cursorZ << endl;
                }
            }
            float cursorX = (rh[0] - xMin) / (xMax - xMin);
            float cursorY = (rh[1] - yMin) / (yMax - yMin);
            float cursorZ = rh[2]/10;
            TuioObject* cursor = tuio->getClosestTuioObject(cursorX, cursorY);
            if ( cursor == NULL || cursor->getTuioTime() == time || (cursorX - cursor->getX()) > detectConf["fingerDistinct"] ) {
                if ( objects.size() < 6 ) {
                    tuio->addTuioObject(0, cursorX, cursorY, cursorZ);
                    cout << "Create main: " << 0 << "  " << cursorX << "  " << cursorY << "  " << cursorZ << endl;
                }
                else
                    cout << "Tried creation main: " << 0 << "  " << cursorX << "  " << cursorY << "  " << cursorZ << endl;
            }
            else  {
                tuio->updateTuioObject(cursor, cursorX, cursorY, cursorZ);
                cout << "Update main: " << 0 << "  " << cursorX << "  " << cursorY << "  " << cursorZ << endl;
            }
            tuio->stopUntouchedMovingObjects();
            tuio->removeUntouchedStoppedObjects();
            tuio->commitFrame();
            objects = tuio->getTuioObjects();
            cout << "Objects ptr: " << objects.size() << " / "<< objects_ptr << endl << endl;
            // -------------------------------------------------------------------------------
        }
        putText(depthMatBgr, trackedInfos, Point(rh[0]-50,rh[1]-50), FONT_HERSHEY_TRIPLEX, 1, Scalar(0, 0, 0, 0), 2);
        if ( globalConf["graphic"] )
            imshow( "depthMatBgr", depthMatBgr );
        int k = cvWaitKey(detectConf["cycle"]);
	}
	g_pSessionManager->EndSession();
	CleanupExit();
    cvDestroyAllWindows();

    //typedef boost::chrono::milliseconds ms;
    //ms d = boost::chrono::thread_clock::now() - start;
    //std::cout << ms.count() << "ms\n";
}

//TODO gHand->getDepthPoint(x, y), associer un id à chaque doigt, paramètres TUIO en fichier de conf, passer en tuioobject ou tuio3D, utiliser TuioTime pour benchmark

//TODO Configuration file (global functions (like printing stuff), segment algo and control) + Compilation conditionnelle (DEBUG/REALEASE ?)
//TODO Deportation de calcul (server, clustering et cartes graphiques à terme)
//TODO Mark when an object is tracked -> two hands (activable ? utilité ?)
//TODO GUI smockin' interface (with PrintSession, affichage et controls, processing)
//TODO Touch tests et gestion des touchs en bord d'écran
//TODO xdotool and lot of XEventEmulation changes, some more controls like pause or quit
//TODO Optimiser tout ça (avant tout mettre des tests en place, rapport ?)
//TODO S'interesser à PCL
