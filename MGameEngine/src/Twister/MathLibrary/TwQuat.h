#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include "TwVec3.h"

namespace MG
{
    class TwQuat {
    public:
        float w, x, y, z;

        // Constructors
        TwQuat() : w(1), x(0), y(0), z(0) {}  // Identity TwQuat
        TwQuat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

        // TwQuat multiplication
        TwQuat operator*(const TwQuat& other) const {
            return TwQuat(
                w * other.w - x * other.x - y * other.y - z * other.z,
                w * other.x + x * other.w + y * other.z - z * other.y,
                w * other.y - x * other.z + y * other.w + z * other.x,
                w * other.z + x * other.y - y * other.x + z * other.w
            );
        }

        // Scalar multiplication
        TwQuat operator*(float scalar) const {
            return TwQuat(w * scalar, x * scalar, y * scalar, z * scalar);
        }

        // Addition
        TwQuat operator+(const TwQuat& other) const {
            return TwQuat(w + other.w, x + other.x, y + other.y, z + other.z);
        }

        // Subtraction
        TwQuat operator-(const TwQuat& other) const {
            return TwQuat(w - other.w, x - other.x, y - other.y, z - other.z);
        }

        // Magnitude
        float magnitude() const {
            return std::sqrt(w * w + x * x + y * y + z * z);
        }

        // Normalization
        TwQuat normalized() const {
            float mag = magnitude();
            if (mag != 0) {
                return *this * (1.0f / mag);
            }
            else {
                return TwQuat();  // Return identity TwQuat if magnitude is zero
            }
        }

        // Conjugate
        TwQuat conjugate() const {
            return TwQuat(w, -x, -y, -z);
        }

        // Inverse
        TwQuat inverse() const {
            float magSq = w * w + x * x + y * y + z * z;
            if (magSq != 0) {
                return conjugate() * (1.0f / magSq);
            }
            else {
                throw std::runtime_error("Cannot invert a zero-magnitude TwQuat");
            }
        }

        // Rotate a vector by this TwQuat
        std::array<float, 3> rotate(const std::array<float, 3>& v) const {
            TwQuat qv(0, v[0], v[1], v[2]);
            TwQuat result = (*this) * qv * inverse();
            return { result.x, result.y, result.z };
        }

        // Rotate a vector by this TwQuat
        TwVec3 rotate(const TwVec3& v) const {
            TwQuat qv(0, v.x, v.y, v.z);
            TwQuat result = (*this) * qv * inverse();
            return { result.x, result.y, result.z };
        }

        // Create a TwQuat from an axis and an angle (in radians)
        static TwQuat fromAxisAngle(const std::array<float, 3>& axis, float angle) {
            float halfAngle = angle / 2.0f;
            float sinHalfAngle = std::sin(halfAngle);
            return TwQuat(std::cos(halfAngle), axis[0] * sinHalfAngle, axis[1] * sinHalfAngle, axis[2] * sinHalfAngle);
        }

        // Create a TwQuat from an axis and an angle (in radians)
        static TwQuat fromAxisAngle(const TwVec3& axis, float angle) {
            float halfAngle = angle / 2.0f;
            float sinHalfAngle = std::sin(halfAngle);
            return TwQuat(std::cos(halfAngle), axis.x * sinHalfAngle, axis.y * sinHalfAngle, axis.z * sinHalfAngle);
        }

        // Print TwQuat
        void print() const {
            std::cout << "TwQuat(" << w << ", " << x << ", " << y << ", " << z << ")\n";
        }
    };
}
