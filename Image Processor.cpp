
#include<iostream>
#include <opencv2\highgui.hpp>
#include<math.h>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>
using namespace std;
using namespace cv;

int cmd;
string imgPath = "beach.png";

Mat img = imread(imgPath, 1);
bool isHSV = false;
void showMenu() {
	cout << "=====================================" << '\n';
	cout << "=====================================" << '\n';
	cout << "              M E N U                " << '\n';
	cout << "=====================================" << '\n';
	cout << "=====================================" << '\n';
	cout << "1. Restore original iamge\n";
	cout << "2. Brightness\n";
	cout << "3. Gamma Correction\n";
	cout << "4. Edge Detection\n";
	cout << "5. Blur\n";
	cout << "6. Rotation\n";
	cout << "7. Rotate Color\n";
	cout << "8. comment\n";
	cout << "0. Exit\n";
	cout << "-------------------------------------" << '\n';
}
void showOriginalImg() {
	
	img = imread(imgPath, 1);
	imshow("image", img);
	isHSV = false;
}
string type2str(int type) { //command 실행 이후 img type 확인
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}


void bright() {
	cout << "correct value? ";
	double dBri;
	cin >> dBri;
	
	img.convertTo(img, -1, 1, dBri); //2번째 인자가 음수면 결과는 원본과 같은 타입
	//세번째 인자는 scale factor(optional)
	//네번째 인자는 밝기 변화
	imshow("image", img);
}

void gamma() {
	cout << "gamma? ";
	double gam;
	cin >> gam;
	
	CV_Assert(gam >= 0);
	if (gam < 0 || gam > 2.0) {
		cout << "gamma 범위 0이상 2이하\n";
		return;
	}
	Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
		p[i] = saturate_cast<uchar>(pow(i / 255.0, gam) * 255.0);

	//Mat res = img.clone();
	LUT(img, lookUpTable, img);
	
	imshow("image", img);
	
}

Mat grayImg;
Mat dst, detected_edges;

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int Ratio = 3;
const int kernel_size = 3;

void cannyEdge() {

	/// Reduce noise with a kernel 3x3
	cvtColor(img, img, COLOR_BGR2GRAY);
	//blur(grayImg, img, Size(3, 3));

	Canny(img, img, lowThreshold, max_lowThreshold, kernel_size);

	cvtColor(img, img,COLOR_GRAY2BGR);
	
	imshow("image", img);
}


void blurFunction() { //커널 사이즈로 짝수 넣으면 터짐
	int kernelSize;
	cout << "blur kernel size? ";
	cin >> kernelSize;
	if (kernelSize % 2 == 0) {
		cout << "******************Kernel size must be non-zero and odd number******************\n\n";
		return;
	}
	GaussianBlur(img, img, Size(kernelSize, kernelSize), 0);
	imshow("image", img);
	string ty = type2str(img.type());
	printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
}

void rotation() {
	int angle;
	cout << "Rotation angle? ";
	cin >> angle;
	Point2f centerPoint(img.cols/2., img.rows/2.);
	Mat temp = getRotationMatrix2D(centerPoint, angle, 1);
	warpAffine(img, img, temp, img.size());
	imshow("image", img);
}

//void rotateColor() {
//
//	cvtColor(img, img, COLOR_BGR2HSV);
//	double hShift;
//	cin >> hShift;
//
//	for (int i = 0; i < img.rows; i++) {
//		for (int j = 0; j < img.cols; j++) {
//			img.at<Vec3b>(i, j)[0] += hShift / 2;
//		}
//	}
//	cvtColor(img, img, COLOR_HSV2BGR);
//	imshow("image", img);
//}
void rotateColor() {
	cvtColor(img, img, COLOR_RGB2HSV);
	double hShift;
	cout << "Rotation in HUE? ";
	cin >> hShift;
	hShift = abs(hShift);
	
	for (int i = 0; i < img.cols; i++) {
		for (int j = 0; j < img.rows; j++) {
			int HUE = img.at<Vec3b>(Point(i, j))[0];
			HUE *= 2;
			HUE = HUE + hShift;
			HUE = HUE % 360;
			img.at<Vec3b>(Point(i, j))[0] = HUE / 2;

		}
	}
	cvtColor(img, img, COLOR_HSV2RGB); //다시 바꿔서 출력
	imshow("image", img);
}
void comment() {
	cout << "this is comment\n";
}

int main(void) {	
	showOriginalImg();
	
	while (1) {
		
		waitKey(1);
		
		showMenu();

		cout << "command? ";
		cin >> cmd;

		if (cmd == 1) { // 원본으로
		
			showOriginalImg();

			/*cout << img.channels() << '\n';
			cout << img.depth() << '\n';*/
			//string ty = type2str(img.type());
			//printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
		}
		else if (cmd == 2) { //밝기 증가
			bright();
			/*cout << img.channels() << '\n';
			cout << img.depth() << '\n';*/
			//string ty = type2str(img.type());
			//printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
		}
		else if (cmd == 3) {
			gamma();
			/*cout << img.channels() << '\n';
			cout << img.depth() << '\n';*/
			//string ty = type2str(img.type());
			//printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
		}
		else if (cmd == 4) { //edge detection. type casting 하는 방법 찾아야함
		
			cvtColor(img, grayImg, COLOR_BGR2GRAY);
			cannyEdge();
			//cout << img.channels() << '\n'; //1, 2, 3이랑 type 이 다르게나옴
			//cout << img.depth() << '\n';
			//string ty = type2str(img.type());
			//printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
		}
		else if (cmd == 5) { //blur 어떤거 써도 상관없는데, kernel size 증가하면 blur가 더 많이 되어야 한다
			blurFunction();
			//string ty = type2str(img.type());
			//printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
		}
		else if (cmd == 6) {
			rotation();
		}
		else if(cmd == 7){
			rotateColor();
			//string ty = type2str(img.type());
			//printf("Matrix: %s %dx%d \n", ty.c_str(), img.cols, img.rows);
		}
		else if (cmd == 8) {
			comment();
		}
		else if (cmd == 0) {
			break;
		}
		/*else if (cmd == 9) {
			rotate_color();
		}*/
		
	}
	return 0;
}
