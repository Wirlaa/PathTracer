/*
 *Our own excellent vector class
 */

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include <iostream>
#include <cmath>

class Vector{
public:

    Vector(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    Vector operator+(Vector& b) {
	    return Vector(x_+b.x_, y_+b.y_, z_+b.z_);
    }
	    
    Vector operator-(Vector& b) {
	    return Vector(x_-b.x_, y_-b.y_, z_-b.z_);
    }

	/**
	 * \brief Multiplication by scalar
	 */
    Vector operator*(double k) {
	    return Vector(k*x_, k*y_, k*z_);
    }

	/**
	 * \brief Returns length of the vector 
	 */
    double Len() { 
	    return std::sqrt(std::pow(x_, 2) + std::pow(y_, 2) + std::pow(z_, 2));
    } 

    Vector Norm() {
	    return *this * (1/Len());
    } 

	/**
	 * \brief Dot Product 
	 */
    double operator*(Vector& v2) {
	    return x_ * v2.x_ + y_ * v2.y_ + z_ * v2.z_;
    }

	/**
	 * \brief Cross Product 
	 */
    Vector operator%(Vector& v2) {
	    return Vector(y_*v2.z_ - z_*v2.y_,  z_*v2.x_ - x_*v2.z_,
			    x_*v2.y_ - y_*v2.x_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector& v); 
    

private:

    double x_, y_, z_;

    
};

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << '(' << v.x_ << ',' << v.y_ << ',' << v.z_  << ')';
    return os;
}

#endif //! _VECTOR_HPP_