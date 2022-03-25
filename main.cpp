#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "ezOptionParser.hpp"
#include "TargetOrientation.h"



using std::cout;
using std::endl;
using ez::ezOptionParser;


void showUsage(ezOptionParser& opt);
void initializeParser(ezOptionParser& opt);
bool isMissingParams(ezOptionParser& opt);
void showProgramCall(ezOptionParser& opt);

using namespace std;
using namespace cv;

int main(int argc, const char* argv[])
{
    ezOptionParser opt;
    initializeParser(opt);
    opt.parse(argc, argv);
    if (opt.isSet("-h") || isMissingParams(opt))
    {
        showUsage(opt);
        return 0;
    }
	
	showProgramCall(opt);

    std::string filename;
    opt.get("-i")->getString(filename);

    int band_index;
    opt.get("-b")->getInt(band_index);

    double percentile;
    if (opt.get("-p")->isSet) {
        opt.get("-p")->getDouble(percentile);
    }else {
        percentile = 0.9;
    }

	auto start = std::chrono::high_resolution_clock::now();
	cv::Mat input_image;


	try
    {
        cout << "Reading Input Image ...";
        input_image = imread(filename.c_str());
        cout << " DONE" << endl;
    }
    catch (const std::exception&)
    {
        cout << endl << "Error reading input file " << filename << endl;
        return 1;
    }


	cv::Mat output_normalize, output_gauss, gauss_wo_normal;

    //applying Gaussain blur
    cout << "Applying Gaussian blur"<< endl;
    cv::GaussianBlur(input_image, gauss_wo_normal, cv::Size(5, 5), 0);

    //finding the Xth percentile
	cout << "Finding the pixel value at " 
		<< ((percentile < 1) ? (int)(100 * percentile + 0.5) : (int)(percentile + 0.5))
		<< " Percentile" << endl;
	double image_XthPercentile = getXPercentile(gauss_wo_normal , percentile);

    double max_val = 1;

    //segmentation using cv::threshold using the 90th Percentile as thresh
    cv::Mat thresh;
    cout << "Applying Segmentation with threshold found from input percentile" << endl;
    cv::threshold(gauss_wo_normal, thresh, image_XthPercentile, max_val, cv::THRESH_BINARY);

    //edge detection using Sobel Edge Detection
    cv::Mat edge_det;
    cout << "Applying Sobel Edge Detection" << endl;
    SobelEdgeDetection(thresh, edge_det);

    //dilating the image
    cv::Mat frameDilate;
    cout << "Dilating the image" << endl;
    dilate(edge_det, frameDilate, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    
    int angle = max_thetha(frameDilate);

	cout << "DONE" << endl << endl;

	cout << "The Target Orientation angle is " << angle << " degrees" << endl << endl;

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;
	cout << "Program Execution Time = " << elapsed.count() << " ms" << endl << endl;


	cout << "Press ENTER key to exit" << endl;
	std::cin.ignore();

    return 0;
}

void showUsage(ezOptionParser& opt)
{
    std::string usage;
    opt.getUsage(usage);
    std::cout << usage;
}

/**********************************************************************************************//**
 * @fn	void initializeParser(ezOptionParser& opt)
 *
 * @brief	Initializes the ezOptionparser.CommandLine TargetOrientation.exe -h displays the Usage.
 *
 * @param [in,out]	opt	The ezOptionParser object.
 **************************************************************************************************/

void initializeParser(ezOptionParser& opt)
{
    opt.overview = "Test";
    opt.syntax = "Test.exe [OPTIONS]";
    opt.example = "Test.exe -i \"input.tif\" -o \"output.tif\" \n\n";

    opt.add("", 0, 0, 0, "Display usage instructions", "-h", "--help");
    opt.add("", 1, 1, 0, "Input image file name", "-i", "--input");
    opt.add("", 1, 1, 0, "Output file name", "-o", "--band_index");

}

/**********************************************************************************************//**
 * @fn	bool isMissingParams(ezOptionParser& opt)
 *
 * @brief	Query if 'opt' is missing parameters
 *
 * @param [in,out]	opt	The option.
 *
 * @returns	True if missing parameters, false if not.
 **************************************************************************************************/

bool isMissingParams(ezOptionParser& opt)
{
    std::vector<std::string> bad_options;
    bool has_reqd_args = opt.gotRequired(bad_options);
    return !has_reqd_args;
}

/**********************************************************************************************//**
 * @fn	void showProgramCall(ezOptionParser& opt)
 *
 * @brief	Shows the program call.
 *
 * @param [in,out]	opt	The ezOptionParser option.
 **************************************************************************************************/

void showProgramCall(ezOptionParser& opt)
{
    std::string filename;
    std::cout << "Starting Test Program... " << std::endl;
    std::cout << "\t" << "Test.exe";


    opt.get("-i")->getString(filename);
    std::cout << " -i " << filename;

    opt.get("-o")->getString(filename);
    std::cout << " -o " << filename;

    std::cout << std::endl << std::endl;
}
