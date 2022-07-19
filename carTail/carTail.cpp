#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>  //文件流库函数
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace cv;

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

int main22()
{

	//Mat image = imread("D:\\image\\res_1135\\test_00575.jpg");
	Mat image = imread("D:\\CPP\\carTail\\carTail\\00575.jpg");
	
	//Mat perMatrix(3, 3, CV_32F);
	//float perspective[3][3] = { { 4.1363954544067383e+00, -3.8299959897994995e-01, -1.0489592285156250e+03 }, { -4.7195121645927429e-01, 4.9817070960998535e+00, -8.0866217041015625e+02 }, { 2.0808174667763524e-05, -1.6460096230730414e-03, 1.0 } };
	//
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		perMatrix.at<float>(i, j) = perspective[i][j];
	//	}
	//}
	//Mat image;
	//resize(image1, image1, Size(570, 439));
	//warpPerspective(image1, image, perMatrix, image1.size());
	//imwrite("perspective.jpg", image);
	int width = image.cols;
	int height = image.rows;
	//对应的二维点和三维点对
	//float threeDim[4][3] = { { 3.52318, 1.60532, 2.382 }, { 3.64162, 3.09532, 2.48514 }, { 3.75237, 3.01082, 2.04784 }, { 3.74107, 1.02553, 2.0045 } };
	////float threeDim[4][3] = { { 44.978, 1848.13, 613.271 }, { -275.636, 1684.22, 572.227 }, { 144.249, 1675.99, 400.711 }, { -137.092, 1723.91, 372.779 } };
	//float threeDim[4][3] = { { -3.67174, 2.47186, 2.28475 }, { -3.65522, 0.851537, 2.47772 }, { -3.56673, 0.809385, 1.30896 }, { -3.75093, 2.99978, 1.35803 } };
	//float twoDim[4][2] = { { 219.08453, 244.02857 }, { 288.20618, 240.12857 }, { 281.74845, 325.92857 }, { 223.62886, 314.22857 } };
	//float twoDim[4][2] = { { 10, 6 }, { 483, 7 }, { 481, 326 }, { 12, 401 } };
	////float twoDim[4][2] = { { 2.39175, 3.342857 }, { 115.52164, 3.9 }, { 115.043298, 181.62857 }, { 2.8701, 223.41428 } };
	vector<Point3f>outDim;
	vector<Point2f>inDim;
	vector<float> distCoeff;
	distCoeff.push_back(-0.302859416524140);
	distCoeff.push_back(-1.271153636954774);
	distCoeff.push_back(1.437499343005666e-04);
	distCoeff.push_back(0.001155793066997);
	distCoeff.push_back(9.961442496187518);

	//for (int i = 0; i < 4; i++)
	//{
	//	outDim.push_back(Point3f(threeDim[i][0], threeDim[i][1], threeDim[i][2]));
	//	inDim.push_back(Point2f(twoDim[i][0], twoDim[i][1]));
	//}

	Mat cameraMatrix(3, 3, CV_32F);

	//float tempMatrix[3][3] = { { 3522.3, 0, 0 }, { 0, 3538.4, 0 }, { 1968.9,1375.4,1.0 } };
	//float tempMatrix[3][3] = { { 1.457167143613874e+03, 1.958581558298678, 9.610202837075110e+02 }, { 0, 1.457401615094959e+03, 5.454445259345974e+02 }, { 0, 0, 1.0 } };//相机的内参矩阵
	//float tempMatrix[3][3] = { { 2.198577757644186e+03, 11.191338025179473, 9.764266941199196e+02 }, { 0, 2.207348158784651e+03, 5.715773841932228e+02 }, { 0, 0, 1.0 } };//相机的内参矩阵
	float tempMatrix[3][3] = { { 2.198577757644186e+03, 11.191338025179473, 9.764266941199196e+02 }, { 0, 2.207348158784651e+03, 5.715773841932228e+02 }, { 0, 0, 1.0 } };//相机的内参矩阵
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cameraMatrix.at<float>(i, j) = tempMatrix[i][j];
		}
	}

	Mat cameraMatrixInvertTmp(3, 3, CV_32F);
	invert(cameraMatrix, cameraMatrixInvertTmp);
	Mat cameraMatrixInvert(4, 3, CV_32F);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cameraMatrixInvert.at<float>(i, j) = cameraMatrixInvertTmp.at<float>(i,j);
		}
	}
	cameraMatrixInvert.at<float>(3, 0) = 0;
	cameraMatrixInvert.at<float>(3, 1) = 0;
	cameraMatrixInvert.at<float>(3, 2) = 0;
	//Mat rvec1(1, 3, CV_32F);
	//Mat	tvec(1, 3, CV_32F);
	
	//solvePnP(outDim, inDim, cameraMatrix, distCoeff, rvec1, tvec);

	//Mat rvec;
	//Rodrigues(rvec1, rvec);
	//cout << rvec.at<float>(0, 0) << endl;
	//cout << "旋转" << endl;
	//cout << rvec << endl;
	//cout << "平移" << endl;
	//cout << tvec << endl;

	//[-0.4908487417993784, 0.7426926889754392, -0.4554943275348207;
	//-0.3682570382042339, 0.2969515762324557, 0.8810258311684764;
	//0.7895912021337448, 0.6001894126986005, 0.1277435023689902]

	//[216.7156610764334;
	//-303.5126719731619;
	//-4149.507534461693]

	//将3*3旋转矩阵和3*1平移矩阵合并为4*4矩阵
	Mat mTransform(4, 4, CV_32F);
	//for (int r = 0; r < 3; r++)
	//{
	//	for (int c = 0; c < 3; c++)
	//	{
	//		mTransform.at<float>(r, c) = rvec.at<float>(r, c);
	//	}
	//}

	mTransform.at<float>(0, 0) = -0.4908487417993784;
	mTransform.at<float>(0, 1) = 0.7426926889754392;
	mTransform.at<float>(0, 2) = -0.4554943275348207;
	mTransform.at<float>(0, 3) = 216.7156610764334;
	mTransform.at<float>(1, 0) = -0.3682570382042339;
	mTransform.at<float>(1, 1) = 0.2969515762324557;
	mTransform.at<float>(1, 2) = 0.8810258311684764;
	mTransform.at<float>(1, 3) = -303.5126719731619;
	mTransform.at<float>(2, 0) = 0.7895912021337448;
	mTransform.at<float>(2, 1) = 0.6001894126986005;
	mTransform.at<float>(2, 2) = 0.1277435023689902;
	mTransform.at<float>(2, 3) = -4149.507534461693;
	mTransform.at<float>(3, 0) = 0;
	mTransform.at<float>(3, 1) = 0;
	mTransform.at<float>(3, 2) = 0;
	mTransform.at<float>(3, 3) = 1;

	Mat mTransformInvert(4, 4, CV_32F);
	invert(mTransform, mTransformInvert);


	Mat point(4, 1, CV_32F);
	Mat test(3, 1, CV_32F);

	ofstream points;
	points.open("points.txt", ios::out);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double x = (double)970 / 232 * j / 1000;
			test.at<float>(0, 0) = x;
			double y = (double)490 / 273 * i / 1000;
			test.at<float>(1, 0) = y;
			test.at<float>(2, 0) = 1;
			//cout << test.at<float>(0, 0) << endl;
			//cout << test.at<float>(1, 0) << endl;
			point = mTransformInvert * cameraMatrixInvert * test;
			//cout << fixed << setprecision(10) << "point0 = " << point.at<float>(0, 0) << endl;
			points << fixed << setprecision(10) << point.at<float>(0, 0) << " " << point.at<float>(2, 0) << " " << point.at<float>(1, 0) << endl;
			//cout << point << endl;
		}
	}
	points.close();

	////读取txt文件
	//ifstream infile;
	//infile.open("1.txt");		//将文件流对象与文件连接起来 
	//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	//string s;
	//vector<string> v;
	//vector<double> tmp;
	//vector<vector<double>> all_data;
	////	all_data.resize(all_data.max_size() + 1);
	//while (getline(infile, s))
	//{
	//	//cout << s << endl;
	//	SplitString(s, v, " "); //可按多个字符来分隔;
	//	for (int i = 0; i < 3; i++)
	//	{
	//		tmp.push_back(atof(v[i].c_str()) * 1000);
	//	}
	//	all_data.push_back(tmp);
	//	v.clear();
	//	tmp.clear();
	//}

	//infile.close();             //关闭文件输入流 
	///*for (int i = 0; i < all_data.size(); i++)
	//{
	//cout << all_data[i][0] << " " << all_data[i][1] << " " << all_data[i][2] << endl;
	//}*/
	//cout << all_data.size() << endl;
	//vector<Point3f> all_points;
	//for (int i = 0; i < all_data.size(); i++)
	//{
	//	all_points.push_back(Point3f(all_data[i][0], all_data[i][1], all_data[i][2]));
	//}

	//std::vector<cv::Point2f> projectedPoints;
	//cv::projectPoints(all_points, rvec, tvec, cameraMatrix, distCoeff, projectedPoints);
	//for (int i = 0; i < projectedPoints.size(); i++)
	//{
	//	Point2f p = projectedPoints[i];
	//	if (p.y <= 1080)
	//	{
	//		circle(image, p, 5, Scalar(255, 255, 0), 1, 8, 0);
	//	}
	//}

	//imwrite("result-2.jpg", image);
	//cout << rvec << endl;
	//cout << tvec << endl;

	return 0;

}

