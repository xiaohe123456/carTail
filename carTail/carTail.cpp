#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctype.h>
using namespace cv;
using namespace std;
int main123450()
{
    // 首先通过标定板的图像像素坐标以及对应的世界坐标，通过PnP求解相机的R&T//
    Point2f point;
    vector<Point2f> boxPoints; //存入像素坐标

    // Setting box corners in image
    //one Point
    point = Point2f((float)729, (float)23); //640X480
    boxPoints.push_back(point);
    

    //two Point
    point = Point2f((float)1534, (float)135); //640X480
    boxPoints.push_back(point);
    

    //three Point
    point = Point2f((float)1520, (float)767); //640X480
    boxPoints.push_back(point);
    

    //four Point
    point = Point2f((float)759, (float)956); //640X480
    boxPoints.push_back(point);
    

    //Setting box corners in real world
    vector<Point3f> worldBoxPoints;  //存入世界坐标
    Point3f tmpPoint;
    tmpPoint = Point3f((float)-4938.41, (float)3620.12, (float)0);
    worldBoxPoints.push_back(tmpPoint);
    tmpPoint = Point3f((float)-4991.92, (float)1223.53, (float)0);
    worldBoxPoints.push_back(tmpPoint);
    tmpPoint = Point3f((float)-4901.97, (float)1170.78, (float)0);
    worldBoxPoints.push_back(tmpPoint);
    tmpPoint = Point3f((float)-4931.1, (float)3644.98, (float)0);
    worldBoxPoints.push_back(tmpPoint);
    //camera  intristic///
    cv::Mat cameraMatrix1 = Mat::eye(3, 3, cv::DataType<double>::type);  //相机内参矩阵
    cv::Mat distCoeffs1(5, 1, cv::DataType<double>::type);  //畸变参数
    distCoeffs1.at<double>(0, 0) = -0.401070471240794;
    distCoeffs1.at<double>(1, 0) = 0.230454752485496;
    distCoeffs1.at<double>(2, 0) = -0.003874071083139;
    distCoeffs1.at<double>(3, 0) = 0.001197211137004;
    distCoeffs1.at<double>(4, 0) = -0.133297293899286;

    //Taken from Mastring OpenCV d
    double fx = 1.348231749848276e+03;
    double fy = 1.349403880998824e+03;
    double cx = 9.890477762727005e+02;
    double cy = 5.874783711978498e+02;
    cameraMatrix1.at<double>(0, 0) = fx;
    cameraMatrix1.at<double>(0, 1) = 0.188888118498424;
    cameraMatrix1.at<double>(1, 1) = fy;
    cameraMatrix1.at<double>(0, 2) = cx;
    cameraMatrix1.at<double>(1, 2) = cy;

    //PnP solve R&T///
    cv::Mat rvec1(3, 1, cv::DataType<double>::type);  //旋转向量
    cv::Mat tvec1(3, 1, cv::DataType<double>::type);  //平移向量
    cv::solvePnP(worldBoxPoints, boxPoints, cameraMatrix1, distCoeffs1, rvec1, tvec1, false, CV_ITERATIVE);
    cv::Mat rvecM1(3, 3, cv::DataType<double>::type);  //旋转矩阵
    cv::Rodrigues(rvec1, rvecM1);

    // 此处用于求相机位于坐标系内的旋转角度, 2D - 3D的转换并不用求
    const double PI = 3.1415926;
    double thetaZ = atan2(rvecM1.at<double>(1, 0), rvecM1.at<double>(0, 0)) / PI * 180;
    double thetaY = atan2(-1 * rvecM1.at<double>(2, 0), sqrt(rvecM1.at<double>(2, 1) * rvecM1.at<double>(2, 1)
        + rvecM1.at<double>(2, 2) * rvecM1.at<double>(2, 2))) / PI * 180;
    double thetaX = atan2(rvecM1.at<double>(2, 1), rvecM1.at<double>(2, 2)) / PI * 180;
    cout << "theta x  " << thetaX << endl << "theta Y: " << thetaY << endl << "theta Z: " << thetaZ << endl;

    ///根据公式求Zc，即s
    cv::Mat imagePoint = cv::Mat::ones(3, 1, cv::DataType<double>::type);
    cv::Mat tempMat, tempMat2;
    //输入一个2D坐标点，便可以求出相应的s
    imagePoint.at<double>(0, 0) = 714;
    imagePoint.at<double>(1, 0) = 23;
    double zConst = 0;//实际坐标系的距离
    //计算参数s
    double s;
    tempMat = rvecM1.inv() * cameraMatrix1.inv() * imagePoint;
    tempMat2 = rvecM1.inv() * tvec1;
    s = zConst + tempMat2.at<double>(2, 0);
    s /= tempMat.at<double>(2, 0);
    cout << "s : " << s << endl;
    ///3D to 2D
    cv::Mat worldPoints = Mat::ones(4, 1, cv::DataType<double>::type);
    worldPoints.at<double>(0, 0) = -4938.41;
    worldPoints.at<double>(1, 0) = 3620.12;
    worldPoints.at<double>(2, 0) = 2956.16;
    cout << "world Points :  " << worldPoints << endl;
    Mat image_points = Mat::ones(3, 1, cv::DataType<double>::type);
    //setIdentity(image_points);
    Mat RT_;
    hconcat(rvecM1, tvec1, RT_);
    cout << "RT_" << RT_ << endl;
    image_points = cameraMatrix1 * RT_ * worldPoints;
    Mat D_Points = Mat::ones(3, 1, cv::DataType<double>::type);
    D_Points.at<double>(0, 0) = image_points.at<double>(0, 0) / image_points.at<double>(2, 0);
    D_Points.at<double>(1, 0) = image_points.at<double>(1, 0) / image_points.at<double>(2, 0);
    //cv::projectPoints(worldPoints, rvec1, tvec1, cameraMatrix1, distCoeffs1, imagePoints);
    cout << "3D to 2D:   " << D_Points << endl;

    //camera_coordinates
    Mat camera_cordinates = -rvecM1.inv() * tvec1;
    // 2D to 3D///
    cv::Mat imagePoint_your_know = cv::Mat::ones(3, 1, cv::DataType<double>::type); //u,v,1
    imagePoint_your_know.at<double>(0, 0) = 714;
    imagePoint_your_know.at<double>(1, 0) = 23;
    Mat wcPoint = rvecM1.inv() * (cameraMatrix1.inv() * s * imagePoint_your_know - tvec1);
    Point3f worldPoint(wcPoint.at<double>(0, 0), wcPoint.at<double>(1, 0), wcPoint.at<double>(2, 0));
    cout << "2D to 3D :" << worldPoint << endl;

    return 0;
}
