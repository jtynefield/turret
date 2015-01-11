#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;


void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    double scale);
string cascadeName = "./haarcascade_frontalface_alt.xml";

int main( int argc, const char** argv ) {
    CascadeClassifier cascade;
    if( !cascade.load( cascadeName ) ) {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

    CvCapture* capture = cvCaptureFromCAM( 0 );
    if(!capture) {
        cerr << "Capture from CAM " <<  0  << " didn't work" << endl;
        return -1;
    }

    // todo, figure out how to make a fullscreen window.
    cvNamedWindow( "result", 1 );

    for(;;)  {
        IplImage* iplImg = cvQueryFrame( capture );
        Mat frame = iplImg;
        if( frame.empty() )
            break;

        Mat frameCopy;
        if( iplImg->origin == IPL_ORIGIN_TL )
            frame.copyTo( frameCopy );
        else
            flip( frame, frameCopy, 0 );
        
        detectAndDraw( frameCopy, cascade, 2.0 );
        
        if( waitKey( 10 ) >= 0 )
            goto _cleanup_;
    }
    
    waitKey(0);

_cleanup_:
    cvReleaseCapture( &capture );
    cvDestroyWindow("result");
    return 0;
}



void detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    double scale ) {
    int i = 0;
    double t = 0;
    vector<Rect> faces;
    const static Scalar colors[] =  { CV_RGB(0,0,255),
                                      CV_RGB(0,128,255),
                                      CV_RGB(0,255,255),
                                      CV_RGB(0,255,0),
                                      CV_RGB(255,128,0),
                                      CV_RGB(255,255,0),
                                      CV_RGB(255,0,0),
                                      CV_RGB(255,0,255)};

    Mat gray, smallImg( cvRound (img.rows/scale), cvRound(img.cols/scale), CV_8UC1 );

    cvtColor( img, gray, CV_BGR2GRAY );
    resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    double minv, maxv;
    Point minp, maxp;
    minMaxLoc( smallImg, &minv, &maxv, &minp, &maxp );

    t = (double)cvGetTickCount();
    cascade.detectMultiScale( smallImg, faces,
        1.1, 2, 0
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        |CV_HAAR_SCALE_IMAGE
        ,
        Size(30, 30) );

    t = (double)cvGetTickCount() - t;
    printf( "detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );

    for( vector<Rect>::const_iterator r = faces.begin(); r != faces.end(); r++, i++ )
    {
        Mat smallImgROI;
        Point center;
        Scalar color = colors[i%8];
        int radius;

        double aspect_ratio = (double)r->width/r->height;
        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
        else
            rectangle( img, cvPoint(cvRound(r->x*scale), cvRound(r->y*scale)),
                       cvPoint(cvRound((r->x + r->width-1)*scale), cvRound((r->y + r->height-1)*scale)),
                       color, 3, 8, 0);
    }
    cv::imshow( "result", img );
}
