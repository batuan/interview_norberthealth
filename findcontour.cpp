#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
RNG rng(12345);
Mat src;
int nbPoint = 100;
float w1 = 0.6442;
float w2 = 0.3558;
float bias = 6.4676;
int flag = 0;
struct bigger
{
    template<class T>
    bool operator()(T const &a, T const &b) const { return a > b; }
};

Mat img2Canny(Mat img){
    float mthreshold = mean(img)[0];
    Mat thresh;
    medianBlur(img, thresh, 5);
    threshold(thresh, thresh, mthreshold, 0, THRESH_TOZERO);
    normalize(thresh, thresh, 0, 255, NORM_MINMAX, CV_8U);
    Canny(thresh, thresh, 30, 60);
    return thresh;
}


float calTemperature(vector<Point> contour){
    Rect bound = boundingRect(contour);
    Mat lena1 = Mat::zeros(Size(src.cols, src.rows),CV_8UC1);
    rectangle(lena1, bound, 255, -1);
    vector<Point> indices;
    findNonZero(lena1, indices);

    vector<float> vectorTemp;
    float sum = 0;
    float maxTemp = -1;
    for (int i = 0; i<indices.size(); i++){
        __int16_t temp = src.at<__int16_t>(indices[i].x, indices[i].y);
        float celcius = float(temp) / 100 - 273.15; 
        sum += celcius;
        vectorTemp.push_back(celcius);
        if (celcius > maxTemp){
            maxTemp = celcius;
        }
    }

    sort(vectorTemp.begin(), vectorTemp.end(), bigger());
    float maxAverage = 0.0;
    float minAverage = 0.0;
    for (auto i = vectorTemp.begin(); i < vectorTemp.begin()+nbPoint; ++i){
        maxAverage += *i;
    }

    for (auto i = vectorTemp.end() - 100; i < vectorTemp.end(); i++)
    {
        minAverage += *i;
    }
    float maxBoundingboxTemp = maxAverage / nbPoint;
    float environmentTemp = minAverage / nbPoint;
    cout << "average temperature of human is " << maxBoundingboxTemp << endl;
    cout << "average temperature of environment is " << environmentTemp << endl;
    cout << "bouding box size: " << indices.size() << endl;
    float averageTemp = sum / indices.size();
    cout << "Average temp of contour "  << averageTemp << endl;
    cout << "Max temp " << maxTemp << endl;
    cout << "Min temp " << vectorTemp.back() << endl;

    if (flag==1){
        float predictTemp = w1 * maxBoundingboxTemp + w2 * environmentTemp + bias;
        cout << "predict temperature of this image is: " << predictTemp << endl;
    }

    cout << endl;
}

void drawMaxContour(Mat img, Mat dst){
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    int maxIndex = -1;
    float maxArea = -1.0;    
    findContours(img, contours, RETR_TREE, CHAIN_APPROX_NONE);
    for (int i=0; i<contours.size(); i++){
        vector<Point>  hull;
        convexHull(contours[i], hull);
        float area = contourArea(hull);
        if (area > maxArea) {
            maxIndex = i;
            maxArea = area;
        }
    }

    Scalar color = Scalar(0, 255, 0);
    drawContours(dst, contours, maxIndex, color, -1);
    calTemperature(contours[maxIndex]);
    Mat mresize;
    resize(dst, mresize, Size(640, 480), INTER_LINEAR);
    if (flag==1) {
        imshow("dst", mresize);
        waitKey(0);
    }
    // imshow("lena", lena1);
}


int main( int argc, char** argv )
{   
    const String keys = "{@input | ./IR/1595432616.161967.png | input image}"
                        "{image  | 1                          | showimage}";
 
    CommandLineParser parser( argc, argv,  keys);
    // int mode[10] = {IMREAD_COLOR, IMREAD_UNCHANGED, IMREAD_GRAYSCALE, IMREAD_ANYDEPTH, };
    flag = parser.get<int>("image");
    src = imread( samples::findFile( parser.get<String>( "@input" ) ) , IMREAD_ANYDEPTH);

    if( src.empty() )
    {
      cout << "Could not open or find the image!\n" << endl;
      cout << "Usage: " << argv[0] << " <Input image>" << endl;
      return -1;
    }

    Mat norm, drawing;
    normalize(src, norm, 0, 255, NORM_MINMAX, CV_8U); // for visualizing 
    // (drawing, CV_8UC3);
    cvtColor(norm, drawing, COLOR_GRAY2BGR);
    Mat thresh = img2Canny(src);
    drawMaxContour(thresh, drawing);
    // imshow("canny", norm);
    
    return 0;
}
