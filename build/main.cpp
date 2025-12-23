#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>
#include <getopt.h>
#include <optional>

// short flag options -- -h (help), -m, -i, -o 
const char* const short_opt = ":hm:i:o:";
const struct option long_opt[] = {
    {"help", 0, NULL, 'h'},
    {"method", 1, NULL, 'm'},
    {"input", 1, NULL, 'i'},
    {"output", 1, NULL, 'o'},
    {NULL, 0, NULL, 0}
};

// helper function

std::string trim_string(std::string s) {

    const char* ignore_chars = " \n\t\'\"";

    const auto start = s.find_first_not_of(ignore_chars);
    const auto end = s.find_last_not_of(ignore_chars);

    if (start == std::string::npos) {
        return "";
    }

    return s.substr(start,end-start+1);

    return s;
}

std::string read_input_line (std::string prompt) {
    std::cout << prompt;
    std::string input_line;
    std::getline(std::cin, input_line);
    return input_line;
}

struct options {
    std::optional<std::vector<std::string>> input;
    std::optional<std::vector<std::string>> output;
    std::optional<char> method;
};

std::optional<char> parse_method(std::string_view s) {
    if (s == "e") return 'e';
    if (s == "s") return 's';
    return std::nullopt;
}

std::optional<char> prompt_method() {

    std::cout << "Select method to be applied: \n" << std::endl;
    std::cout << "  e: Canny edge detection\n" << std::endl;
    std::cout << "  s: SIFT\n" << std::endl;
    std::cout << "Enter method: " << std::endl;
    std::cout << "> ";

    std::string method_input;
    std::getline(std::cin, method_input);

    if (method_input == "e" || method_input == "s") {
        return method_input[0];
    }
    return std::nullopt;
}

std::optional<std::vector<std::string>> parse_input_output(const std::string& s) {

    const char *delim = ",";
    std::vector<std::string> input_list;

    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, *delim)) {
        token = trim_string(token);
        input_list.push_back(token);
    }

    if (input_list.size() > 0) {
        return input_list;
    } else {
        return std::nullopt;
    }
}

std::optional<std::vector<std::string>> prompt_input_output() {

    std::cout << "\nList image files to be edited: \n" << std::endl;
    std::cout << "  Separate them by comma within quotes\n" << std::endl;
    std::cout << "  ex: \"file_1.jpg, file_2.png\"" << std::endl;
    std::cout << "> ";
    std::string input_line;
    std::getline(std::cin, input_line);
    return parse_input_output(input_line.data());

}

void help_func(char *call) {
    //edit
    std::cout << "Usage: " << call << '\n';
    std::cout << "Options:" << '\n';
    std::cout << "\t-h or --help: Display Help information" << '\n';
    std::cout << "\t-m or --method: Method to be Applied to Image" << '\n';
    std::cout << "\t\t Method 1: Edge Detection" << '\n';
    std::cout << "\t\t\t Smooths the image to reduce noise, useful for object detection" << '\n';
    std::cout << "\t-i or --input: Input File" << '\n';
    std::cout << "\t-o or --output: Output File" << '\n';
}

bool window_watcher(const std::vector<std::string>& windows) {
    for (const auto& w : windows) {
        if (cv::getWindowProperty(w, cv::WND_PROP_VISIBLE) >=1 ) { return true; }
    }
    return false;
}

int load_edge_detection(const std::string img_file, const std::string out_file){

    std::cout << "Current image: " << img_file << std::endl;
    std::cout << "Current method: Canny" << std::endl;

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
    cv::Canny(img_gray, final, 100, 200);

    // DISPLAY CODE
    
    cv::namedWindow("orig window", cv::WINDOW_KEEPRATIO);
    //cv::namedWindow("gray window", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("edge detected window", cv::WINDOW_KEEPRATIO);

    imshow("orig window", image);
    cv::resizeWindow("orig window", 300,300);
    //imshow("gray window", img_gray);
    //cv::resizeWindow("gray window", 300,300);
    imshow("edge detected window", final);
    cv::resizeWindow("edge detected window", 400,400);

    while (window_watcher({"orig window", "edge detected window"})) {
        cv::waitKey(100);
    }
    
    return 0;
}

