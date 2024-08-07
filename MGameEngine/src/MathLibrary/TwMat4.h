#pragma once

#ifndef TWMAT4_H
#define TWMAT4_H

#include <iostream>
#include <cmath>
#include <array>
//#include "TwVec4.h"

namespace MG
{
    class TwMat4 {
    public:
        std::array<float, 16> elements;

        // Default constructor (identity matrix)
        TwMat4() {
            elements = { 1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1 };
        }

        // Constructor with elements
        TwMat4(float e0, float e1, float e2, float e3,
            float e4, float e5, float e6, float e7,
            float e8, float e9, float e10, float e11,
            float e12, float e13, float e14, float e15) {
            elements = { e0, e1, e2, e3,
                        e4, e5, e6, e7,
                        e8, e9, e10, e11,
                        e12, e13, e14, e15 };
        }

        // Matrix addition
        TwMat4 operator+(const TwMat4& other) const {
            TwMat4 result;
            for (int i = 0; i < 16; ++i) {
                result.elements[i] = elements[i] + other.elements[i];
            }
            return result;
        }

        // Matrix subtraction
        TwMat4 operator-(const TwMat4& other) const {
            TwMat4 result;
            for (int i = 0; i < 16; ++i) {
                result.elements[i] = elements[i] - other.elements[i];
            }
            return result;
        }

