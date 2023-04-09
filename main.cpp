#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>
#include <Windows.h>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
/*
cv::Mat get_rotation_mat(int angle){
    //cv::Mat a(2,2, )
}
 */
using std::cout, std::endl, std::vector;
void add_circle(cv::Mat & m, double x,double y, int r, unsigned discr = 360){
    if(r > m.rows/2 || r > m.cols/2){
        cout << "WARNING radius is bigger then image\n";
    }

    x += r;
    const float PI = 3.14;
    for(unsigned i = 0; i < discr; ++i){
        double degrees = (i * PI) / 180.0;
        double x_m = x * cos(degrees) - y * sin(degrees) + m.rows/2;
        double y_m = x * sin(degrees) + y * cos(degrees) + m.cols/2;
        if (( x_m > 0 && x_m < m.rows) && ( y_m > 0 && y_m < m.cols))
            m.at<uchar>(x_m, y_m) = 255;
    }
}

void draw_circle(cv::Mat & m, double x,double y, int r, const vector<double>& trans_mat = {1, 0, 0, 1}, unsigned discr = 360){
    if(trans_mat.size() != 4){
        cout << "incorrect size of transformation matrix\n";
        return;
    }
    //assert(trans_mat.size() != 4 && "incorrect size of transformation matrix");
    x += r;
    const float PI = 3.14;
    for(unsigned i = 0; i < discr; ++i){
        double degrees = (i * PI) / 180.0;
        double x_m = x * cos(degrees) - y * sin(degrees);
        double y_m = x * sin(degrees) + y * cos(degrees);

        x_m = x_m * trans_mat[0] + y_m * trans_mat[1];
        y_m = x_m * trans_mat[2] + y_m * trans_mat[3];

        x_m += m.rows/2;
        y_m += m.cols/2;
        if (( x_m > 0 && x_m < m.rows) && ( y_m > 0 && y_m < m.cols))
            m.at<uchar>(x_m, y_m) = 255;
    }
}

int main() {
    int monitor_width = GetSystemMetrics(SM_CXSCREEN);
    int monitor_height = GetSystemMetrics(SM_CYSCREEN);

    const unsigned x_size =  monitor_height/2 - 0.1 * monitor_height,
                   y_size = monitor_width/2 - 0.1 * monitor_width;

    cv::namedWindow("track", 1);
    cv::resizeWindow("track", y_size, 250);
    cv::moveWindow("track", 0, x_size + 0.05 * x_size);

    cv::namedWindow("standart circle", 1);
    cv::resizeWindow("standart circle", x_size, y_size);
    cv::moveWindow("standart circle", 0, 0);

    cv::namedWindow("modified circle", 1);
    cv::resizeWindow("modified circle", x_size, y_size);
    cv::moveWindow("modified circle", y_size, 0);

    int radius = 50;
    cv::createTrackbar("Radius", "track", &radius, 200);

    //Create trackbar to change contrast
    vector<double> transformation_matrix(4);
    vector<int> tr_mati = {100, 0, 0, 100};

    cv::createTrackbar("[0][0] %", "track", &tr_mati[0], 1000);
    cv::createTrackbar("[0][1] %", "track", &tr_mati[1], 1000);
    cv::createTrackbar("[1][0] %", "track", &tr_mati[2], 1000);
    cv::createTrackbar("[1][1] %", "track", &tr_mati[3], 1000);

    cv::setTrackbarMin("[0][0] %", "track", -1000);
    cv::setTrackbarMin("[0][1] %", "track", -1000);
    cv::setTrackbarMin("[1][0] %", "track", -1000);
    cv::setTrackbarMin("[1][1] %", "track", -1000);

    while(true) {
        cv::Mat img_before = cv::Mat::zeros(x_size, y_size, CV_8UC1);
        cv::Mat img_after = cv::Mat::zeros(x_size, y_size, CV_8UC1);


        for(unsigned i = 0; i < 4; ++i){
            transformation_matrix[i] = static_cast<double>(tr_mati[i])/100;
        }

        draw_circle(img_before, 0, 0, radius);
        draw_circle(img_after, 0, 0, radius, transformation_matrix);

        //cout << iSliderValue1 << " " << iSliderValue2 << "\n";

        cv::imshow("standart circle", img_before);
        cv::imshow("modified circle", img_after);

    //cv::namedWindow("Trackbar Example");

    //int input_value = cv::getTrackbarPos("Number", "Number Input");
        if (cv::waitKey(10) == 27) {
            break;
        }
    }

}
