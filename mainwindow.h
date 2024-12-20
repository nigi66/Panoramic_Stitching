#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QFileDialog>
#include <QComboBox>
#include <QTimer>
#include <QImage>
#include <QDebug>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/stitching/detail/matchers.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadFirstImage();
    void loadSecondImage();
    void stitchingImages();
    void displayImage(const QString &path, QLabel *label);

private:
    Ui::MainWindow *ui;

    QLabel *imageLabel1;
    QLabel *imageLabel2;
    QLabel *stitchImageLabel;
    QLabel *correspodenceLabel;
    QComboBox *stitchingModeCmb;

    QString imagePath1;
    QString imagePath2;

};
#endif // MAINWINDOW_H
