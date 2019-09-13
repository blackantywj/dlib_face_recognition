#include "testgui.h"
#include <QFileDialog>
#include <stdio.h>
#include <QWidget>
#include <QDir>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include<QMessageBox>
#include<Windows.h>
using namespace std;
using namespace dlib;

testgui::testgui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	clk.start(20);
	deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
	QObject::connect(&clk, SIGNAL(timeout()), this, SLOT(start()));
	QObject::connect(&clk, SIGNAL(timeout()), this, SLOT(process()));
}

testgui::~testgui()
{
}

void testgui::trainModel(QString filename)
{
	matrix<rgb_pixel> img;
	//matrix<float, 0, 1> candidates_descriptor;
	load_image(img, filename.toStdString());
	std::vector<dlib::rectangle> dets = detector(img);
	if (dets.size() != 0) {
		string name;
		std::vector<matrix<rgb_pixel>> faces;
		//deserialize("out.dat") >> faces;
		QStringList list = filename.split('/');
		QString Name = list.at(5);
		QStringList listname = Name.split('.');
		name = (listname.at(0)).toStdString();
		// printf("%s\n",name); 
		string candidate = name;
		//cout << "candidate: " << candidate << endl;
		candidates.push_back(candidate);
		full_object_detection shape = sp(img, dets[0]);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(face_chip);
		candidates_descriptors = net(faces);
		candidates_descriptors_vec.push_back(candidates_descriptors);
	}
}

void testgui::flag1()
{
	flag_1 = !flag_1;
}
VideoCapture capture1(700);

void testgui::start()
{
	//	if (flag_1 == 1)//////flag函数
	//	{
	capture1 >> image1;
	Size outSize;
	//image1 = imread(file, 1);
	cvtColor(image1, image1, CV_BGR2RGB);
	outSize.width = 400;
	outSize.height = 300;
	cv::resize(image1, image1, outSize, 0, 0, INTER_AREA);
	show_image1 = QImage((const unsigned char*)image1.data, image1.cols, image1.rows, image1.cols*image1.channels(), QImage::Format_RGB888);
	//namedWindow("1");
	//imshow("1", image1);
	if (flag_1 == 1) {
		try {
			ui.pic_1->setPixmap(QPixmap::fromImage(show_image1)); //pic--显示图片窗
			QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存图片"),
				QDir::currentPath(),
				tr("Images (*.png *.xpm *.jpg)"));
			cvtColor(image1, image1, CV_BGR2RGB);
			imwrite(fileName.toStdString(), image1);
			trainModel(fileName); 
			flag1();
		}
		catch (dlib::image_load_error) {
			flag1();
		}
		catch (cv::Exception) {
			flag1();

		}
		
		//imwrite(fileName.toStdString(),image1);
	}

	if (flag_3 == 1) {
		try {
			Size outSize;
			QString fileName1 = QFileDialog::getOpenFileName(this,
				tr("选择图像"),
				QDir::currentPath(),
				tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
			image3 = imread(fileName1.toStdString(),1);
			cvtColor(image3, image3, CV_BGR2RGB);
			outSize.width = 400;
			outSize.height = 300;
			cv::resize(image3, image3, outSize, 0, 0, INTER_AREA);
			show_image3 = QImage((const unsigned char*)image3.data, image3.cols, image3.rows, image3.cols*image3.channels(), QImage::Format_RGB888);
			trainModel(fileName1);
			flag3();
			ui.pic_1->setPixmap(QPixmap::fromImage(show_image3)); //pic--显示图片窗
		}
		catch (dlib::image_load_error) {
			cout << "error1" << endl;
			flag3();
		}
		catch (cv::Exception) {
			cout << "error" << endl;
			flag3();
		}
		//imwrite(fileName.toStdString(),image1);
	}
	
//	}
}

void testgui::flag3() 
{
	flag_3 = !flag_3;
}

void testgui::flag4()
{
	flag_4 = !flag_4;
}
void testgui::flag2()
{
	flag_2 = !flag_2;
}

