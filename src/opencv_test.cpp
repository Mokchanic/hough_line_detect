// Copyright (c) 2023 Mokchanic, All Rights Reserved.
// Authors: JinWoo Mok

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

#include "opencv2/opencv.hpp"

cv::Mat RegionOfInterest(cv::Mat canny_img){
    const int x_ = 250;
    const cv::Point p1_(0, 390), p2_(x_, 410);
    const cv::Point p3_(640 - x_, 390), p4_(640, 410);
    const cv::Rect rc1(p1_, p2_);
    const cv::Rect rc2(p3_, p4_);

    cv::Mat img_mask = cv::Mat::zeros(canny_img.rows, canny_img.cols, CV_8UC1);
    cv::Scalar ignore_mask_color = cv::Scalar(255, 255, 255);

    cv::rectangle(img_mask, rc1, ignore_mask_color, CV_FILLED, 0);
    cv::rectangle(img_mask, rc2, ignore_mask_color, CV_FILLED, 0);

	cv::Mat img_masked;
	bitwise_and(canny_img, img_mask, img_masked);

    imshow("frame", img_mask);
    imshow("Gray_img", img_masked);

    return img_masked;
}

int main(){
    cv::VideoCapture cap("../resource/Sub_project.avi");

    if(!cap.isOpened()){
        std::cerr << "Video open failed!" << std::endl;
        return -1;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    const int delay = cvRound(1000 / fps);
    const int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    //const int height = cvRound(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    const cv::Point p1(0, 400), p2(width, 400);

    cv::Mat frame, dst, gray_scale_img, gaussian_img, canny_img, img_edge, hough_img;
    
    std::vector<cv::Vec4i> lines(4);
    
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
    
        cv::cvtColor(frame, gray_scale_img, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray_scale_img, gaussian_img, cv::Size(), 3.0);
        cv::Canny(gray_scale_img, canny_img, 240, 250);
        img_edge = RegionOfInterest(canny_img);
        img_edge.copyTo(hough_img);
        cv::HoughLinesP(hough_img, lines, hough_rho, hough_theta, hough_threshold, min_line_length, max_line_length);

        dst = frame.clone();
        cv::line(dst, p1, p2, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
        for(cv::Vec4i line : lines){
            cv::line(dst, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
            // std::cout << line[0] << " " << line[1];
            // std::cout << line[2] << " " << line[3] << std::endl;
        }

        imshow("frame", frame);
        imshow("Gray_img", gray_scale_img);
        imshow("Gaussian_img", gaussian_img);
        imshow("Canny_img", canny_img);
        imshow("Roi_img", img_edge);
        imshow("Dst_img", dst);


        // ESC is exit key
        if(cv::waitKey(delay) == 27){
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