        // Matrix multiplication
        TwMat4 operator*(const TwMat4& other) const {
            TwMat4 result;
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    result.elements[row * 4 + col] = elements[row * 4 + 0] * other.elements[0 * 4 + col] +
                        elements[row * 4 + 1] * other.elements[1 * 4 + col] +
                        elements[row * 4 + 2] * other.elements[2 * 4 + col] +
                        elements[row * 4 + 3] * other.elements[3 * 4 + col];
                }
            }
            return result;
        }

        // Matrix-vector multiplication
        std::array<float, 4> operator*(const std::array<float, 4>& vec) const {
            return {
                elements[0] * vec[0] + elements[1] * vec[1] + elements[2] * vec[2] + elements[3] * vec[3],
                elements[4] * vec[0] + elements[5] * vec[1] + elements[6] * vec[2] + elements[7] * vec[3],
                elements[8] * vec[0] + elements[9] * vec[1] + elements[10] * vec[2] + elements[11] * vec[3],
                elements[12] * vec[0] + elements[13] * vec[1] + elements[14] * vec[2] + elements[15] * vec[3]
            };
        }

        // Transpose
        TwMat4 transpose() const {
            return TwMat4(
                elements[0], elements[4], elements[8], elements[12],
                elements[1], elements[5], elements[9], elements[13],
                elements[2], elements[6], elements[10], elements[14],
                elements[3], elements[7], elements[11], elements[15]
            );
        }

        // Determinant (only meaningful for 4x4 matrices)
        float determinant() const {
            return
                elements[3] * elements[6] * elements[9] * elements[12] - elements[2] * elements[7] * elements[9] * elements[12] -
                elements[3] * elements[5] * elements[10] * elements[12] + elements[1] * elements[7] * elements[10] * elements[12] +
                elements[2] * elements[5] * elements[11] * elements[12] - elements[1] * elements[6] * elements[11] * elements[12] -
                elements[3] * elements[6] * elements[8] * elements[13] + elements[2] * elements[7] * elements[8] * elements[13] +
                elements[3] * elements[4] * elements[10] * elements[13] - elements[0] * elements[7] * elements[10] * elements[13] -
                elements[2] * elements[4] * elements[11] * elements[13] + elements[0] * elements[6] * elements[11] * elements[13] +
                elements[3] * elements[5] * elements[8] * elements[14] - elements[1] * elements[7] * elements[8] * elements[14] -
                elements[3] * elements[4] * elements[9] * elements[14] + elements[0] * elements[7] * elements[9] * elements[14] +
                elements[1] * elements[4] * elements[11] * elements[14] - elements[0] * elements[5] * elements[11] * elements[14] -
                elements[2] * elements[5] * elements[8] * elements[15] + elements[1] * elements[6] * elements[8] * elements[15] +
                elements[2] * elements[4] * elements[9] * elements[15] - elements[0] * elements[6] * elements[9] * elements[15] -
                elements[1] * elements[4] * elements[10] * elements[15] + elements[0] * elements[5] * elements[10] * elements[15];
        }

        // Inverse
        TwMat4 inverse() const {
            float det = determinant();
            if (det == 0) {
                throw std::runtime_error("Matrix is singular and cannot be inverted");
            }
            float invDet = 1.0f / det;
            TwMat4 result;

            result.elements[0] = invDet * (elements[6] * elements[11] * elements[13] - elements[7] * elements[10] * elements[13] +
                elements[7] * elements[9] * elements[14] - elements[5] * elements[11] * elements[14] -
                elements[6] * elements[9] * elements[15] + elements[5] * elements[10] * elements[15]);
            result.elements[1] = invDet * (elements[3] * elements[10] * elements[13] - elements[2] * elements[11] * elements[13] -
                elements[3] * elements[9] * elements[14] + elements[1] * elements[11] * elements[14] +
                elements[2] * elements[9] * elements[15] - elements[1] * elements[10] * elements[15]);
            result.elements[2] = invDet * (elements[2] * elements[7] * elements[13] - elements[3] * elements[6] * elements[13] +
                elements[3] * elements[5] * elements[14] - elements[1] * elements[7] * elements[14] -
                elements[2] * elements[5] * elements[15] + elements[1] * elements[6] * elements[15]);
            result.elements[3] = invDet * (elements[3] * elements[6] * elements[9] - elements[2] * elements[7] * elements[9] -
                elements[3] * elements[5] * elements[10] + elements[1] * elements[7] * elements[10] +
                elements[2] * elements[5] * elements[11] - elements[1] * elements[6] * elements[11]);

            result.elements[4] = invDet * (elements[7] * elements[10] * elements[12] - elements[6] * elements[11] * elements[12] -
                elements[7] * elements[8] * elements[14] + elements[4] * elements[11] * elements[14] +
                elements[6] * elements[8] * elements[15] - elements[4] * elements[10] * elements[15]);
            result.elements[5] = invDet * (elements[2] * elements[11] * elements[12] - elements[3] * elements[10] * elements[12] +
                elements[3] * elements[8] * elements[14] - elements[0] * elements[11] * elements[14] -
                elements[2] * elements[8] * elements[15] + elements[0] * elements[10] * elements[15]);
            result.elements[6] = invDet * (elements[3] * elements[6] * elements[12] - elements[2] * elements[7] * elements[12] -
                elements[3] * elements[4] * elements[14] + elements[0] * elements[7] * elements[14] +
                elements[2] * elements[4] * elements[15] - elements[0] * elements[6] * elements[15]);
            result.elements[7] = invDet * (elements[2] * elements[7] * elements[8] - elements[3] * elements[6] * elements[8] +
                elements[3] * elements[4] * elements[10] - elements[0] * elements[7] * elements[10] -
                elements[2] * elements[4] * elements[11] + elements[0] * elements[6] * elements[11]);

            result.elements[8] = invDet * (elements[5] * elements[11] * elements[12] - elements[7] * elements[9] * elements[12] +
                elements[7] * elements[8] * elements[13] - elements[4] * elements[11] * elements[13] -
                elements[5] * elements[8] * elements[15] + elements[4] * elements[9] * elements[15]);
            result.elements[9] = invDet * (elements[3] * elements[9] * elements[12] - elements[1] * elements[11] * elements[12] -
                elements[3] * elements[8] * elements[13] + elements[0] * elements[11] * elements[13] +
                elements[1] * elements[8] * elements[15] - elements[0] * elements[9] * elements[15]);
            result.elements[10] = invDet * (elements[1] * elements[7] * elements[12] - elements[3] * elements[5] * elements[12] +
                elements[3] * elements[4] * elements[13] - elements[0] * elements[7] * elements[13] -
                elements[1] * elements[4] * elements[15] + elements[0] * elements[5] * elements[15]);
            result.elements[11] = invDet * (elements[3] * elements[5] * elements[8] - elements[1] * elements[7] * elements[8] -
                elements[3] * elements[4] * elements[9] + elements[0] * elements[7] * elements[9] +
                elements[1] * elements[4] * elements[11] - elements[0] * elements[5] * elements[11]);

            result.elements[12] = invDet * (elements[6] * elements[9] * elements[12] - elements[5] * elements[10] * elements[12] -
                elements[6] * elements[8] * elements[13] + elements[4] * elements[10] * elements[13] +
                elements[5] * elements[8] * elements[14] - elements[4] * elements[9] * elements[14]);
            result.elements[13] = invDet * (elements[1] * elements[10] * elements[12] - elements[2] * elements[9] * elements[12] +
                elements[2] * elements[8] * elements[13] - elements[0] * elements[10] * elements[13] -
                elements[1] * elements[8] * elements[14] + elements[0] * elements[9] * elements[14]);
            result.elements[14] = invDet * (elements[2] * elements[5] * elements[12] - elements[1] * elements[6] * elements[12] -
                elements[2] * elements[4] * elements[13] + elements[0] * elements[6] * elements[13] +
                elements[1] * elements[4] * elements[14] - elements[0] * elements[5] * elements[14]);
            result.elements[15] = invDet * (elements[1] * elements[6] * elements[8] - elements[2] * elements[5] * elements[8] +
                elements[2] * elements[4] * elements[9] - elements[0] * elements[6] * elements[9] -
                elements[1] * elements[4] * elements[10] + elements[0] * elements[5] * elements[10]);

            return result;
        }

        // Static methods for creating transformation matrices
        static TwMat4 translation(float tx, float ty, float tz) {
            return TwMat4(1, 0, 0, tx,
                0, 1, 0, ty,
                0, 0, 1, tz,
                0, 0, 0, 1);
        }

        static TwMat4 scale(float sx, float sy, float sz) {
            return TwMat4(sx, 0, 0, 0,
                0, sy, 0, 0,
                0, 0, sz, 0,
                0, 0, 0, 1);
        }

        static TwMat4 rotationX(float angle) {
            float c = std::cos(angle);
            float s = std::sin(angle);
            return TwMat4(1, 0, 0, 0,
                0, c, -s, 0,
                0, s, c, 0,
                0, 0, 0, 1);
        }

        static TwMat4 rotationY(float angle) {
            float c = std::cos(angle);
            float s = std::sin(angle);
            return TwMat4(c, 0, s, 0,
                0, 1, 0, 0,
                -s, 0, c, 0,
                0, 0, 0, 1);
        }

        static TwMat4 rotationZ(float angle) {
            float c = std::cos(angle);
            float s = std::sin(angle);
            return TwMat4(c, -s, 0, 0,
                s, c, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
        }

        // Print matrix
        void print() const {
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    std::cout << elements[row * 4 + col] << " ";
                }
                std::cout << std::endl;
            }
        }
    };
}

#endif // TWMAT4_H