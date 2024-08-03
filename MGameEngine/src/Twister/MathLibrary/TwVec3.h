#pragma once

#include <iostream>
#include <cmath>

namespace Twister
{
    class TwVec3 {
    public:
        float x, y, z;

        // Constructors
        TwVec3() : x(0), y(0), z(0) {}
        TwVec3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Addition
        TwVec3 operator+(const TwVec3& other) const {
            return TwVec3(x + other.x, y + other.y, z + other.z);
        }

        // Subtraction
        TwVec3 operator-(const TwVec3& other) const {
            return TwVec3(x - other.x, y - other.y, z - other.z);
        }

        // Scalar multiplication
        TwVec3 operator*(float scalar) const {
            return TwVec3(x * scalar, y * scalar, z * scalar);
        }

        // Scalar division
        TwVec3 operator/(float scalar) const {
            if (scalar != 0) {
                return TwVec3(x / scalar, y / scalar, z / scalar);
            }
            else {
                throw std::invalid_argument("Division by zero");
            }
        }

        // Dot product
        float dot(const TwVec3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        // Cross product
        TwVec3 cross(const TwVec3& other) const {
            return TwVec3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        // Magnitude
        float magnitude() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Normalization
        TwVec3 normalized() const {
            float mag = magnitude();
            if (mag != 0) {
                return *this / mag;
            }
            else {
                return TwVec3(0, 0, 0); // Return zero vector if magnitude is zero
            }
        }

        // Print vector
        void print() const {
            std::cout << "TwVec3(" << x << ", " << y << ", " << z << ")\n";
        }
    };
}
