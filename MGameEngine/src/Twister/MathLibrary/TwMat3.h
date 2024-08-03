#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include "TwVec3.h"

namespace Twister
{
    class TwMat3 {
    public:
        std::array<float, 9> elements;

        // Default constructor (identity matrix)
        TwMat3() {
            elements = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
        }

        // Constructor with elements
        TwMat3(float e0, float e1, float e2,
            float e3, float e4, float e5,
            float e6, float e7, float e8) {
            elements = { e0, e1, e2, e3, e4, e5, e6, e7, e8 };
        }

        void setDiagonal(float e0, float e4, float e8)
        {
            elements[0] = e0;
            elements[4] = e4;
            elements[8] = e8;
        }

        void operator*=(const float value)
        {
            for(int i = 0; elements.size(); ++i)
            {
                elements[i] *= value;
            }
        }

        // Matrix addition
        TwMat3 operator+(const TwMat3& other) const {
            return TwMat3(
                elements[0] + other.elements[0], elements[1] + other.elements[1], elements[2] + other.elements[2],
                elements[3] + other.elements[3], elements[4] + other.elements[4], elements[5] + other.elements[5],
                elements[6] + other.elements[6], elements[7] + other.elements[7], elements[8] + other.elements[8]
            );
        }

        // Matrix subtraction
        TwMat3 operator-(const TwMat3& other) const {
            return TwMat3(
                elements[0] - other.elements[0], elements[1] - other.elements[1], elements[2] - other.elements[2],
                elements[3] - other.elements[3], elements[4] - other.elements[4], elements[5] - other.elements[5],
                elements[6] - other.elements[6], elements[7] - other.elements[7], elements[8] - other.elements[8]
            );
        }

        // Matrix multiplication
        TwMat3 operator*(const TwMat3& other) const {
            return TwMat3(
                elements[0] * other.elements[0] + elements[1] * other.elements[3] + elements[2] * other.elements[6],
                elements[0] * other.elements[1] + elements[1] * other.elements[4] + elements[2] * other.elements[7],
                elements[0] * other.elements[2] + elements[1] * other.elements[5] + elements[2] * other.elements[8],

                elements[3] * other.elements[0] + elements[4] * other.elements[3] + elements[5] * other.elements[6],
                elements[3] * other.elements[1] + elements[4] * other.elements[4] + elements[5] * other.elements[7],
                elements[3] * other.elements[2] + elements[4] * other.elements[5] + elements[5] * other.elements[8],

                elements[6] * other.elements[0] + elements[7] * other.elements[3] + elements[8] * other.elements[6],
                elements[6] * other.elements[1] + elements[7] * other.elements[4] + elements[8] * other.elements[7],
                elements[6] * other.elements[2] + elements[7] * other.elements[5] + elements[8] * other.elements[8]
            );
        }

        // Matrix-vector multiplication
        std::array<float, 3> operator*(const std::array<float, 3>& vec) const {
            return {
                elements[0] * vec[0] + elements[1] * vec[1] + elements[2] * vec[2],
                elements[3] * vec[0] + elements[4] * vec[1] + elements[5] * vec[2],
                elements[6] * vec[0] + elements[7] * vec[1] + elements[8] * vec[2]
            };
        }

        // Matrix-vector multiplication
        std::array<float, 3> operator*(const TwVec3& vec) const {
            return {
                elements[0] * vec.x + elements[1] * vec.y + elements[2] * vec.z,
                elements[3] * vec.x + elements[4] * vec.y + elements[5] * vec.z,
                elements[6] * vec.x + elements[7] * vec.y + elements[8] * vec.z
            };
        }

        // Transpose
        TwMat3 transpose() const {
            return TwMat3(
                elements[0], elements[3], elements[6],
                elements[1], elements[4], elements[7],
                elements[2], elements[5], elements[8]
            );
        }

        // Determinant
        float determinant() const {
            return elements[0] * (elements[4] * elements[8] - elements[5] * elements[7]) -
                elements[1] * (elements[3] * elements[8] - elements[5] * elements[6]) +
                elements[2] * (elements[3] * elements[7] - elements[4] * elements[6]);
        }

        // Inverse
        TwMat3 inverse() const {
            float det = determinant();
            if (det == 0) {
                throw std::runtime_error("Matrix is singular and cannot be inverted");
            }

            float invDet = 1.0f / det;
            return TwMat3(
                (elements[4] * elements[8] - elements[5] * elements[7]) * invDet,
                (elements[2] * elements[7] - elements[1] * elements[8]) * invDet,
                (elements[1] * elements[5] - elements[2] * elements[4]) * invDet,

                (elements[5] * elements[6] - elements[3] * elements[8]) * invDet,
                (elements[0] * elements[8] - elements[2] * elements[6]) * invDet,
                (elements[2] * elements[3] - elements[0] * elements[5]) * invDet,

                (elements[3] * elements[7] - elements[4] * elements[6]) * invDet,
                (elements[1] * elements[6] - elements[0] * elements[7]) * invDet,
                (elements[0] * elements[4] - elements[1] * elements[3]) * invDet
            );
        }

        // Print matrix
        void print() const {
            std::cout << "TwMat3(\n";
            for (int i = 0; i < 3; ++i) {
                std::cout << "  ";
                for (int j = 0; j < 3; ++j) {
                    std::cout << elements[i * 3 + j] << " ";
                }
                std::cout << "\n";
            }
            std::cout << ")\n";
        }
    };
}