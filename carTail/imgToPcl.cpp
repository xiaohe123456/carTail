#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main12345000()
{
	//������û�е�ͼ�����еĲ��ֵ������С
	int width = 186;// 672;
	int height = 320;// 354;
	//�����ص�ת��Ϊ��ά��
	ofstream points;
	points.open("points.txt", ios::out);
	Point3d point;
	//�ڼ�������ʱ������ʵ���������x��y��z�е�һ��ֵΪ��ֵ����ͼ������ת��Ϊ��ά����
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//����x����ĵ�  ʵ�ʿ��/ͼ���ж�Ӧ���ֵ�������Ŀ*���/1000����λ����Ϊm��+��ʼ������
			//point.x = (double)4164.2 / 672 * j / 1000 - 3.67777;
			point.x = -0.979542;
			point.y = (double)(3253.41 - 2346.64) / 186 * j / 1000 + 2.34664;
			//point.y = 3.31432;	//����x��zƽ�棬y��Ϊ��ֵ
			//����z����ĵ�  ʵ�ʸ߶�/ͼ���ж�Ӧ���ֵ�������Ŀ*�߶�/1000+��ʼ������
			//point.z = (double)1247.98 / 354 * i / 1000 + 1.22306;
			point.z = (double)(597.086 - 447.072) / 320 * i / 1000 + 0.447072;
			//д���ļ�
			points << fixed << setprecision(6) << point.x << " " << point.y << " " << point.z << endl;
		}
	}
	points.close();
	return 0;
}