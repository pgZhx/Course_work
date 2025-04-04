#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>
#include <pthread.h>
using std::vector;
// 定义一个结构体，用于存储每个线程所需的参数
typedef struct {
    int thread_id;          // 线程的唯一标识符
    int start_row;          // 线程处理的起始行索引
    int end_row;            // 线程处理的结束行索引
    unsigned char* figure;  // 输入图像数据的指针
    unsigned char* result;  // 输出图像数据的指针
    size_t size;            // 图像的尺寸
} thread_data_t;
// 线程执行函数，每个线程都会调用这个函数来计算高斯滤波
void *Internal_Gaussion(void *arg) {
    thread_data_t* data = (thread_data_t *)arg; // 将传入的参数转换为 thread_data_t 类型
    unsigned char* figure = data->figure;        // 获取输入图像数据
    unsigned char* result = data->result;        // 获取输出图像数据
    size_t size = data->size;                    // 获取图像尺寸
    int i, j, k; 
    // 计算属于该线程的部分的高斯滤波
    for (i = data->start_row; i < data->end_row; ++i) { // 遍历线程负责的行
        for (j = 1; j < size - 1; ++j) { 
            //首尾行单独处理，线程函数仅处理内部区域
            if (i == 0) continue; 
            if (i == size - 1) continue; 
            // 应用高斯滤波公式计算结果
            result[i*size+j] = 
                (figure[(i - 1)*size+j - 1] + 2 * figure[(i - 1)*size+j] +
                 figure[(i - 1)*size+j + 1] + 2 * figure[i*size+j - 1] + 4 * figure[i*size+j] +
                 2 * figure[i*size+j + 1] + figure[(i + 1)*size+j - 1] +
                 2 * figure[(i + 1)*size+j] + figure[(i + 1)*size+j + 1]) /
                16;
        }
    }
    pthread_exit(NULL);
}
class FigureProcessor {
private:
  unsigned char* figure;
  unsigned char* result;
  const size_t size;
  unsigned char power[256];//建立查找表，用于优化幂次运算
  int num_threads = 64;
public:
  FigureProcessor(size_t size, size_t seed = 0) : size(size) {
    
    // !!! Please do not modify the following code !!!
    // 如果你需要修改内存的数据结构，请不要修改初始化的顺序和逻辑
    // 助教可能会通过指定某个初始化seed 的方式来验证你的代码
    // 如果你修改了初始化的顺序，可能会导致你的代码无法通过测试
    std::random_device rd;
    std::mt19937_64 gen(seed == 0 ? rd() : seed);
    std::uniform_int_distribution<unsigned char> distribution(0, 255);
    // !!! ----------------------------------------- !!!

    // 两个数组的初始化在这里，可以改动，但请注意 gen 的顺序是从上到下从左到右即可。
    this -> figure = new unsigned char[size*size];
    this -> result = new unsigned char[size*size];
    constexpr float gamma = 0.5f;
    for(int i = 0;i < 256;i++){
        float normalized = i / 255.0f;
        power[i] = i == 0 ? 0 : static_cast<unsigned char>(255.0f * std::pow(normalized, gamma) + 0.5f); 
    }
    for(int i = 0;i < size*size;i++){
        figure[i] = static_cast<unsigned char>(distribution(gen));
        result[i] = 0;
    }
  }

  ~FigureProcessor(){
    delete []figure;
    delete []result;
  }


  // Gaussian filter
  // [[1, 2, 1], [2, 4, 2], [1, 2, 1]] / 16
  //FIXME: Feel free to optimize this function
  //Hint: You can use SIMD instructions to optimize this function


