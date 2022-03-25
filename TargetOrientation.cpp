/************************************************************************************************
 * @file		TargetOrientation.cpp
 *
 * @brief	Implements the TargetOrientation Class
 **************************************************************************************************/

#include "TargetOrientation.h"


 /**********************************************************************************************//**
  * @fn	double getMedian(Mat image)
  *
  * @brief	Calculates the value of the median pixel.
  *
  * @param 	image  	The input image.
  * 
  * @returns	The value of the median pixel.
  **************************************************************************************************/
double getMedian(cv::Mat image)
{
    double m = (image.rows * image.cols) / 2;
    int bin = 0;
    double med = -1;
    int histSize = 256;
    float range[] = { 0, 256};
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;

    cv::Mat hist;
    cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    
    for (float i = 0; i < histSize && med < 0; i++)
    {
        bin = bin + cvRound(hist.at<float>(i));
                
        if (bin > m && med < 0)
            med = i;
    }

    return med;
}
/**********************************************************************************************//**
  * @fn	int max_thetha(cv::Mat& input)
  *
  * @brief	Calculates the optimum angle for rotation from the calculation of the row with maximum pixel when rotated.
  *
  * @param 	image  	The input image.
  *
  * @returns	The optimum angle of rotation.
  **************************************************************************************************/

int max_thetha(cv::Mat& input) {
    cv::Mat row_sum;

    int iBorderMode = 0;
    
    int iImageCenterY = input.rows/2;
    int iImageCenterX = input.cols/2;

    std::vector<std::pair< double , int>> angle_maxValue;

    for (int theta = 0; theta <= 180; theta+=5) {

        cv::Mat matRotation = cv::getRotationMatrix2D(cv::Point(iImageCenterX, iImageCenterY), theta, 1);

        // Rotate the image
        cv::Mat imgRotated;
        warpAffine(input, imgRotated, matRotation, input.size(), cv::INTER_LINEAR, iBorderMode, cv::Scalar());
        
        reduce(imgRotated, row_sum, 1, cv::REDUCE_SUM, CV_32F);

        double min, max;
        cv::minMaxLoc(row_sum, &min, &max);

        angle_maxValue.push_back(std::make_pair(max , theta));
        
    }
    std::sort(angle_maxValue.begin(), angle_maxValue.end());


    return angle_maxValue.back().second;
}
/**********************************************************************************************//**
  * @fn	int getXPercentile(cv::Mat& input , double percentile)
  *
  * @brief	Calculates the 90th Percentile of the image pixel values when sorted.
  *
  * @param 	image  	The input image.
  * @param  percentile  The percentile to use to get threshold value
  *
  * @returns	The optimum angle of rotation.
  **************************************************************************************************/

int getXPercentile(cv::Mat& input , double percentile) {
    std::vector<uchar> vec;
    if (input.isContinuous()) {
        vec.assign((uchar*)input.datastart, (uchar*)input.dataend);
    }
    else {
        for (int i = 0; i < input.rows; ++i) {
            vec.insert(vec.end(), input.ptr<uchar>(i), input.ptr<uchar>(i) + input.cols);
        }
    }
    std::sort(vec.begin(), vec.end());

    int the90th = vec.size() * percentile;

    return vec[the90th];
}

/**********************************************************************************************//**
  * @fn	void SobelEdgeDetection(cv::Mat input , cv::Mat &output) 
  *
  * @brief	Applies Sobel edge Detection to the input image
  *
  * @param 	image  	The input image. 
  * @param 	output 	The resulting output image.

  **************************************************************************************************/

void SobelEdgeDetection(cv::Mat input , cv::Mat &output) {
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::Sobel(input, grad_x, CV_32F, 1, 0);

    cv::Sobel(input, grad_y, CV_32F, 0 , 1);

    // converting back to CV_8U
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
}
/**********************************************************************************************//**
  * @fn	void finalRotation(cv::Mat input , cv::Mat &output)
  *
  * @brief	Applies Sobel edge Detection to the input image
  *
  * @param 	image  	The input image.
  * @param 	output 	The resulting output image.

  **************************************************************************************************/

void finalRotation(cv::Mat input, cv::Mat& output , int angle) {
    int iBorderMode = 0;

    int iImageCenterY = input.rows / 2;
    int iImageCenterX = input.cols / 2;

    cv::Mat matRotation = cv::getRotationMatrix2D(cv::Point(iImageCenterX, iImageCenterY), angle, 1);
    // Rotate the image
    cv::Mat imgRotated;
    warpAffine(input, output, matRotation, input.size(), cv::INTER_LINEAR, iBorderMode, cv::Scalar());
}



