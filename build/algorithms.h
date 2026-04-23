#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>

cv::Mat method_edge_detection(const cv::Mat& img_file, int w, int h, int threshold_1, int threshold_2);
cv::Mat method_sift_detection(const cv::Mat& img_file, int w, int h, int nfeatures,
    int nlayers, double contrast_threshold, double edge_threshold);

#endif