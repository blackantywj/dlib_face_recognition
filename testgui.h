#ifndef TESTGUI_H
#define TESTGUI_H
#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include "dlib/image_processing.h"
#include "dlib/image_transforms/interpolation_abstract.h"
#include <dlib/clustering.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_transforms.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QtWidgets/QMainWindow>
#include "ui_testgui.h"
#include <QTimer>
#include <io.h>
using namespace cv;
using namespace std;
using namespace dlib;
#define PI 3.14159
template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
	input_rgb_image_sized<150>
	>>>>>>>>>>>>;


class testgui : public QMainWindow
{
	Q_OBJECT

public:
	testgui(QWidget *parent = 0);
	~testgui();
	shape_predictor sp; //人脸关键点检测器
	anet_type net;  // 人脸识别模型
	std::vector<matrix<float, 0, 1>> candidates_descriptors;
	std::vector<string> candidates;
	frontal_face_detector detector = get_frontal_face_detector(); // 人脸正脸检测器
	std::vector<matrix<rgb_pixel>> jitter_image(
		const matrix<rgb_pixel>& img
	);
	Mat image1;
	QImage show_image1;
	QImage show_image2;
	QTimer clk;
	int flag_1 = 0;
	int flag_2 = 0;
private:
	Ui::testguiClass ui;
private slots:
	void start();
	void process();
	void flag1();//开始按钮
	void flag2();//处理按钮
	void trainModel(QString file);
	cv::Rect Detect(cv::Mat im);
//	int candidates_train(const char *facesFile, std::vector<matrix<float, 0, 1>>&candidates_descriptors, std::vector<string>&candidates);
};

#endif // TESTGUI_H
