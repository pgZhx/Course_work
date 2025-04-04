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
// ͼ�����ݵĶ�ȡ��д��洢
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
// ͼ�����ݵ�ѹ���洢
void compressImage(Mat &image, const string& compressedFileName) {
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(50); // Adjust the quality as needed

    imwrite(compressedFileName, image, compression_params);
}
Mat decompressImage(string compressedFileName) {
    return imread(compressedFileName, IMREAD_UNCHANGED);
}
// ��ɫͼ��ת��Ϊ�Ҷ�ͼ��
Mat convertToGrayscale(const Mat& colorImage) {
    Mat grayscaleImage;
    cvtColor(colorImage, grayscaleImage, COLOR_BGR2GRAY);
    return grayscaleImage;
}
// ͼ��ߴ������
Mat resizeImage(const Mat& image, int width, int height) {
    Mat resizedImage;
    resize(image, resizedImage, Size(width, height));
    return resizedImage;
}
int main() {
    // ��ȡͼ��
    system("cls");
    cout << "��������Ҫ�����ͼƬ�ļ���" << endl;
    string filename;
    cin >> filename;
    Mat colorImage = readImage(filename);
    // ����ɫͼ��ת��Ϊ�Ҷ�ͼ��
    Mat grayscaleImage = convertToGrayscale(colorImage);
    // ����Ҷ�ͼ��
    saveImage(grayscaleImage, "grayscale.png");
    // ��ͼ��ߴ�����Ϊ256x256
    Mat resizedImage = resizeImage(colorImage,256,256);
    // �������ź��ͼ��
    saveImage(resizedImage, "resized.png");
    // ѹ��������ͼ������
    compressImage(colorImage, "compressed.png");
    // ��ȡ����ѹ��ͼ������
    Mat decompressedImage = decompressImage("compressed.png");
    // �����ѹ�����ͼ��
    saveImage(decompressedImage, "decompressed.png");
    // չʾͼ��
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
