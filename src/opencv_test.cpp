// Copyright (c) 2023 Mokchanic, All Rights Reserved.
// Authors: JinWoo Mok

#include <iostream>
#include <opencv2/imgproc.hpp>
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
    cv::VideoCapture cap("../resource/Sub_project.avi");

    if(!cap.isOpened()){
        std::cerr << "Video open failed!" << std::endl;
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);
    [[maybe_unused]]int h[256] = {};
    cv::Mat frame, gray_scale_img, gaussian_img, canny_img;

    while(true){
        cap >> frame;
        if(frame.empty()){
            break;
        }
    
        cv::setMouseCallback("frame", on_mouse);

        cv::cvtColor(frame, gray_scale_img, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray_scale_img, gaussian_img, cv::Size(), 2.0);
        cv::Canny(gray_scale_img, canny_img, 50, 150);

        imshow("frame", frame);
        imshow("Gray_img", gray_scale_img);
        imshow("Gaussian_img", gaussian_img);

        //imshow("Canny_img", canny_img);

        // ESC is exit key
        if(cv::waitKey(delay) == 27){
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
