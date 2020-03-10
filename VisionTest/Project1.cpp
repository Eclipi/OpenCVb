//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

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
        bool bSuccess = cap.read(frame); // read a new frame from video 

        if (bSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

        cvtColor(frame, grayImage, COLOR_RGB2GRAY);

        Canny(grayImage, edgeImage, 80, 150, 3);

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