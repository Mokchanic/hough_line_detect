// Copyright (c) 2023 Mokchanic, All Rights Reserved.
// Authors: JinWoo Mok

#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//변환 좌표를 알기 위해 사용
// void on_mouse(int event, int x, int y, int flags, void*){
//     switch(event){
//     case EVENT_LBUTTONDOWN:
//         cout << "EVENT_LBUTTONDOWN: " << x << "," << y << endl;
//         break;
//     default:
//         break;
//     }
//     return;
// }

int main()
{
    VideoCapture cap("data/project_video.mp4");

    if(!cap.isOpened()){
        cerr << "Video open failed!" << endl;
        return -1;
    }

    double fps = cap.get(CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);
    int width = cvRound(cap.get(CAP_PROP_FRAME_WIDTH));
    int height = cvRound(cap.get(CAP_PROP_FRAME_HEIGHT));
    
    Mat frame, imgUndistort, Mat_x, img_warp;

    while(true){
        cap >> frame;
        if(frame.empty()) break;
    
        //setMouseCallback("frame", on_mouse);
        imshow("frame", frame);

        // ESC is exit key
        if(waitKey(delay) == 27) break;
    }
    destroyAllWindows();
    return 0;
}