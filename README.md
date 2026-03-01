# Star Vision
## Modern C++ Exploration of Computer-aided Design (CAD) Techniques 

### Current Status:
1. SIFT Algorithm - Scale-Invariant Feature Transform (s)
   - Detects features that are consistent across transformations in scale and rotation (image reconstruction, object recognition)
  
2. Canny Edge Detection (e)
   - Identify edges in images (lane detection, object recognition)
  
Currently included in the repository are two images downloaded to use as examples ("image.png", "star_img_1.jpeg")

To run once files are downloaded (or repo is cloned):

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
