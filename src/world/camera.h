//
// Created by Aleksi Lassila on 4.11.2023.
//

#ifndef PATHTRACER_CAMERA_H
#define PATHTRACER_CAMERA_H

#include "../utils/ray.hpp"
#include <cmath>
#include <algorithm>

#define DEFAULT_LOOK_INCREMENT 0.05
#define DEFAULT_MOVEMENT_INCREMENT 0.1

/**
 * Camera class is essentially a ray with some
 * extra properties and functionality such as fov
 * and rendering related stuff
 */
class Camera {

public:
    Camera(Vector position = Vector(0, 0, 0), double viewPlaneDistance = 1, double yaw = 0, double pitch = 0) :
            position_(position), viewPlaneDistance_(viewPlaneDistance), yaw_(yaw), pitch_(pitch) {
    }

    void MoveTo(const Vector &position) { position_ = position; }

    Vector GetPosition() { return position_; }

    void SetViewPlaneDistance(double viewPlaneDistance) { viewPlaneDistance_ = viewPlaneDistance; }

    [[nodiscard]] double GetViewPlaneDistance() const { return viewPlaneDistance_; }

    void SetYaw(double yaw) {
        yaw_ = std::fmod(yaw, 2 * M_PI);
        UpdateCamera_();
    }

    [[nodiscard]] double GetYaw() const { return yaw_; }

    void SetPitch(double pitch) {
        pitch_ = std::clamp(pitch, -M_PI / 2, M_PI / 2);
        UpdateCamera_();
    }

    [[nodiscard]] double GetPitch() const { return pitch_; }

    void LookAt(Vector target) {
        Vector direction = (target - position_).Norm();
        yaw_ = std::atan2(direction.x(), direction.z());
        pitch_ = std::asin(direction.y());
        direction_ = direction;
    }

    void SetFovDeg(double fov = 70) {
        viewPlaneDistance_ = 1 / std::tan(fov * M_PI / 360);
        UpdateCamera_();
    }

    [[nodiscard]] double GetFovDeg() const {
        return std::atan(1 / viewPlaneDistance_) * 360 / M_PI;
    }

    void IncrementMoveSpeed(double amount = DEFAULT_MOVEMENT_INCREMENT) {
        movementSpeed_ += amount;
        std::cout << "Speed: " << movementSpeed_ << std::endl;
    }

    void DecrementMoveSpeed(double amount = DEFAULT_MOVEMENT_INCREMENT) {
        movementSpeed_ -= amount;
        if (movementSpeed_ < 0) {
            movementSpeed_ = 0;
        }
        std::cout << "Speed: " << movementSpeed_ << std::endl;
    }

    void IncrementLookSensitivity(double amount = DEFAULT_LOOK_INCREMENT) {
        lookSpeed += amount;
        std::cout << "Angle Change Speed: " << lookSpeed << std::endl;
    }

    void DecrementLookSensitivity(double amount = DEFAULT_LOOK_INCREMENT) {
        lookSpeed -= amount;
        if (lookSpeed < 0) {
            lookSpeed = 0;
            std::cout << "Angle Change Speed: " << lookSpeed << std::endl;
        }
    }

    /**
     * @param xs x coordinate scaled between -1 and 1
     * @param ys y coordinate scaled between -1 and 1
     */
    Ray CastRay(double xs, double ys) {
        return Ray(position_, direction_ * viewPlaneDistance_ + (yCross_ * xs) + (xCross_ * ys));
    }

    void Rotate(double yawAdd, double pitchAdd) {
        SetYaw(yawAdd + yaw_);
        SetPitch(pitchAdd + pitch_);
        UpdateCamera_();
        std::cout << "Yaw: " << yaw_ << std::endl;
        std::cout << "Pitch: " << pitch_ << std::endl;
    }

    void Move(double x, double y, double z) {
        position_ += Vector(x, y, z);
    }

    void MoveForward() {
        position_ += direction_ * movementSpeed_;
    }

    void MoveBackward() {
        position_ += direction_ * -1 * movementSpeed_;
    }

    void MoveRight() {
        position_ += yCross_ * movementSpeed_;
    }

    void MoveLeft() {
        position_ += yCross_ * -1 * movementSpeed_;
    }

    void MoveUp() {
        position_ += xCross_ * -1 * movementSpeed_;
    }

    void MoveDown() {
        position_ += xCross_ * movementSpeed_;
    }

    void MoveUpAlongYaxis() {
        position_ += Vector(0, 1, 0) * movementSpeed_;
    }

    void MoveDownAlongYaxis() {
        position_ += Vector(0, -1, 0) * movementSpeed_;
    }

    void LookRight() {
        Rotate(lookSpeed, 0);
    }

    void LookLeft() {
        Rotate(-lookSpeed, 0);
    }

    void LookUp() {
        Rotate(0, lookSpeed);
    }

    void LookDown() {
        Rotate(0, -lookSpeed);
    }

private:
    void UpdateCamera_() {
        direction_ = Vector(std::sin(yaw_) * std::cos(pitch_), std::sin(pitch_),
                            std::cos(yaw_) * std::cos(pitch_));
        yCross_ = (Vector(-std::cos(yaw_), 0, std::sin(yaw_))).Norm();
        xCross_ = yCross_.CrossProduct(direction_).Norm();
    }

    Vector position_;
    Vector direction_;
    double viewPlaneDistance_;
    double yaw_ = 0;
    double pitch_ = 0;
    double lookSpeed = 0.05; // Angle increment
    double movementSpeed_ = 1;
    Vector yCross_;
    Vector xCross_;
};

#endif //PATHTRACER_CAMERA_H
