/************************************************************************************//*
 * @file TargetOrientation.h.
 *
 * Declares the target orientation functions
 ************************************************************************************/
#pragma once
#ifndef TARGETORIENTATION_H
#define TARGETORIENTATION_H

#include <iostream>
#include <opencv2/opencv.hpp>

 /**********************************************************************************************//**
  * @fn	double getMedian(Mat image)
  *
  * @brief	Calculates the value of the median pixel.
  *
  * @param 	image  	The input image.
  *
  * @returns	The value of the median pixel.
  **************************************************************************************************/
double getMedian(cv::Mat image);

/**********************************************************************************************//**
  * @fn	int max_thetha(cv::Mat& input)
  *
  * @brief	Calculates the optimum angle for rotation from the calculation of the row with th maximum pixel when rotated.
  *
  * @param 	image  	The input image.
  *
  * @returns	The optimum angle of rotation.
  **************************************************************************************************/
int max_thetha(cv::Mat& input);

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
int getXPercentile(cv::Mat& input, double percentile);

/**********************************************************************************************//**
  * @fn	void SobelEdgeDetection(cv::Mat input , cv::Mat &output)
  *
  * @brief	Applies Sobel edge Detection to the input image
  *
  * @param 	image  	The input image.
  * @param 	output 	The resulting output image.

  **************************************************************************************************/
void SobelEdgeDetection(cv::Mat input, cv::Mat& output);

/**********************************************************************************************//**
  * @fn	void finalRotation(cv::Mat input , cv::Mat &output)
  *
  * @brief	Applies Sobel edge Detection to the input image
  *
  * @param 	image  	The input image.
  * @param 	output 	The resulting output image.

  **************************************************************************************************/
void finalRotation(cv::Mat input, cv::Mat& output, int angle);


#endif // !TARGETORIENTATION_H

