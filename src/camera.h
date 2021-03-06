/* 
 *  Copyright (c) 2015-2017  Rafael Pagés (rafael.pages (at) scss.tcd.ie)
 *
 *  This file is part of SSMVtex
 *
 *  SSMVtex is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  SSMVtex is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */


#ifndef CAMERA_H
#define CAMERA_H

#include <FreeImagePlus.h>

#include "mesh3d.h"

class Camera {

 public:
    Camera();
    virtual ~Camera();

    // Read parameters from text line
    void loadCameraParameters(const std::string& _textline);
    void loadBundlerCameraParameters(std::ifstream& _stream, const std::string& _imageName);
    bool loadImageDimensions(const std::string& _imageName);

    // Data access
    inline const Matrix3f& getIntrinsicParam() const {
        return K_;
    }
    inline const Matrix3f& getExtrinsicParam() const {
        return R_;
    }
    inline const Vector3f& getPosition() const {
        return position_;
    }
    inline Vector2i getImageDim() const {
        return Vector2i(imWidth_, imHeight_);
    }
    inline unsigned int getImageWidth() const{
        return imWidth_;
    }
    inline unsigned int getImageHeight() const{
        return imHeight_;
    }
    inline Vector3f getTranslationVector() const{ // T = -RC
        return -R_ * position_;
    }
    inline Vector2f getDistortionParams() const{
        return Vector2f(k1_,k2_);
    }

    // set camera position using the translation vector
    inline void setPosition(const Vector3f& _translation){
        position_ = -R_.transpose() * _translation;
    }



    // X = [R t] in homogeneous coordinates:
    // last row is [0 0 0 1]
    MatrixXf getXMatrix () const;
    // P = K [R T] in homogeneous coordinates:
    // last column of K is [0 0 0]'
    MatrixXf getProjectiveMatrix() const;

    // Coordinates transformations from world coordinates
    Vector3f transform2CameraCoord(const Vector3f& _v) const;
    Vector3f transform2TextureCoord(const Vector3f& _v) const;
    Vector2f transform2uvCoord(const Vector3f& _v) const;

    // Coordinates transformations from uv coordinates
    Vector3f get3Dpoint(const Vector2f& _p) const;

    // Ratings calculated for each triangle and vertex
    //std::vector<float> tri_ratings_;
    std::vector<float> vtx_ratings_;

 private:

    Matrix3f K_; // Intrinsic parameters
    Matrix3f R_; // Extrinsic parameters
    Vector3f position_; // C
    unsigned int imWidth_, imHeight_; //nPixX, nPixY
    float k1_, k2_; // Radial distortion coefficients




};

#endif // CAMERA_H
