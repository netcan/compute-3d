/*************************************************************************
    > File Name: Shader.h
    > Author: Netcan
    > Blog: https://netcan.github.io/
    > Mail: netcan1996@gmail.com
    > Created Time: 2024-09-07 20:59
************************************************************************/
#pragma once

#include "Model.h"
#include "Geometry.hpp"
#include <format>
#include <print>

inline const char* LoadEnv(const char* name, const char* defaultValue = "") {
    if (auto v = std::getenv(name)) {
        return v;
    }
    return defaultValue;
}

struct TextureShader {
    TextureShader(const Vec3f& light) : light_(light) {}
    const auto& faces() const { return model_.faces_; }

    Point3i vertex(const Model::FaceIndex& index) {
        auto        v      = model_.verts_[index.vIndex].toHomogeneous();
        const auto& uv     = model_.uv_[index.uvIndex];
        Vec3f normal = Vec4f(uniformMIT_ * model_.normal_[index.nIndex].toHomogeneous());

        varyingUv_.setCol(index.nth, Vec2i(uv.x_() * diffuse_.width_, uv.y_() * diffuse_.height_));
        varyingNormal_.setCol(index.nth, normal);

        return (viewport_ * uniformM_ * v).toV().toAffine();
    }

    bool fragment(const Point3f& bar, Color& color) const {
        Vec2f uvP       = varyingUv_ * bar.toM();
        Vec3f normal    = varyingNormal_ * bar.toM();
        Vec3f l         = Vec4f(uniformM_ * light_.toHomogeneous());
        auto  intensity = std::clamp(-l.normalize() * normal.normalize(), 0.f, 1.f);

        color     = (diffuse_ ? diffuse_.get(uvP) : colors::white) * intensity;

        return false;
    }

    enum ShaderInfo {
        ModelInfo,
        MatrixInfo,
    };

    void dumpInfo(ShaderInfo info) const {
        switch (info) {
            case ModelInfo: {
                ImGui::Text("vertex: %zu vt: %zu normal: %zu faces: %zu", model_.verts_.size(), model_.uv_.size(),
                            model_.normal_.size(), model_.faces_.size());
                break;
            }
            case MatrixInfo: {
                ImGui::Text("%s", std::format("viewport_:\n{::6.2f}", viewport_).c_str());
                ImGui::Text("%s", std::format("uniformM_:\n{::5.2f}", uniformM_).c_str());
                ImGui::Text("%s", std::format("uniformMIT_:\n{::5.2f}", uniformMIT_).c_str());
                break;
            }
        }
    }

    void updateM(const Matrix44f& viewport, const Matrix44f& project, const Matrix44f& lookat) {
        viewport_ = viewport;
        uniformM_ = project * lookat;
        uniformMIT_ = uniformM_.invertTranspose();
    }

private:
    Matrix44f            uniformM_;
    Matrix44f            uniformMIT_;
    Matrix44f            viewport_;
    const Vec3f&         light_;
    const Model          model_{LoadEnv("MODEL")};
    const Texture        diffuse_{LoadEnv("TEXTURE")};

    Matrixi<2, 3> varyingUv_;
    Matrixf<3, 3> varyingNormal_;
};
