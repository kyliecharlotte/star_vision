#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <fstream>
#include <getopt.h>
#include <optional>

// Star Vision: Command-line interface and prompter for applying
// computer vision algorithms (Canny edge detection, SIFT)
// to multiple images using OpenCV.
//
// Entry: main()

//=====================================================

constexpr const char* short_opt = ":hm:i:o:";

/**
 * @brief Listed out COMMAND LINE flag values 
 * 
 */
const struct option long_opt[] = {
    {"help", 0, NULL, 'h'},
    {"method", 1, NULL, 'm'},
    {"input", 1, NULL, 'i'},
    {"output", 1, NULL, 'o'},
    {NULL, 0, NULL, 0}
};

/**
 * @brief Mode referencing whether user text is for input images or output path
 * 
 */
enum class Mode {
    in,
    out
};

/**
 * @brief State allowing users to exit COMMAND PROMPT by pressing q  
 * 
 */
enum class Exit{
    Ok,
    Quit
};

/**
 * @brief Available methods for users to select from to be applied to their photos
 * 
 */
enum class Method {
    Edge,
    Sift
};

/**
 * @brief Input that will be collected from the user (output is optional)
 * 
 */
struct options {
    std::optional<std::vector<std::string>> input;
    std::optional<std::vector<std::string>> output;
    std::optional<Method> method;
};

//=====================================================

/**
 * @brief Text only function displayed to users in COMMAND LINE interface regarding flags and inputs
 * 
 */
void help_func() {
    std::cout << "Options:" << '\n';
    std::cout << "\t-h or --help: Display Help information" << '\n';
    std::cout << "\t-m or --method: Method to be Applied to Image" << '\n';
    std::cout << "\t\t Method 1: Edge Detection (e)" << '\n';
    std::cout << "\t\t - Canny edge detection algorithm smooths the image to reduce noise, useful for object detection" << '\n';
    std::cout << "\t\t Method 2: SIFT (s)" << '\n';
    std::cout << "\t\t - Identifies and points out objects in images, useful for object recognition tasks" << '\n';
    std::cout << "\t-i or --input: Input File(s)" << '\n';
    std::cout << "\t\t Example: \"input_1.jpg, input_2.png\" " << '\n';
    std::cout << "\t-o or --output: Output File(s)" << '\n';
    std::cout << "\t\t Example: \"output_1.jpg, output_2.png\" " << '\n';
    std::cout << "\t\t If you do not want to save the results, just enter \"\"" << '\n';
}

/**
 * @brief Trims a string to remove any whitespace or extra characters
 * 
 * @param s String to trim
 * @return std::string
 */
std::string trim_string(std::string s) {

    const char* ignore_chars = " \n\t\'\"";

    const auto start = s.find_first_not_of(ignore_chars);
    const auto end = s.find_last_not_of(ignore_chars);

    if (start == std::string::npos) {
        return "";
    }

    return s.substr(start,end-start+1);
}

/**
 * @brief Take user-entered string representing their chosen method and convert into char 
 * 
 * @param s String from user representing method
 * @return std::optional<char> Null if not a pre-defined method
 */
std::optional<Method> parse_method(std::string_view s) {
    if (s == "e" || s == "E") return Method::Edge;
    if (s == "s" || s == "S") return Method::Sift;
    return std::nullopt;
}

/**
 * @brief Method input instructional menu shown to user in COMMAND PROMPT
 * 
 * @return std::optional<char> 
 */
std::optional<Method> prompt_method(Exit& curr_state) {
    curr_state = Exit::Ok; // Reset current state for each prompt_method call

    std::cout << "*** Welcome to Star Vision! ***\n" << std::endl; 
    std::cout << "Type the method you want to apply to your images: \n" << std::endl;
    std::cout << "  e: Canny edge detection\n" << std::endl;
    std::cout << "      - Canny edge detection: Smooths the image to reduce noise, useful for object detection\n" << std::endl;
    std::cout << "  s: SIFT (Scale-Invariant Feature Transform)\n" << std::endl;
    std::cout << "      - Identifies and points out objects in images, useful for object recognition tasks\n" << std::endl;
    std::cout << "  q: Quit the program\n" << std::endl;
    std::cout << "Enter method: " << std::endl;
    std::cout << "> ";

    std::string method_input;
    std::getline(std::cin, method_input);

    if (method_input == "e" || method_input == "E" || method_input == "S" || method_input == "s") {
        return parse_method(method_input);
    }
    if (method_input == "q" || method_input == "Q") {
        curr_state = Exit::Quit;
        return std::nullopt;
    }
    return std::nullopt;
}

