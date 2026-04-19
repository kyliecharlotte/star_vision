# Star Vision - in development
## Modern C++ Exploration of Computer-aided Design (CAD) Techniques 

### Current Application Implementation:
1. SIFT Algorithm - Scale-Invariant Feature Transform (s)
   - Detects features that are consistent across transformations in scale and rotation (image reconstruction, object recognition)
  
2. Canny Edge Detection (e)
   - Identify edges in images (lane detection, object recognition)

#### On Branch gui_work:

To run once files are downloaded and dependencies are installed:

1. ./main --> opens GUI

#### On Branch Main:

To run once files are downloaded and dependencies are installed:

1. ./main --> enters command prompt format where user will be prompted for information

2. Run using flags:
   
   Options:
     -h (--help): Help
   
     -m (--method): Method
   
       s: SIFT Algorithm
   
       e: Edge Detection
   
     -i (--input): Input File
   
       ex: "input_1.jpeg", "input_2.png"
   
     -o (--output): Output Files
   
       ex: "out_1.jpeg, out_2.png"
       ex: "" (for no output saving)

ex: ./main -m e -i image.png -o output.png

Next Steps:

- Add more CAD algorithms + customization options
- Add a display option
- Move code around out of main.cpp
