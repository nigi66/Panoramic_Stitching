#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layoutV = new QVBoxLayout(centralWidget);

    imageLabel1 = new QLabel("No image loaded", this);
    imageLabel1->setAlignment(Qt::AlignCenter);

    imageLabel2 = new QLabel("No image loaded", this);
    imageLabel2->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(imageLabel1);
    layout->addWidget(imageLabel2);

    QPushButton *loadImage1Button = new QPushButton("Load First Image", this);
    QPushButton *loadImage2Button = new QPushButton("Load Second Image", this);
    QPushButton *stitchingButton = new QPushButton("Stitch Images", this);
    layoutV->addLayout(layout);
    layoutV->addWidget(loadImage1Button);
    layoutV->addWidget(loadImage2Button);
    layoutV->addWidget(stitchingButton);

    correspodenceLabel = new QLabel("No image loaded", this);
    correspodenceLabel->setAlignment(Qt::AlignCenter);
    layoutV->addWidget(correspodenceLabel);

    stitchImageLabel = new QLabel("No image loaded", this);
    stitchImageLabel->setAlignment(Qt::AlignCenter);
    layoutV->addWidget(stitchImageLabel);

    setCentralWidget(centralWidget);


    // Connect buttons to actions
    connect(loadImage1Button, &QPushButton::clicked, this, &MainWindow::loadFirstImage);
    connect(loadImage2Button, &QPushButton::clicked, this, &MainWindow::loadSecondImage);
    connect(stitchingButton, &QPushButton::clicked, this, &MainWindow::stitchingImages);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadFirstImage(){
    imagePath1 = QFileDialog::getOpenFileName(this, "Load Image 1");
    displayImage(imagePath1, imageLabel1);
}


void MainWindow::loadSecondImage(){
    imagePath2 = QFileDialog::getOpenFileName(this, "Load Image 1");
    displayImage(imagePath2, imageLabel2);
}



void MainWindow::stitchingImages() {
    if (imagePath1.isEmpty() || imagePath2.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please load both images first!");
        return;
    }

    // Load images with OpenCV
    cv::Mat img1 = cv::imread(imagePath1.toStdString());
    cv::Mat img2 = cv::imread(imagePath2.toStdString());
    if (img1.empty() || img2.empty()) {
        QMessageBox::warning(this, "Error", "Failed to load one or both images!");
        return;
    }

    cv::Mat gray1, gray2;
    cv::cvtColor(img1, gray1, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img2, gray2, cv::COLOR_BGR2GRAY);

    // Detect and compute features using ORB
    cv::Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    orb->detectAndCompute(gray1, cv::noArray(), keypoints1, descriptors1);
    orb->detectAndCompute(gray2, cv::noArray(), keypoints2, descriptors2);

    cv::BFMatcher matcher(cv::NORM_HAMMING, true);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Sort matches by distance
    std::sort(matches.begin(), matches.end(), [](const cv::DMatch &a, const cv::DMatch &b) {
        return a.distance < b.distance;
    });

    cv::Mat matchesImg;
    cv::drawMatches(img1, keypoints1, img2, keypoints2, matches, matchesImg, cv::Scalar::all(-1), cv::Scalar::all(-1),
                    std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    QString matchesPath = "matches.jpg";
    displayImage(matchesPath, correspodenceLabel);

    // stitching the images
    std::vector<cv::Mat> images = {img1, img2};
    cv::Mat pano;
    cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(mode);
    cv::Stitcher::Status status = stitcher->stitch(images, pano);

    if (status != cv::Stitcher::OK) {
        QMessageBox::warning(this, "Error", "Failed to stitch images!");
        return;
    }


    QString resultPath = "result.jpg";
    displayImage(resultPath, stitchImageLabel);
}


void MainWindow::displayImage(const QString &path, QLabel *label){
    QPixmap pixmap(path);
    label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
}


