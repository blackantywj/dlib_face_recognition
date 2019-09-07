#include "testgui.h"
#include <QFileDialog>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include<Windows.h>
#include<QMessageBox>
using namespace std;
using namespace dlib;
testgui::testgui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	deserialize("./shape_predictor_68_face_landmarks.dat") >> sp;
	deserialize("./dlib_face_recognition_resnet_model_v1.dat") >> net;
	clk.start(20);
	QObject::connect(&clk, SIGNAL(timeout()), this, SLOT(start()));
	QObject::connect(&clk, SIGNAL(timeout()), this, SLOT(process()));
}

testgui::~testgui()
{
}

void testgui::trainModel(QString file)
{
	string name;
	std::vector<matrix<rgb_pixel>> faces;
	//deserialize("out.dat") >> faces;
	QStringList list = file.split('.');
	name = (list.at(0)).toStdString();
	// printf("%s\n",name); 
	string candidate = name;
	//cout << "candidate: " << candidate << endl;
	candidates.push_back(candidate);
	matrix<rgb_pixel> img;
	load_image(img, file.toStdString());
	std::vector<dlib::rectangle> dets = detector(img);
	full_object_detection shape = sp(img, dets[0]);
	matrix<rgb_pixel> face_chip;
	extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
	faces.push_back(face_chip);
	candidates_descriptors = net(faces);
}

void testgui::flag1()
{
	flag_1 = !flag_1;
}
VideoCapture capture1(0);

void testgui::start()
{
	//	if (flag_1 == 1)//////flag∫Ø ˝
	//	{
	capture1 >> image1;
	cvtColor(image1, image1, CV_BGR2RGB);
	//cv::Mat dec;
	//image1.copyTo(dec);
	//cv::cvtColor(dec, dec, CV_BGR2GRAY);
	//cv::Rect box;
	//box = Detect(dec);
	//cv::rectangle(image1, box, cv::Scalar(0, 0, 255), 1, 1, 0);
	show_image1 = QImage((const unsigned char*)image1.data, image1.cols, image1.rows, image1.cols*image1.channels(), QImage::Format_RGB888);
	//namedWindow("1");
	//imshow("1", image1);
	if (flag_1 == 1) {
		QString fileName;
		fileName  = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("±£¥ÊÕº∆¨"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg)"));
		trainModel(fileName);
		flag1();
	}
	ui.pic_1->setPixmap(QPixmap::fromImage(show_image1)); //pic--œ‘ æÕº∆¨¥∞
//	}
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
	std::vector<dlib::rectangle> dets = detector(img);//ºÏ≤‚»À¡≥
												//≤È’“◊Ó¥Û¡≥
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
		while (true) {
			matrix<rgb_pixel> img;
			std::vector<matrix<rgb_pixel>> faces;
			chip_details dets;
			imwrite("./cap.jpg", image1); 
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
				break;
			}
			/*std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);
			std::vector<sample_pair> edges;
			for (size_t i = 0; i < face_descriptors.size(); ++i)
			{
				for (size_t j = i; j < face_descriptors.size(); ++j)
				{
					if (length(face_descriptors[i] - face_descriptors[j]) < 0.6)
						edges.push_back(sample_pair(i, j));
				}
			}
			std::vector<unsigned long> labels;
			const auto num_clusters = chinese_whispers(edges, labels);
			cout << "number of people found in the image: " << num_clusters << endl;
			std::vector<image_window> win_clusters(num_clusters);
			for (size_t cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
			{
				std::vector<matrix<rgb_pixel>> temp;
				for (size_t j = 0; j < labels.size(); ++j)
				{
					if (cluster_id == labels[j])
						temp.push_back(faces[j]);
				}
				win_clusters[cluster_id].set_title("face cluster " + cast_to_string(cluster_id));
				win_clusters[cluster_id].set_image(tile_images(temp));
			}*/
		}
		ui.pic_2->setPixmap(QPixmap::fromImage(show_image1)); //pic--œ‘ æÕº∆¨¥∞
	}
}
