#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>

cv::Mat method_edge_detection(const cv::Mat& img_file, int w, int h);
cv::Mat method_sift_detection(const cv::Mat& img_file, int w, int h);

#endif