/**
 * @brief Take in a string containing file names, separated by commas, and insert each file into a vector
 * 
 * @param s String containing user-entered text to be parsed
 * @param state State to indicate whether the entered text is for input or output files
 * @return std::optional<std::vector<std::string>> : Input list containing all of the entries (separated by comma)
 *         std::optional<std::vector<std::string>>{} : Empty if in Output Mode and no text is entered (display only)   
 */
std::optional<std::vector<std::string>> parse_input_output(const std::string& s, Mode state) {

    const char *delim = ",";
    std::vector<std::string> input_list;

    if (state == Mode::out && (s.empty())) {
        return std::vector<std::string>{}; // In case of empty list of output file paths
    }

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

/**
 * @brief Input / Output file input instructional menu shown to user in COMMAND PROMPT
 * 
 * @param state Class indicating whether the files are being entered as input or output, passed through to @parse_input_output
 * @return std::optional<std::vector<std::string>> 
 */
std::optional<std::vector<std::string>> prompt_input_output(Exit& curr_status, Mode state) {

    std::cout << "\nList image files: \n" << std::endl;
    std::cout << "  Please list the image files within quotes and separate them by commas.\n" << std::endl;
    std::cout << "  example one: \"file_1.jpg\"" << std::endl;
    std::cout << "  example one: \"file_1.jpg, file_2.png\"" << std::endl;
    std::cout << "> ";

    std::string input_line;
    std::getline(std::cin, input_line);

    if (input_line == "q") {
        curr_status = Exit::Quit;
    }

    return parse_input_output(input_line.data(), state);
}

/**
 * @brief Monitors OpenCV windows and returns false once all are closed
 * 
 * @param windows Names of windows declared in cv::NamedWindow
 * @return true If any of the possible windows are open
 * @return false If no window is open
 */
bool window_watcher(const std::vector<std::string>& windows) {

    for (const auto& w : windows) {
        if (cv::getWindowProperty(w, cv::WND_PROP_VISIBLE) >=1 ) { return true; } // >=1 indicates fully open window
    }
    return false;
}

/**
 * @brief Run the Canny Edge Detection algorithm on an image and optionally save the results as a new image
 * 
 * @param img_file Input file path representing image that the method is to be applied to 
 * @param out_file Output file path representing where the image will be saved (empty if display only)
 * @return int 
 */
int method_edge_detection(const std::string& img_file, const std::string& out_file){

    std::cout << "\nCurrent image: " << img_file << std::endl;
    std::cout << "Current method: Canny" << std::endl;
    std::cout << "You can close the display windows using the exit button or esc/return keys\n" << std::endl;

    cv::Mat image = cv::imread(img_file); // Try to load image

    if (image.empty()) {
        std::cout << "This image does not exist (or is on the wrong file path): " << img_file << std::endl;
        return -1;
    }

    cv::Mat img_gray, final;

    cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY); // Apply gray scale conversion for canny algorithm

    cv::Canny(img_gray, final, 100, 200);

    // DISPLAY CODE

    cv::namedWindow("orig window", cv::WINDOW_KEEPRATIO);
    cv::namedWindow("edge detected window", cv::WINDOW_KEEPRATIO);

    imshow("orig window", image);
    cv::resizeWindow("orig window", 300,300);

    imshow("edge detected window", final);
    cv::resizeWindow("edge detected window", 500,500);

    int key;

    while (window_watcher({"orig window", "edge detected window"})) {
        key = cv::waitKey(50);
        if (key == 27 || key == 13) { // return/esc keys to close windows
            cv::destroyWindow("orig window");
            cv::destroyWindow("edge detected window");
            break;
        }
    }

    if (!out_file.empty()) { // Save to output file if one exists
        try {
            cv::imwrite(out_file, final);
        }
        catch(const std::exception& e){
            std::cerr << "Error: " << e.what() << "\n";
            return -1;
        }
    }
    
    return 0;
}

/**
 * @brief Run the SIFT algorithm on an image and optionally save the results as a new image
 * 
 * @param img_file Input file path representing image that the method is to be applied to 
 * @param out_file Output file path representing where the image will be saved (empty if display only)
 * @return int 
 */
int method_sift(const std::string& img_file, const std::string& out_file){

    std::cout << "\nCurrent image: " << img_file << std::endl;
    std::cout << "Current method: SIFT" << std::endl;
    std::cout << "You can close the display windows using the exit button or esc/return keys\n" << std::endl;

    // Load Image
    cv::Mat image = cv::imread(img_file);

    if (image.empty()) {
        std::cout << "This image does not exist (or is on the wrong file path): " << img_file << std::endl;
        return -1;
    }

    cv::Mat img_gray, final;

    cv::cvtColor(image, img_gray, cv::COLOR_BGR2GRAY); // gray scale conversion to prepare for sift

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
    cv::resizeWindow("sift window", 500,500);

    int key;

    while (window_watcher({"orig window", "sift window"})) {
        key = cv::waitKey(50);
        if (key == 27 || key == 13) {
            cv::destroyWindow("orig window");
            cv::destroyWindow("sift window");
            break;
        }
    }

    if (!out_file.empty()) {
        try {
            cv::imwrite(out_file, display_image);
        }
        catch(const std::exception& e){
            std::cerr << "Error: " << e.what() << "\n";
            return -1;
        }
    }

    return 0;
}

