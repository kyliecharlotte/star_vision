#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

#include "display.h"
#include "algorithms.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>
#include <getopt.h>
#include <optional>

/**
 * @brief Run the Canny Edge Detection algorithm on an image and optionally save the results as a new image
 * 
 * @param img_file Input file path representing image that the method is to be applied to 
 * @param out_file Output file path representing where the image will be saved (empty if display only)
 * @return cv::Mat 
 */
cv::Mat method_edge_detection(const cv::Mat& img_file, int w, int h, int threshold_1, int threshold_2){

    //cv::Mat image = cv::imread(img_file); // Try to load image

    std::cout << "\nCanny Edge Detection" << std::endl;

    cv::Mat img_gray, final;

    cv::cvtColor(img_file, img_gray, cv::COLOR_BGR2GRAY); // Apply gray scale conversion for canny algorithm

    //threshold arguments: threshold2 ~ threshold1 * 2/3
    //threshold2: definitely an edge, threshold 1: maybe an edge
    cv::Canny(img_gray, final, threshold_1, threshold_2);

    cv::resize(final, final, cv::Size(w,h));

    return final;
};

/**
 * @brief Run the SIFT algorithm on an image and optionally save the results as a new image
 * 
 * @param img_file Input file path representing image that the method is to be applied to 
 * @param out_file Output file path representing where the image will be saved (empty if display only)
 * @return int 
 */
cv::Mat method_sift_detection(const cv::Mat& img_file, int w, int h, int nfeatures,
                            int nlayers, double contrast_threshold, double edge_threshold){

    cv::Mat img_gray, final;

    if (img_file.channels() == 3) {
        cv::cvtColor(img_file, img_gray, cv::COLOR_BGR2GRAY); // gray scale conversion to prepare for sift
    } else {
        img_gray = img_file.clone();
    }

    const auto sift = cv::SIFT::create(nfeatures=nfeatures, nlayers,
                                        contrast_threshold, edge_threshold);
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    sift->detectAndCompute(img_gray, cv::noArray(), keypoints, descriptors);
    cv::Mat output;
    cv::drawKeypoints(img_gray, keypoints, output);

    cv::resize(output, output, cv::Size(w,h));
    return output;

};