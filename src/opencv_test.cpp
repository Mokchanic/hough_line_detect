// Copyright (c) 2023 Mokchanic, All Rights Reserved.
// Authors: JinWoo Mok

#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "opencv2/opencv.hpp"

cv::Mat RegionOfInterest(cv::Mat canny_img){
    const int x_ = 255;
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

    imshow("Mask_img", img_mask);

    return img_masked;
}

int main(){
    cv::VideoCapture cap("../resource/Sub_project.avi");

    if(!cap.isOpened()){
        std::cerr << "Video open failed!" << std::endl;
        return -1;
    }

    const double fps = cap.get(cv::CAP_PROP_FPS);
    const int delay = cvRound(1000 / fps);
    const int width = cvRound(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    const int y_offset = 400;
    const cv::Point p1(0, y_offset), p2(width, y_offset);

    cv::Mat frame, dst, gray_scale_img, gaussian_img, canny_img, img_edge, hough_img;
    
    std::vector<cv::Vec4i> lines;
    std::vector<int> v_l_pos;
    std::vector<int> v_r_pos;
    
    float hough_rho = 2;
    float hough_theta = CV_PI/180;
    float hough_threshold = 15;
    float min_line_length = 10;
    float max_line_length = 20;

    int prev_l_pos = 0;
    int prev_r_pos = 0;
    int cur_l_pos = 0;
    int cur_r_pos = 0;

    while(true){
        cap >> frame;
        if(frame.empty()){
            break;
        }
    
        cv::cvtColor(frame, gray_scale_img, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray_scale_img, gaussian_img, cv::Size(), 1.0);
        cv::Canny(gaussian_img, canny_img, 240, 250);
        img_edge = RegionOfInterest(canny_img);
        img_edge.copyTo(hough_img);
        cv::HoughLinesP(hough_img, lines, hough_rho, hough_theta, hough_threshold, min_line_length, max_line_length);
        
        dst = frame.clone();
        cv::line(dst, p1, p2, cv::Scalar(255, 0, 221), 2, cv::LINE_AA);
        cv::circle(dst, cv::Point(width / 2, y_offset), 10, cv::Scalar(255, 0, 221), 2, cv::LINE_AA);

        for(cv::Vec4i line : lines){
            if(line[2] < width / 2 && line[0] < width / 2){
                if(v_l_pos.size() != 10){
                    v_l_pos.push_back(line[2]);
                    v_l_pos.push_back(line[0]);
                }
                else{
                    int sum = 0;
                    for(int num : v_l_pos){
                        sum += num;
                    }
                    cur_l_pos = sum / 10;

                    if(prev_l_pos == 0){
                        prev_l_pos = cur_l_pos;
                    }

                    if(abs(prev_l_pos - cur_l_pos) < 70){
                        prev_l_pos = cur_l_pos;
                    }
                    v_l_pos.clear();
                }
            }
            else if(line[2] > width / 2 && line[0] > width / 2){
                if(v_r_pos.size() != 10){
                    v_r_pos.push_back(line[2]);
                    v_r_pos.push_back(line[0]);
                }
                else{
                    int sum = 0;
                    for(int num : v_r_pos){
                        sum += num;
                    }
                    cur_r_pos = sum / 10;

                    if(prev_r_pos == 0){
                        prev_r_pos = cur_r_pos;
                    }

                    if(abs(prev_r_pos - cur_r_pos) < 70){
                        prev_r_pos = cur_r_pos;
                    }
                    v_r_pos.clear();
                }
            }
        }

        cv::circle(dst, cv::Point(prev_l_pos, y_offset), 5, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        cv::circle(dst, cv::Point(prev_r_pos, y_offset), 5, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
        cv::circle(dst, cv::Point((prev_l_pos + prev_r_pos) / 2, y_offset), 5, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);

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
