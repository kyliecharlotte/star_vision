#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>
#include <getopt.h>
#include <optional>

// SET UP

// short flag options -- -h (help), -m, -i, -o 
const char* const short_opt = ":hm:i:o:";
const struct option long_opt[] = {
    {"help", 0, NULL, 'h'},
    {"method", 1, NULL, 'm'},
    {"input", 1, NULL, 'i'},
    {"output", 1, NULL, 'o'},
    {NULL, 0, NULL, 0}
};

struct options {
    std::optional<std::vector<std::string>> input;
    std::optional<std::string> output;
    std::optional<char> method;
};

std::optional<char> parse_method(std::string_view s) {
    std::cout << "method work" << std::endl;
    if (s == "e") return 'e';
    if (s == "y") return 'y';
    if (s == "") return 'z';
    return std::nullopt;
}

std::optional<std::vector<std::string>> parse_input(char *s) {
    //std::string s = s;
    const char *delim = ",";
    std::vector<std::string> input_list;

    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, ',')) {
        input_list.push_back(token);
    }

    std::cout << "input work" << std::endl;

    if (input_list.size() > 0) {
        return input_list;
    } else {
        return std::nullopt;
    }

}

void help_func(char *call) {
    std::cout << "Usage: " << call << '\n';
    std::cout << "Options:" << '\n';
    std::cout << "\t-h or --help: Display Help information" << '\n';
    std::cout << "\t-m or --method: Method to be Applied to Image" << '\n';
    std::cout << "\t\t Method 1: Edge Detection" << '\n';
    std::cout << "\t\t\t Smooths the image to reduce noise, useful for object detection" << '\n';
    std::cout << "\t-i or --input: Input File" << '\n';
    std::cout << "\t-o or --output: Output File" << '\n';
}

int load_edge_detection(const std::string img_file){

    std::cout << "Current image: " << img_file << std::endl;

    // Load Image
    cv::Mat image = cv::imread(img_file);

    if (image.empty()) {
        std::cout << "This image does not exist (or is on the wrong file path): " << img_file << std::endl;
        return -1;
    }

    cv::Mat img_gray, final;

    // Gray Scale Conversion
    cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY);

    // Edge Detection

    // cv::Canny(img_gray, final, 100, 200);
    cv::GaussianBlur(img_gray, final, cv::Size(11,11), 5, 0);
    
    // DISPLAY CODE
    
    cv::namedWindow("orig window", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("gray window", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("gaussian window", cv::WINDOW_KEEPRATIO);

    imshow("orig window", image);
    cv::resizeWindow("orig window", 300,300);
    imshow("gray window", img_gray);
    cv::resizeWindow("gray window", 300,300);
    imshow("gaussian window", final);
    cv::resizeWindow("gaussian window", 300,300);

    cv::waitKey(0);
    return 0;
}

int main(int argc, char *argv[]) {

    options opt;
    const char *short_opts;
    const option *long_opts;

    while (true) {

        const int c = getopt_long(argc, argv, short_opt, long_opts, nullptr);

        if (c == -1) {break;}

        switch(c) {
            case 'h':
                help_func(argv[0]);
                return EXIT_SUCCESS;
            case 'm': 
                opt.method = parse_method(optarg);
                if (!opt.method) {
                    std::cerr << "Invalid method " << optarg << '\n';
                    return EXIT_FAILURE;
                }
                break;
            case 'i':
                opt.input = parse_input(optarg);
                break;
            case 'o':
                opt.output = optarg;
                break;
            default:
                help_func(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char method;

    if (opt.method) {
        method = *opt.method;
    }

    switch (method) {
        case 'e':
            for (auto i: *opt.input) {
                load_edge_detection(i);
            }
            break;
        case 'y':
            break;
        case 'z':
            break;

    }
}





