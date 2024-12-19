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

    stitchImageLabel = new QLabel("No image loaded", this);
    stitchImageLabel->setAlignment(Qt::AlignCenter);
    layoutV->addWidget(stitchImageLabel);

    setCentralWidget(centralWidget);


    // Connect buttons to actions
    connect(loadImage1Button, &QPushButton::clicked, this, &MainWindow::loadFirstImage);
    connect(loadImage2Button, &QPushButton::clicked, this, &MainWindow::loadSecondImage);
    connect(stitchingButton, &QPushButton::clicked, this, &MainWindow::stitchImages);

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


void MainWindow::stitchImages(){
    if (imagePath1.isEmpty() || imagePath2.isEmpty()) {
         QMessageBox::warning(this, "Error", "Please load both images first!");
         return;
     }

     cv::Mat img1 = cv::imread(imagePath1.toStdString());
     cv::Mat img2 = cv::imread(imagePath2.toStdString());
     if (img1.empty() || img2.empty()) {
         QMessageBox::warning(this, "Error", "Failed to load one or both images!");
         return;
     }

     // Stitch images
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
     cv::imwrite(resultPath.toStdString(), pano);
     displayImage(resultPath, stitchImageLabel);
}


void MainWindow::displayImage(const QString &path, QLabel *label){
    QPixmap pixmap(path);
    label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
}


