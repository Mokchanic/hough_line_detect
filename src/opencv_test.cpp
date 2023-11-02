// Copyright (c) 2023 Mokchanic, All Rights Reserved.
// Authors: JinWoo Mok

#include <iostream>
#include <opencv2/core/matx.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"

// void on_mouse(int event, int x, int y, [[maybe_unused]]int flags, void*){
//     switch(event){
//     case cv::EVENT_LBUTTONDOWN:
//         std::cout << "EVENT_LBUTTONDOWN: " << x << "," << y << std::endl;
//         break;
//     default:
//         break;
//     }
//     return;
// }

cv::Mat RegionOfInterest(cv::Mat canny_img, cv::Point *points){
    cv::Mat img_mask = cv::Mat::zeros(canny_img.rows, canny_img.cols, CV_8UC1);;

    cv::Scalar ignore_mask_color = cv::Scalar(255, 255, 255);
	const cv::Point* ppt[1] = { points };
	int npt[] = { 4 };

	fillPoly(img_mask, ppt, npt, 1, cv::Scalar(255, 255, 255), cv::LINE_8);

	cv::Mat img_masked;
	bitwise_and(canny_img, img_mask, img_masked);

    return img_masked;
}

int main(){
    cv::VideoCapture cap("../resource/Sub_project.avi");

    if(!cap.isOpened()){
        std::cerr << "Video open failed!" << std::endl;
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    int delay = cvRound(1000 / fps);

    cv::Point points[4];
    points[0] = cv::Point(190, 290);
    points[1] = cv::Point(30, 400);
    points[2] = cv::Point(610, 400);
    points[3] = cv::Point(450, 290);

    cv::Mat frame, gray_scale_img, gaussian_img, canny_img, img_edge, hough_img, line_img;
    line_img = cv::Mat::zeros(cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH)),\
                             cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT)), CV_8UC3);
    
    std::vector<cv::Vec4i> lines;
    
    float hough_rho = 2;
    float hough_theta = CV_PI/180;
    float hough_threshold = 15;
    float min_line_length = 10;
    float max_line_length = 20;

    while(true){
        cap >> frame;
        if(frame.empty()){
            break;
        }
    
        //cv::setMouseCallback("frame", on_mouse);

        cv::cvtColor(frame, gray_scale_img, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray_scale_img, gaussian_img, cv::Size(), 2.0);
        cv::Canny(gray_scale_img, canny_img, 230, 240);
        img_edge = RegionOfInterest(canny_img, points);
        img_edge.copyTo(hough_img);
        cv::HoughLinesP(hough_img, lines, hough_rho, hough_theta, hough_threshold, min_line_length, max_line_length);

        for(cv::Vec4i line : lines){
            cv::line(frame, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        }

        imshow("frame", frame);
        imshow("Gray_img", gray_scale_img);
        imshow("Gaussian_img", gaussian_img);
        imshow("Canny_img", canny_img);
        imshow("Roi_img", img_edge);

        // ESC is exit key
        if(cv::waitKey(delay) == 27){
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
