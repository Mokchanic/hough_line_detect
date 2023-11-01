// Copyright (c) 2023 Mokchanic, All Rights Reserved.
// Authors: JinWoo Mok

#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"

void on_mouse(int event, int x, int y, [[maybe_unused]]int flags, void*){
    switch(event){
    case cv::EVENT_LBUTTONDOWN:
        std::cout << "EVENT_LBUTTONDOWN: " << x << "," << y << std::endl;
        break;
    default:
        break;
    }
    return;
}

int main(){
    cv::VideoCapture cap("../resource/project_video.mp4");

    if(!cap.isOpened()){
        std::cerr << "Video open failed!" << std::endl;
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);
    
    cv::Mat frame;

    while(true){
        cap >> frame;
        if(frame.empty()){
            break;
        }
    
        cv::setMouseCallback("frame", on_mouse);
        imshow("frame", frame);

        // ESC is exit key
        if(cv::waitKey(delay) == 27){
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
