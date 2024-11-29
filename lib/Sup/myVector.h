/**
 * @brief 2次元ベクトルを表すクラスです。
 * 
 * このクラスは、2次元ベクトルの操作を行うための機能を提供します。
 * ベクトルの加算、減算、内積の計算、ベクトルの大きさの計算、ベクトルの正規化、
 * および疑似微分の計算が可能です。
 */

#pragma once

#include<Arduino.h>

// Getter for angle in radians

class Vector2D{
private:
    float x;
    float y;
    float azimuth;
    float arg;
    float magnitude;
public:
    // Constructors
    Vector2D() {x = 0, y = 0;};
    Vector2D(float x_, float y_);
    Vector2D(float azimuth_, float magnitude, int);

    void set_coodinate(float x,float y);
    void set_polar(float azimuth,float magnitude);
    float return_x();
    float return_y();
    float return_azimuth();
    float return_magnitude();
    float return_arg();


    // Destructor
    ~Vector2D();

    // Getters and setters
    void print();

    // Vector operations
    Vector2D add(const Vector2D& other) const;
    Vector2D subtract(const Vector2D& other) const;
    double dotProduct(const Vector2D& other) const;
    Vector2D normalize() const;

    // Pseudo differentiation
    Vector2D differentiate(const Vector2D& previous) const;
};

double operator*(const Vector2D& v1, const Vector2D& v2);
Vector2D operator*(Vector2D& v1, double scalar);
Vector2D operator*(double scalar, Vector2D& v1);
Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
Vector2D operator/(Vector2D& v1, double scalar);