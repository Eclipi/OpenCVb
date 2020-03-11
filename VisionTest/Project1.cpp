//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

float trap_bottom_width = 0.85;  // width of bottom edge of trapezoid, expressed as percentage of image width
float trap_top_width = 0.07;     // ditto for top edge of trapezoid
float trap_height = 0.4;


Mat region_of_interest(Mat img_edges, Point *points)
{
    Mat img_mask = Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);

    Scalar ignore_mask_color = Scalar(255, 255, 255);
    const Point* ppt[1] = { points };
    int npt[] = { 4 };

    fillPoly(img_mask, ppt, npt, 1, Scalar(255, 255, 255), LINE_8);

    Mat img_masked;
    bitwise_and(img_edges, img_mask, img_masked);

    return img_masked;
}


int main(int argc, char* argv[])
{
    //Open the default video camera
    char buf[256];
    char buf2[256];
    int index = 0;
    VideoCapture cap(0);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

    string window_name = "My Camera Feed";
    namedWindow(window_name, WINDOW_AUTOSIZE); //create a window called "My Camera Feed"
    namedWindow("edgeImage", WINDOW_AUTOSIZE);

    while (true)
    {
        Mat frame, grayImage, edgeImage;
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        bool bSuccess = cap.read(frame); // read a new frame from video 

        if (bSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

        cvtColor(frame, grayImage, COLOR_RGB2GRAY);

        Canny(grayImage, edgeImage, 80, 150, 3);

       //영역 설정
        Point points[4];
        points[0] = Point((dWidth * (1 - trap_bottom_width)) / 2, dHeight);
        points[1] = Point((dWidth * (1 - trap_top_width)) / 2, dHeight - dHeight * trap_height);
        points[2] = Point(dWidth - (dWidth * (1 - trap_top_width)) / 2, dHeight - dHeight * trap_height);
        points[3] = Point(dWidth - (dWidth * (1 - trap_bottom_width)) / 2, dHeight);

        edgeImage = region_of_interest(edgeImage, points);
      
        //Breaking the while loop if the frames cannot be captured
        
        //show the frame in the created window
        imshow(window_name, frame);

        imshow("edgeImage", edgeImage);
        //wait for for 10 ms until any key is pressed.  
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop 
        //If any key is not pressed withing 10 ms, continue the loop
        if (waitKey(10) == 27)
        {
            cout << "Esc key is pressed by user. Stoppig the video" << endl;
            break;
        }

        //capture by pressing ,
        if (waitKey(10) == 46)
        {
            
            sprintf_s(buf, "D:/Code/Junk/MyImage%06d.jpg", index);
            sprintf_s(buf2, "D:/Code/Junk/MyEdgeImage%06d.jpg", index);
            cout << buf << endl;
            cout << buf2 << endl;
            bool isSuccess = imwrite(buf, frame);
            imwrite(buf2, edgeImage);

            index++;

            if (isSuccess == false)
            {
                cout << "Failed to save the image" << endl;
                cin.get();
                return -1;
            }
        }
    }

    return 0;

}