int load_sift(const std::string img_file, const std::string out_file){

    std::cout << "Current image: " << img_file << std::endl;
    std::cout << "Current method: SIFT" << std::endl;

    // Load Image
    cv::Mat image = cv::imread(img_file);

    if (image.empty()) {
        std::cout << "This image does not exist (or is on the wrong file path): " << img_file << std::endl;
        return -1;
    }

    cv::Mat img_gray, final;

    // Gray Scale Conversion
    cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY);

    // sift Detection

    const auto sift = cv::SIFT::create();
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    sift->detectAndCompute(img_gray, cv::noArray(), keypoints, descriptors);
    
    // DISPLAY CODE
    
    cv::namedWindow("orig window", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("sift window", cv::WINDOW_KEEPRATIO);

    imshow("orig window", image);
    cv::resizeWindow("orig window", 300,300);

    cv::Mat display_image;
    cv::drawKeypoints(img_gray, keypoints, display_image);
    imshow("sift window", display_image);
    cv::resizeWindow("sift window", 300,300);

    int key;

    while (window_watcher({"orig window", "sift window"})) {
        key = cv::waitKey(100);
        if (key == 27) {
            cv::destroyWindow("orig window");
            cv::destroyWindow("sift window");
        }
    }

    return 0;
}

options prompt_user() {
    options opt;

    while (!opt.method) {
        opt.method=prompt_method();
        if (!opt.method) {
            std::cout << "Invalid method\n";
        }
    }
    while (!opt.input) {
        std::cout << "Input File Paths" << std::endl;
        opt.input=prompt_input_output();
        if (!opt.input) {
            std::cout << "Invalid input format\n";
        }
    }

    while (!opt.output) {
        std::cout << "Output File Paths" << std::endl;
        opt.output=prompt_input_output();
        if (!opt.output) {
            std::cout << "Invalid output format\n";
        }
    }

    if (opt.input->size() != opt.output->size()) {
        throw std::runtime_error(
            "Input and output must have the same number of files\n"
        );
    }
    return opt;

}

options parse_command_line(int argc, char* argv[]) {
    options opt; 
    while (true) {

        const int c = getopt_long(argc, argv, short_opt, long_opt, nullptr);
        
        if (c==-1) {break;}

        switch(c) {
            case 'h':
                help_func(argv[0]);
                throw std::runtime_error(
                    "\n"
                );
            case 'm':
                opt.method=parse_method(optarg);
                if (!opt.method) {
                    throw std::runtime_error(
                        "Incorrect method\n"
                    );
                }
                break;
            case 'i':
                opt.input=parse_input_output(optarg);
                if (!opt.input) {
                    throw std::runtime_error(
                        "Issue with input files\n"
                    );
                }
                break;
            case 'o':
                opt.output = parse_input_output(optarg);
                if (!opt.output) {
                    throw std::runtime_error(
                        "Issue with output files\n"
                    );
                }
                break;
            default:
                help_func(argv[0]);
                throw std::runtime_error(
                    "\n"
                );
        }
    }

    if (opt.input->size() != opt.output->size()) {
        throw std::runtime_error(
            "Input and output must have the same number of files\n"
        );
    }

    return opt;

}


int main(int argc, char* argv[]) {

    try {

        options opt;

        if (argc > 1) {
            opt=parse_command_line(argc, argv);
        } else {
            opt=prompt_user();
        }

        const auto& input = *opt.input;
        const auto& output = *opt.output;

        switch (*opt.method) {
            case 'e':
                for (size_t i = 0; i < input.size(); ++i) {
                    load_edge_detection(input[i], output[i]);
                }
                break;
            case 's':
                for (size_t i = 0; i < input.size(); ++i) {
                    load_sift(input[i], output[i]);
                }
                break;
        }

    }

    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
/*
int main(int argc, char *argv[]) {

    options opt;

    while (true) {

        const int c = getopt_long(argc, argv, short_opt, long_opt, nullptr);

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
                opt.input = parse_input_output(optarg);
                if (!opt.input) {
                    std::cerr << "Invalid input " << optarg << '\n';
                    return EXIT_FAILURE;
                }
                break;
            case 'o':
                opt.output = parse_input_output(optarg);
                if (!opt.input) {
                    std::cerr << "Invalid input " << optarg << '\n';
                    return EXIT_FAILURE;
                }
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

    if (opt.input->size() != opt.output->size()) {
        std::cerr << "Number of input files and output files needs to match." << '\n';
        return EXIT_FAILURE;
    }

    const auto& inputs = *opt.input;
    const auto& output = *opt.output;

    switch (method) {
        case 'e':
            for (std::size_t i = 0; i < opt.input->size(); ++i) {
                load_edge_detection(inputs[i], output[i]);
            }
            break;
        case 's':
            for (std::size_t i = 0; i < opt.input->size(); ++i) {
                load_sift(inputs[i], output[i]);
            }
            break;
        case 'z':
            break;

    }

}
*/





