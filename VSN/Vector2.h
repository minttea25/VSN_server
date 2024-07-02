#pragma once

#include <cmath>

class Vector2
{
    static constexpr float TH = 1e-0f;
public:
    float x;
    float y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    void normalize()
    {
        float magnitude = std::sqrt(x * x + y * y);
        if (magnitude > TH)
        {
            x = x / magnitude;
            y = y / magnitude;
        }
        else
        {
            x = 0;
            y = 0;
        }
    }

    Vector2 DirectionVector(const Vector2& to) const
    {
        return Normalize(to - *this);
    }

    static Vector2 Normalize(const Vector2&& vector)
    {
        const float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);
        if (magnitude > TH) return Vector2(vector.x / magnitude, vector.y / magnitude);
        else return Vector2(0, 0);
    }

    // Get direction vector between two vectors
    static Vector2 DirectionVector(const Vector2& from, const Vector2& to)
    {
        return from.DirectionVector(to);
    }


    // Magnitude (length) of the vector
    float magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    // Squared magnitude (useful for comparison to avoid sqrt calculation)
    float squaredMagnitude() const
    {
        return x * x + y * y;
    }

    // Dot product
    static float dot(const Vector2& v1, const Vector2& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    // Addition
    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    // Subtraction
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    // Scalar multiplication
    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    // Scalar division
    Vector2 operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    // Equality check
    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    // Inequality check
    bool operator!=(const Vector2& other) const
    {
        return !(*this == other);
    }

    // Stream insertion for easy output
    friend std::ostream& operator<<(std::ostream& os, const Vector2& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }
};

