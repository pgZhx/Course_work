#include <chrono>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>
#include <pthread.h>
#include <immintrin.h>
#include <omp.h>
using std::vector;
// 线程参数结构体
typedef struct {
    int thread_id;
    int start_row;
    int end_row;
    unsigned char* figure;
    unsigned char* result;
    size_t size;
} thread_data_t;   
// 线程执行函数
const __m256i one = _mm256_set1_epi16(1);
const __m256i two = _mm256_set1_epi16(2);
const __m256i four = _mm256_set1_epi16(4);
void powerLawTransformationSIMD(unsigned char* input, unsigned char* output, size_t size, const unsigned char* lut) {
    #pragma omp parallel for
    for (size_t i = 0; i < size * size; i += 16) {
        __m128i pixels = _mm_loadu_si128((__m128i*)&input[i]);
        __m128i transformed = _mm_set_epi8(
            lut[_mm_extract_epi8(pixels, 0)], lut[_mm_extract_epi8(pixels, 1)],
            lut[_mm_extract_epi8(pixels, 2)], lut[_mm_extract_epi8(pixels, 3)],
            lut[_mm_extract_epi8(pixels, 4)], lut[_mm_extract_epi8(pixels, 5)],
            lut[_mm_extract_epi8(pixels, 6)], lut[_mm_extract_epi8(pixels, 7)],
            lut[_mm_extract_epi8(pixels, 8)], lut[_mm_extract_epi8(pixels, 9)],
            lut[_mm_extract_epi8(pixels, 10)], lut[_mm_extract_epi8(pixels, 11)],
            lut[_mm_extract_epi8(pixels, 12)], lut[_mm_extract_epi8(pixels, 13)],
            lut[_mm_extract_epi8(pixels, 14)], lut[_mm_extract_epi8(pixels, 15)]
        );
        _mm_storeu_si128((__m128i*)&output[i], transformed);
    }
}

void* Internal_Gaussion(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    unsigned char* figure = data->figure;
    unsigned char* result = data->result;
    size_t size = data->size;
    #pragma omp parallel for
    for (int i = data->start_row; i < data->end_row; ++i) {
        if (i == 0 || i == size - 1) continue;

        for (int j = 1; j <= size - 16 - 1; j += 16) { // SIMD 处理 16 像素
            // 加载像素数据并扩展到 16 位以避免溢出
            __m256i top_left = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[(i - 1) * size + (j - 1)]));
            __m256i top = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[(i - 1) * size + j]));
            __m256i top_right = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[(i - 1) * size + (j + 1)]));

            __m256i mid_left = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[i * size + (j - 1)]));
            __m256i mid = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[i * size + j]));
            __m256i mid_right = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[i * size + (j + 1)]));

            __m256i bottom_left = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[(i + 1) * size + (j - 1)]));
            __m256i bottom = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[(i + 1) * size + j]));
            __m256i bottom_right = _mm256_cvtepu8_epi16(_mm_loadu_si128((__m128i*)&figure[(i + 1) * size + (j + 1)]));

            // 加权求和
            __m256i sum = _mm256_setzero_si256();
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(top_left, one));
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(top, two));
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(top_right, one));

            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(mid_left, two));
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(mid, four));
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(mid_right, two));

            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(bottom_left, one));
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(bottom, two));
            sum = _mm256_add_epi16(sum, _mm256_mullo_epi16(bottom_right, one));

            // 归一化并截断到 8 位
            sum = _mm256_srli_epi16(sum, 4); // 除以 16（2^4）
            __m128i final_result = _mm_packus_epi16(_mm256_castsi256_si128(sum), _mm256_extracti128_si256(sum, 1));

            // 存储结果
            _mm_storeu_si128((__m128i*)&result[i * size + j], final_result);
        }
        #pragma omp parallel for
        // 处理剩余的像素
        for (int j = size - 16; j < size - 1; ++j) {
            result[i * size + j] =
                (figure[(i - 1) * size + j - 1] + 2 * figure[(i - 1) * size + j] +
                 figure[(i - 1) * size + j + 1] + 2 * figure[i * size + j - 1] +
                 4 * figure[i * size + j] + 2 * figure[i * size + j + 1] +
                 figure[(i + 1) * size + j - 1] + 2 * figure[(i + 1) * size + j] +
                 figure[(i + 1) * size + j + 1]) / 16;
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
    // posix_memalign((void**)&figure, 32, size * size * sizeof(unsigned char));
    // posix_memalign((void**)&result, 32, size * size * sizeof(unsigned char));

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
      result[i*size+0] =
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
//   void powerLawTransformation() {
//     for (size_t i = 0; i < size; ++i) {
//       for (size_t j = 0; j < size; ++j) {
//         result[i*size+j] = power[figure[i*size+j]];
//       }
//     }

//   }
    void powerLawTransformation() {
        powerLawTransformationSIMD(figure, result, size, power);
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
    omp_set_num_threads(4);
    constexpr size_t size = 16384;
    FigureProcessor processor(size, argc > 1 ? std::stoul(argv[1]) : 0);
    processor.runBenchmark();
    return 0;
}