  void gaussianFilter() {
    // 处理内部区域
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    int rows_per_thread = size / num_threads; // 每个线程负责的行数
    int remaining_rows = size % num_threads; // 处理剩余的行
    // 创建线程并分配任务
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == num_threads - 1) ? (i + 1) * rows_per_thread + remaining_rows : (i + 1) * rows_per_thread;
        thread_data[i].figure = this -> figure;
        thread_data[i].result = this -> result;
        thread_data[i].size = this -> size;     
        pthread_create(&threads[i], NULL, Internal_Gaussion, (void *)&thread_data[i]);
    }
    // 等待所有线程完成
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    for (size_t i = 1; i < size - 1; ++i) {
      result[i*size] =
          (figure[(i - 1)*size] + 2 * figure[(i - 1)*size] + figure[(i - 1)*size+1] +
           2 * figure[i*size] + 4 * figure[i*size] + 2 * figure[i*size+1] +
           figure[(i + 1)*size] + 2 * figure[(i + 1)*size] + figure[(i + 1)*size+1]) /
          16;
      result[i*size+size - 1] =
          (figure[(i - 1)*size+size - 2] + 2 * figure[(i - 1)*size+size - 1] +
           figure[(i - 1)*size+size - 1] + 2 * figure[i*size+size - 2] +
           4 * figure[i*size+size - 1] + 2 * figure[i*size+size - 1] +
           figure[(i + 1)*size+size - 2] + 2 * figure[(i + 1)*size+size - 1] +
           figure[(i + 1)*size+size - 1]) /
          16;
    }
    for (size_t j = 1; j < size - 1; ++j) {
      result[0*size+j] =
          (figure[j - 1] + 2 * figure[j] + figure[j + 1] +
           2 * figure[j - 1] + 4 * figure[j] + 2 * figure[j + 1] +
           figure[1*size+j - 1] + 2 * figure[1*size+j] + figure[1*size+j + 1]) /
          16;

      result[(size - 1)*size+j] =
          (figure[(size - 2)*size+j - 1] + 2 * figure[(size - 2)*size+j] +
           figure[(size - 2)*size+j + 1] + 2 * figure[(size - 1)*size+j - 1] +
           4 * figure[(size - 1)*size+j] + 2 * figure[(size - 1)*size+j + 1] +
           figure[(size - 1)*size+j - 1] + 2 * figure[(size - 1)*size+j] +
           figure[(size - 1)*size+j + 1]) /
          16;
    }
    // 处理四个角点
    // 左上角
    result[0] = (4 * figure[0] + 2 * figure[1] + 2 * figure[1*size+0] +
                    figure[1*size+1]) /
                   9; 
    // 右上角
    result[size - 1] = (4 * figure[size - 1] + 2 * figure[size - 2] +
                           2 * figure[1*size+size - 1] + figure[1*size+size - 2]) /
                          9;
    // 左下角
    result[(size - 1)*size+0] = (4 * figure[(size - 1)*size+0] + 2 * figure[(size - 1)*size+1] +
                           2 * figure[(size - 2)*size+0] + figure[(size - 2)*size+1]) /
                          9;
    // 右下角
    result[(size - 1)*size+size - 1] =
        (4 * figure[(size - 1)*size+size - 1] + 2 * figure[(size - 1)*size+size - 2] +
         2 * figure[(size - 2)*size+size - 1] + figure[(size - 2)*size+size - 2]) /
        9;
  }

  // Power law transformation
  // FIXME: Feel free to optimize this function
  // Hint: LUT to optimize this function?
  void powerLawTransformation() {
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        result[i*size+j] = power[figure[i*size+j]];
      }
    }

  }
  // Run benchmark
    unsigned int calcChecksum() {
        unsigned int sum = 0;
        constexpr size_t mod = 1000000007;
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                sum += result[i*size+j];
                sum %= mod;
            }
        }
        return sum;
    }
  void runBenchmark() {
    auto start = std::chrono::high_resolution_clock::now();
    gaussianFilter();
    auto middle = std::chrono::high_resolution_clock::now();

    unsigned int sum = calcChecksum();

    auto middle2 = std::chrono::high_resolution_clock::now();
    powerLawTransformation();
    auto end = std::chrono::high_resolution_clock::now();

    sum += calcChecksum();
    sum %= 1000000007;
    std::cout << "Checksum: " << sum << "\n";

    auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(middle - start) +
        std::chrono::duration_cast<std::chrono::milliseconds>(end - middle2);
    std::cout << "Benchmark time: " << milliseconds.count() << " ms\n";
  }
};

// Main function
// !!! Please do not modify the main function !!!
int main(int argc, const char **argv) {
    
    constexpr size_t size = 16384;
    FigureProcessor processor(size, argc > 1 ? std::stoul(argv[1]) : 0);
    processor.runBenchmark();
    return 0;
}
