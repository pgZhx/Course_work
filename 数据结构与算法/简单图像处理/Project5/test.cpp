#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
// 图像数据的读取与写入存储
Mat readImage(const string& filename) {
    Mat image = imread(filename);
    if (image.empty()) {
        cerr << "Failed to read image: " << filename << endl;
        exit(1);
    }
    return image;
}
void saveImage(const Mat& image, const string& filename) {
    imwrite(filename, image);
}
// 图像数据的压缩存储
void compressImage(Mat &image, const string& compressedFileName) {
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50); // Adjust the quality as needed

    imwrite(compressedFileName, image, compression_params);
}
Mat decompressImage(string compressedFileName) {
    return imread(compressedFileName, IMREAD_UNCHANGED);
}
// 彩色图像转换为灰度图像
Mat convertToGrayscale(const Mat& colorImage) {
    Mat grayscaleImage;
    cvtColor(colorImage, grayscaleImage, COLOR_BGR2GRAY);
    return grayscaleImage;
}
// 图像尺寸的缩放
Mat resizeImage(const Mat& image, int width, int height) {
    Mat resizedImage;
    resize(image, resizedImage, Size(width, height));
    return resizedImage;
}
int main() {
    // 读取图像
    system("cls");
    cout << "请输入需要处理的图片文件名" << endl;
    string filename;
    cin >> filename;
    Mat colorImage = readImage(filename);
    // 将彩色图像转换为灰度图像
    Mat grayscaleImage = convertToGrayscale(colorImage);
    // 保存灰度图像
    saveImage(grayscaleImage, "grayscale.png");
    // 将图像尺寸缩放为256x256
    Mat resizedImage = resizeImage(colorImage,256,256);
    // 保存缩放后的图像
    saveImage(resizedImage, "resized.png");
    // 压缩并保存图像数据
    compressImage(colorImage, "compressed.png");
    // 读取并解压缩图像数据
    Mat decompressedImage = decompressImage("compressed.png");
    // 保存解压缩后的图像
    saveImage(decompressedImage, "decompressed.png");
    // 展示图像
    namedWindow("Original Color Image", WINDOW_NORMAL);
    namedWindow("Grayscale Image", WINDOW_NORMAL);
    namedWindow("Resized Image", WINDOW_NORMAL);
    namedWindow("Decompressed Image", WINDOW_NORMAL);
    imshow("Original Color Image", colorImage);
    imshow("Grayscale Image", grayscaleImage);
    imshow("Resized Image", resizedImage);
    imshow("Decompressed Image", decompressedImage);
    waitKey(0);
    return 0;
}