cv::Rect testgui::Detect(cv::Mat im)
{
	cv::Rect R;
	array2d<bgr_pixel> img;
	assign_image(img, cv_image<uchar>(im));
	std::vector<dlib::rectangle> dets = detector(img);//检测人脸
												//查找最大脸
	if (dets.size() != 0)
	{
		int Max = 0;
		int area = 0;
		for (unsigned long t = 0; t < dets.size(); ++t)
		{
			if (area < dets[t].width()*dets[t].height())
			{
				area = dets[t].width()*dets[t].height();
				Max = t;
			}
		}
		R.x = dets[Max].left();
		R.y = dets[Max].top();
		R.width = dets[Max].width();
		R.height = dets[Max].height();
		//cout << "(" << R.x << "," << R.y << "," << R.width << "," << R.height << ")" << endl;
	}
	return R;
}
void testgui::process()
{
	if (flag_2 == 1)
	{
		//capture1 >> image2;
		//image2 = imread("wenjie.jpg", 1);
		//deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
		//deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
			capture1 >> image1;
			matrix<rgb_pixel> img;
			std::vector<matrix<rgb_pixel>> faces;
			chip_details dets;
			imwrite("./cap.jpg", image1); 
			cvtColor(image1, image1, CV_BGR2RGB);
			load_image(img, "cap.jpg");
			for (auto face : detector(img))
			{
				auto shape = sp(img, face);
				matrix<rgb_pixel> face_chip;
				dets = get_face_chip_details(shape, 150, 0.25);
				extract_image_chip(img, dets, face_chip);
				faces.push_back(face_chip);
			}
			if (faces.size() == 0)
			{
				cout << "No faces found in image!" << endl;
			}
			// This call asks the DNN to convert each face image in faces into a 128D vector.
			// In this 128D vector space, images from the same person will be close to each other
			// but vectors from different people will be far apart.  So we can use these vectors to
			// identify if a pair of images are from the same person or from different people.  
			std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);

			int num=0;
			// In particular, one simple thing we can do is face clustering.  This next bit of code
			// creates a graph of connected faces and then uses the Chinese whispers graph clustering
			// algorithm to identify how many people there are and which faces belong to whom.
			//std::vector<sample_pair> edges;
			for (size_t i = 0; i < face_descriptors.size(); ++i)
			{
				for (size_t j = 0; j < candidates_descriptors_vec.size(); ++j) {
					auto len = length(face_descriptors[i] - candidates_descriptors_vec[j][0]);
						if(len < 0.8) {
							cv::Mat dec;
							Size outSize;
							outSize.width = 400;
							outSize.height = 300;
							cv::resize(image1, image1, outSize, 0, 0, INTER_AREA);
							image1.copyTo(dec);
							cv::cvtColor(dec, dec, CV_BGR2GRAY);
							cv::Rect box;
							box = Detect(dec);
							cv::rectangle(image1, box, cv::Scalar(0, 0, 255), 1, 1, 0);
							cv::putText(image1, candidates[j], box.tl(), cv::FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 1, 8, false);
							show_image2 = QImage((const unsigned char*)image1.data, image1.cols, image1.rows, image1.cols*image1.channels(), QImage::Format_RGB888);
							num++;
						}
				}
			}
			if (num == 0) {
				QMessageBox::information(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("未匹配人脸"));
			}
			else {
				ui.pic_2->setPixmap(QPixmap::fromImage(show_image2)); //pic--显示图片窗
			}
			flag2();
	}
	if (flag_4 == 1)
	{
		try {
			QString fileName = QFileDialog::getOpenFileName(this,
				tr("选择图像"),
				QDir::currentPath(),
				tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
			//capture1 >> image2;
			image2 = imread(fileName.toStdString(), 1);
			cvtColor(image2, image2, CV_BGR2RGB);
			//deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
			//deserialize("dlib_face_recognition_resnet_model_v1.dat") >> net;
			matrix<rgb_pixel> img;
			std::vector<matrix<rgb_pixel>> faces;
			chip_details dets;
			//imwrite("./cap.jpg", image2); 
			load_image(img, fileName.toStdString());
			int num=0;
			for (auto face : detector(img))
			{
				auto shape = sp(img, face);
				matrix<rgb_pixel> face_chip;
				dets = get_face_chip_details(shape, 150, 0.25);
				extract_image_chip(img, dets, face_chip);
				faces.push_back(face_chip);
			}
			if (faces.size() == 0)
			{
				cout << "No faces found in image!" << endl;
			}
			// This call asks the DNN to convert each face image in faces into a 128D vector.
			// In this 128D vector space, images from the same person will be close to each other
			// but vectors from different people will be far apart.  So we can use these vectors to
			// identify if a pair of images are from the same person or from different people.  
			std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);


			// In particular, one simple thing we can do is face clustering.  This next bit of code
			// creates a graph of connected faces and then uses the Chinese whispers graph clustering
			// algorithm to identify how many people there are and which faces belong to whom.
			//std::vector<sample_pair> edges;
			for (size_t i = 0; i < face_descriptors.size(); ++i)
			{
				for (size_t j = 0; j < candidates_descriptors_vec.size(); ++j) {
					auto len = length(face_descriptors[i] - candidates_descriptors_vec[j][0]);
					if (len < 0.6) {
						cv::Mat dec;
						Size outSize;
						image2.copyTo(dec);
						cv::cvtColor(dec, dec, CV_BGR2GRAY);
						cv::Rect box;
						box = Detect(dec);
						cv::rectangle(image2, box, cv::Scalar(0, 0, 255), 1, 1, 0);
						cv::putText(image2, candidates[j], box.tl(), cv::FONT_HERSHEY_PLAIN, 1, (0, 255, 0), 1, 8, false);
						outSize.width = 400;
						outSize.height = 300;
						cv::resize(image2, image2, outSize, 0, 0, INTER_AREA);
						show_image2 = QImage((const unsigned char*)image2.data, image2.cols, image2.rows, image2.cols*image2.channels(), QImage::Format_RGB888);
						num++;
					}
				}
			}
			if (num == 0) {
				QMessageBox::information(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("未匹配人脸"));
			}
			flag4();
			ui.pic_2->setPixmap(QPixmap::fromImage(show_image2)); //pic--显示图片窗
		}
		catch (dlib::image_load_error) {
			flag4();
		}
		catch (cv::Exception) {
			flag4();
		}
	}
}
