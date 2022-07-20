#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	//点云中没有但图像中有的部分的区域大小
	int width = 672;		
	int height = 354;
	//将像素点转换为三维点
	ofstream points;
	points.open("points.txt", ios::out);
	Point3d point;
	//在计算坐标时，根据实际情况设置x、y、z中的一个值为定值，将图像坐标转换为三维数据
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//补充x方向的点  实际宽度/图像中对应部分的像素数目*宽度/1000（单位换算为m）+初始点坐标
			point.x = (double)4164.2 / 672 * j / 1000 - 3.67777;
			point.y = 3.31432;	//补充x、z平面，y设为定值
			//补充z方向的点  实际高度/图像中对应部分的像素数目*高度/1000+初始点坐标
			point.z = (double)1247.98 / 354 * i / 1000 + 1.22306;
			//写入文件
			points << fixed << setprecision(6) << point.x << " " << point.y << " " << point.z << endl;
		}
	}
	points.close();
	return 0;
}