/**
 * @brief Main program for running COMMAND PROMPT interface
 * 
 * @return options Returns instance of options class containing method, input file path(s), and optional output file paths
 */
options prompt_user() {

    options opt;
    Exit exit_state = Exit::Ok;
    Mode mode;

    while (!opt.method) {

        opt.method=prompt_method(exit_state);

        if (exit_state == Exit::Quit) {
            throw std::runtime_error(
                "User quit program\n"
            );
            break;
            
        }

        if (!opt.method) {
            std::cout << "Invalid method\n";
        }

    }
    while (!opt.input) {

        if (exit_state == Exit::Quit) { // skip user entry if they are trying to quit
            break;
        }

        mode = Mode::in;
        std::cout << "\n---Input Files---" << std::endl;
        opt.input=prompt_input_output(exit_state, mode);

        if (exit_state == Exit::Quit) {
            throw std::runtime_error(
                "User quit program\n"
            );
            break;
        }

        if (!opt.input) {
            std::cout << "Invalid input format\n";
        }
    }

    while (!opt.output) {

        if (exit_state == Exit::Quit) { // skip user entry if they are trying to quit
            break;
        }

        mode = Mode::out;
        std::cout << "\n---Output Files---" << std::endl;
        std::cout << "TIP: You can leave this empty if you do not want to save the images (blank or \"\")" << std::endl;
        opt.output=prompt_input_output(exit_state, mode);

        if (exit_state == Exit::Quit) {
            throw std::runtime_error(
                "User quit program\n"
            );
            break;
        }

        if (!opt.output) {
            std::cout << "Invalid output format\n";
        }
    }

    return opt;

}

/**
 * @brief Main program for running COMMAND LINE interface
 * 
 * @throw runtime_error Throws errors for issues with user-entered text format
 * @param argc Int representing the number of args
 * @param argv Array containing user input with flags and arguments
 * @return options Returns instance of options class containing method, input file path(s), and optional output file paths
 */
options parse_command_line(int argc, char* argv[]) {

    options opt; 
    Mode mode;
    bool help = false;
    while (true) {

        const int c = getopt_long(argc, argv, short_opt, long_opt, nullptr);
        
        if (c==-1) {break;}

        switch(c) {

            case 'h':
                help_func();
                help=true;
                break;

            case 'm':
                opt.method=parse_method(optarg);
                if (!opt.method) {
                    throw std::runtime_error(
                        "Incorrect method\n"
                    );
                }
                break;

            case 'i':
                mode = Mode::in;
                opt.input=parse_input_output(optarg, mode);
                if (!opt.input) {
                    throw std::runtime_error(
                        "Issue with input files\n"
                    );
                }
                break;

            case 'o':
                mode = Mode::out;
                opt.output = parse_input_output(optarg, mode);
                if (!opt.output) {
                    throw std::runtime_error(
                        "Issue with output files\n"
                    );
                }
                break;

            default:
                help_func();
                break;
        }
    }

    if (!opt.method || !opt.input || !opt.output) {
        if (help != true) {
            help_func();
            throw std::runtime_error(
                "Missing arguments"
            );
        }
    }

    return opt;
}

/**
 * @brief Main function responsible for assigning COMMAND LINE/INTERFACE, applying methods to inputs, and optionally storing in output
 * 
 * @exception Catches thrown exceptions and displays them for users
 * @param argc Int representing the number of args
 * @param argv Array containing user input with flags and arguments
 * @return int 
 */
int main(int argc, char* argv[]) {

    try {

        options opt;

        if (argc > 1) { // if there are any arguments, assume user is entering command line interface
            opt=parse_command_line(argc, argv);
        } else {
            opt=prompt_user();
        }

        const auto& input = *opt.input;
        std::vector<std::string> output;

        if (opt.output) { // change output only if it is non-null
            output = *opt.output;
        }

        switch (*opt.method) {

            case Method::Edge:
                for (size_t i = 0; i < input.size(); ++i) {
                    method_edge_detection(input[i], i < output.size() ? output[i] : ""); // send empty argument if no output file
                }
                break;

            case Method::Sift:
                for (size_t i = 0; i < input.size(); ++i) {
                    method_sift(input[i], i < output.size() ? output[i] : ""); // send empty argument if no output file
                }
                break;
        }
    }

    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n"; // catch any thrown exceptions and exit before running
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}





