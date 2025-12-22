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

// user-defined scoped enum, wr = 0, ap = 1
// write_mode mode = write_mode::wr;
enum class write_mode {
    wr,
    ap
};

std::optional<char> parse_method(std::string_view s) {
    std::cout << "method work" << std::endl;
    if (s == "x") return 'x';
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
    std::cout << "\t-m or --method: Method" << '\n';
    std::cout << "\t\t Method 1: Desc" << '\n';
    std::cout << "\t-i or --input: Input File" << '\n';
    std::cout << "\t-o or --output: Output File" << '\n';
}

int load_preprocess(const std::string img_file){
    cv::Mat image = cv::imread(img_file);
    cv::Mat gray;
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5,5), 0);
    return 0;
}

int main(int argc, char *argv[]) {

    options opt;
    const char *short_opts;
    const option *long_opts;

    while (true) {

        const int c = getopt_long(argc, argv, short_opt, long_opts, nullptr);

        if (c == -1) {help_func(" "); return EXIT_FAILURE;}

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
        std::cout << "Testing" << std::endl;
        method = *opt.method;
    }

    switch (method) {
        case 'x':
            load_preprocess((*opt.input).front());
            break;
        case 'y':
            break;
        case 'z':
            break;

    }
}





