#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include "D8MCapture.h"
#include "hps_0.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <unistd.h>
#include <iostream>

using namespace cv;
using namespace std;

#ifndef CAPTURE_RAM_DEVICE
#define CAPTURE_RAM_DEVICE "/dev/f2h-dma-memory"
#endif /* ifndef CAPTURE_RAM_DEVICE */


// Function Used for Finding Image Format 
string openCVType2str(int type) {
    string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}
int calXcoor(cv::Mat org, int location)
{
    int x; 

    switch (location)
    {
    case 1: 
         x = 0; 
        break;
    case 2:
        x = 800 - org.cols; 
        break;
    case 3:
        x = 0;
        break;
    case 4:
        x = 800 - org.cols;
        break; 



    }
    return x; 
}
int calYcoor(cv::Mat org, int location)
{
    int y; 
    switch (location)
    {
    case 1:
        y = 0;
        break;
    case 2:
        y = 0; 
        break;
    case 3:
        y = 480 - org.rows;
        break;
    case 4:
        y = 480 - org.rows;
        break;
    }

    return y; 
}




// Main 
int main()
{
   
    while (1)
    {
        destroyAllWindows();
        int overLayLocation;
        cout << "\nTop Left [1]\nTop Right [2]\nBottom Left[3]\nBottom Right[4]\nEXIT[5]\n" << endl;
        cout << "Enter Desired OverLay Image Placement: ";
        cin >> overLayLocation;
       // destroyAllWindows();

        if (overLayLocation == 1 || overLayLocation == 2 || overLayLocation == 3 || overLayLocation == 4)
        {
            Mat src;
            D8MCapture* cap = new D8MCapture(TV_DECODER_TERASIC_STREAM_CAPTURE_BASE, CAPTURE_RAM_DEVICE);
            namedWindow("camera");
            
            while (1) {
                if (!cap->read(src))
                    return -1;



                Mat dst;
                Mat img = imread("/home/root/Desktop/assg4/ImageFromClient.bmp", IMREAD_UNCHANGED);
                usleep(3);

                // overlay img has 3 channels || regular img as 1 channel 
                // live video is 8UC4 USER image is 8UC1
                //The Next to If Statements Matches the Overlay imaeg channel to Live Video Channel 

                if (img.channels() == 3) //overlayed images 
                {
                    cvtColor(img, dst, COLOR_BGR2BGRA); // Color_BGR2BGRA will alway be 4 channels and 8U format 
                    dst.copyTo(src(cv::Rect(calXcoor(dst,overLayLocation), calYcoor(dst,overLayLocation), img.cols, img.rows))); // overlay converted on live video 

                }
                if (img.channels() == 1) // regualr imgs 
                {
                    Mat channels[4] = { img,img,img,img };
                    merge(channels, 4, dst);  // convert 1 channel to 4 
                    dst.copyTo(src(cv::Rect(calXcoor(dst, overLayLocation), calYcoor(dst, overLayLocation), img.cols, img.rows))); // overlay converted on live video X,Y format 

                }


                imshow("camera", src); // window namevc == camera 
                 




                //-- bail out if escape was pressed
                int c = waitKey(10);
                if ((char)c == 27) {
                    break;
                }
            }
            delete cap;
            destroyAllWindows();
            destroyWindow("camera");
            
        }
        if (overLayLocation == 5)
        {

            return 0; 
        }
        else
        {
            cout << "Invalid Input Try Again " << endl;
        }

    }

}
