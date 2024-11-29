#include<myVector.h>

// Default constructor

Vector2D::Vector2D(float x_,float y_){
    this->x = x_;
    this->y = y_;
    arg = degrees(atan2(y_,x_));
    if(arg < -180){
        arg += 360;
    }
    if(180 < arg){
        arg -= 360;
    }
    azimuth = 90 - arg;
    if(azimuth < -180){
        azimuth += 360;
    }
    if(180 < azimuth){
        azimuth -= 360;
    }
    magnitude = sqrt(x * x + y * y);
}

Vector2D::Vector2D(float Azimuth,float Magnitude,int a){
    this->azimuth = Azimuth;
    arg = 90 - Azimuth;
    if(arg < -180){
        arg += 360;
    }
    if(180 < arg){
        arg -= 360;
    }
    this->magnitude = Magnitude;
    arg = 90 - azimuth;
    if(arg < -180){
        arg += 360;
    }
    if(180 < arg){
        arg -= 360;
    }
    x = magnitude * cos(radians(arg));
    y = magnitude * sin(radians(arg));
}
// Parameterized constructor

void Vector2D::set_coodinate(float X,float Y){
    this->x = X;
    this->y = Y;
    arg = degrees(atan2(Y,X));
    if(arg < -180){
        arg += 360;
    }
    if(180 < arg){
        arg -= 360;
    }

    azimuth = 90 - arg;
    if(azimuth < -180){
        azimuth += 360;
    }
    if(180 < azimuth){
        azimuth -= 360;
    }
    magnitude = sqrt(x * x + y * y);
}

void Vector2D::set_polar(float Azimuth,float Magnitude){
    this->azimuth = Azimuth;
    if(azimuth < -180){
        azimuth += 360;
    }
    if(180 < azimuth){
        azimuth -= 360;
    }
    this->magnitude = Magnitude;
    arg = 90 - azimuth;
    if(arg < -180){
        arg += 360;
    }
    if(180 < arg){
        arg -= 360;
    }
    x = magnitude * cos(radians(arg));
    y = magnitude * sin(radians(arg));
}

float Vector2D::return_x(){
    return x;
}

float Vector2D::return_y(){
    return y;
}

float Vector2D::return_azimuth(){
    return azimuth;
}

float Vector2D::return_magnitude(){
    return magnitude;
}

float Vector2D::return_arg(){
    return arg;
}
// Destructor
Vector2D::~Vector2D() {}


// Addition of two vectors
Vector2D Vector2D::add(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

// Subtraction of two vectors
Vector2D Vector2D::subtract(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

// Dot product of two vectors
double Vector2D::dotProduct(const Vector2D& other) const {
    return x * other.x + y * other.y;
}

// Normalization of the vector
Vector2D Vector2D::normalize() const{
    double mag = magnitude;
    return Vector2D(x / mag, y / mag);
}

// Pseudo differentiation
Vector2D Vector2D::differentiate(const Vector2D& previous) const {
    double dx = x - previous.x;
    double dy = y - previous.y;
    return Vector2D(dx, dy);
}



void Vector2D::print() {
    Serial.printf(" (θ,r) = (%.2f, %.2f) (x,y) = (%.2f, %.2f)", return_azimuth(),return_magnitude(), return_x(), return_y());
}



double operator*(const Vector2D& v1, const Vector2D& v2) {  // Dot product
    return v1.dotProduct(v2);
}

Vector2D operator*(Vector2D& v1, double scalar) {  // Scalar multiplication
    return Vector2D(v1.return_x() * scalar, v1.return_y() * scalar);
}

Vector2D operator*(double scalar, Vector2D& v1) {  // Scalar multiplication
    return Vector2D(v1.return_x() * scalar, v1.return_y() * scalar);
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {  // Vector addition
    return v1.add(v2);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {  // Vector subtraction
    return v1.subtract(v2);
}

Vector2D operator/(Vector2D& v1, double scalar){
    return v1 * (1 / scalar);
}