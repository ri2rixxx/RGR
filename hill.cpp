#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <functional> 
#include <cmath>   
#include "hill.hpp"

using namespace std;

const int MOD = 256;

template <typename T>
ostream& operator<<(ostream& os, vector<T> vec){
    for(T i : vec){
        os << i << ' ';
    }
    os << endl;
    return os;
}

// Расширенный алгоритм Евклида
int gcdExtended(int a, int b, int &x, int &y)
{
    if (a == 0)
    {
        x = 0;
        y = 1;
        return b;
    }
    int x1, y1;
    int gcd = gcdExtended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

// Функция для нахождения обратного элемента по модулю
int modInverse(int a, int m)
{
    int x, y;
    int g = gcdExtended(a, m, x, y);
    if (g != 1)
    {
        throw invalid_argument("Обратного элемента не существует");
    }
    return (x % m + m) % m;
}

// Функция для вычисления определителя матрицы
int calculateDeterminant(const vector<vector<int> > &matrix)
{
    int n = matrix.size();
    if (n == 1)
        return matrix[0][0];

    vector<vector<int> > lu(matrix);
    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 0);

    int det = 1;

    for (int k = 0; k < n; ++k)
    {
        // Поиск pivot элемента
        int pivot = k;
        for (int i = k + 1; i < n; ++i)
        {
            if (abs(lu[i][k]) > abs(lu[pivot][k]))
            {
                pivot = i;
            }
        }

        if (pivot != k)
        {
            swap(lu[k], lu[pivot]);
            swap(permutation[k], permutation[pivot]);
            det = -det;
        }

        if (lu[k][k] == 0)
        {
            return 0; 
        }

        det = (det * lu[k][k]) % MOD;


        for (int i = k + 1; i < n; ++i)
        {
            lu[i][k] = (lu[i][k] * modInverse(lu[k][k], MOD)) % MOD;
            for (int j = k + 1; j < n; ++j)
            {
                lu[i][j] = (lu[i][j] - lu[i][k] * lu[k][j]) % MOD;
                if (lu[i][j] < 0)
                    lu[i][j] += MOD;
            }
        }
    }

    return det;
}
void generateInvertibleMatrix(vector<vector<int>>& matrix, const string& key) {
    hash<string> hasher;
    mt19937 gen(hasher(key));
    uniform_int_distribution<int> dist(1, 255);
    
    matrix.resize(16, vector<int>(16, 0));
    

    for(int i = 0; i < 16; ++i) {
        matrix[i][i] = dist(gen) | 1;
    }
    
    for(int i = 0; i < 16; ++i) {
        for(int j = i+1; j < 16; ++j) {
            matrix[i][j] = dist(gen);
        }
    }
}
vector<vector<int>> inverseMatrix(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    if (n == 0) throw invalid_argument("Матрица пуста");
    
    // Создаем расширенную матрицу [matrix|I]
    vector<vector<int>> aug(n, vector<int>(2 * n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            aug[i][j] = matrix[i][j];
        }
        aug[i][n + i] = 1;
    }
    
    // Прямой ход метода Гаусса
    for (int col = 0; col < n; ++col) {
        int pivot = -1;
        for (int i = col; i < n; ++i) {
            if (aug[i][col] != 0) {
                pivot = i;
                break;
            }
        }
        
        if (pivot == -1) {
            throw invalid_argument("Матрица вырождена");
        }
        
        if (pivot != col) {
            swap(aug[col], aug[pivot]);
        }

        int inv_val = modInverse(aug[col][col], MOD);
        for (int j = 0; j < 2 * n; ++j) {
            aug[col][j] = (aug[col][j] * inv_val) % MOD;
            if (aug[col][j] < 0) aug[col][j] += MOD;
        }

        for (int i = 0; i < n; ++i) {
            if (i != col && aug[i][col] != 0) {
                int factor = aug[i][col];
                for (int j = 0; j < 2 * n; ++j) {
                    aug[i][j] = (aug[i][j] - factor * aug[col][j]) % MOD;
                    if (aug[i][j] < 0) aug[i][j] += MOD;
                }
            }
        }
    }

    vector<vector<int>> inverse(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = aug[i][n + j];
        }
    }
    
    return inverse;
}

bool isInvertible(const vector<vector<int>>& matrix) {
    try {
        inverseMatrix(matrix); 
        return true;
    } catch (...) {
        return false;
    }
}

void fill_key(vector<vector<int>>& matrix, const string& key) {
    int attempts = 0;
    const int max_attempts = 3; 
    
    do {
        generateInvertibleMatrix(matrix, key + to_string(attempts));
        attempts++;
    } while(attempts < max_attempts && !isInvertible(matrix));
    
    if(!isInvertible(matrix)) {
        matrix.resize(16, vector<int>(16, 0));
        for(int i = 0; i < 16; ++i) {
            matrix[i][i] = 1; 
            for(int j = i+1; j < 16; ++j) {
                matrix[i][j] = 1; 
            }
        }
    }
}

// Функция для умножения матрицы на вектор
vector<int> matrixMultiply(const vector<vector<int> > &matrix, const vector<int> &vec)
{
    if (matrix.empty() || matrix[0].size() != vec.size())
    {
        throw invalid_argument("Несовместимые размеры матрицы и вектора");
    }

    vector<int> result(matrix.size(), 0);
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < vec.size(); ++j)
        {
            result[i] += matrix[i][j] * vec[j];
        }
        result[i] %= MOD;
    }
    return result;
}

// Функция шифрования Хилла
string hillCipherEncrypt(const string &plaintext, const string &str_key)
{
    vector<vector<int> > key;
    fill_key(key, str_key);
    int blockSize = key.size();
    if (blockSize == 0)
        throw invalid_argument("Ключ пуст");

    string text = plaintext;
    int padding = (blockSize - (text.size() % blockSize)) % blockSize;
    text.append(padding, static_cast<char>(0));
    string ciphertext;
    for (size_t i = 0; i < text.size(); i += blockSize)
    {
        vector<int> block(blockSize);
        for (int j = 0; j < blockSize; ++j)
        {
            block[j] = static_cast<unsigned char>(text[i + j]);
        }

        vector<int> encryptedBlock = matrixMultiply(key, block);

        for (int num : encryptedBlock)
        {
            ciphertext += static_cast<char>(num);
        }
    }

    return ciphertext;
}

// Функция расшифровки Хилла
string hillCipherDecrypt(const string &ciphertext, const string &str_key)
{
    vector<vector<int> > key;
    fill_key(key, str_key);
    int blockSize = key.size();
    if (blockSize == 0)
        throw invalid_argument("Ключ пуст");
    if (ciphertext.size() % blockSize != 0)
    {
        throw invalid_argument("Длина шифротекста должна быть кратна размеру блока");
    }

    vector<vector<int> > inverseKey;
    try
    {
        inverseKey = inverseMatrix(key);
    }
    catch (const invalid_argument &e)
    {
        throw invalid_argument("Невозможно расшифровать: " + string(e.what()));
    }

    string plaintext;
    for (size_t i = 0; i < ciphertext.size(); i += blockSize)
    {
        vector<int> block(blockSize);
        for (int j = 0; j < blockSize; ++j)
        {
            block[j] = static_cast<unsigned char>(ciphertext[i + j]);
        }

        vector<int> decryptedBlock = matrixMultiply(inverseKey, block);

        for (int num : decryptedBlock)
        {
            plaintext += static_cast<char>(num);
        }
    }

    while (!plaintext.empty() && plaintext.back() == 0)
    {
        plaintext.pop_back();
    }

    return plaintext;
}
