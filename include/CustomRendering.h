/*************************************************************************
    > File Name: CustomRendering.h
    > Author: Netcan
    > Blog: https://netcan.github.io/
    > Mail: netcan1996@gmail.com
    > Created Time: 2024-08-16 22:31
************************************************************************/
#include <SDL.h>
#include <SDL_pixels.h>
#include <memory>
#include "Shader.h"
#include "utils/Delector.hpp"
#include "Geometry.hpp"
#pragma once

using ZBuffer = std::vector<uint8_t>;
struct Canvas {
    Canvas(int w, int h, SDL_Renderer *render)
        : surface_(SDL_CreateRGBSurfaceWithFormat(0, w, h, 0, SDL_PIXELFORMAT_RGB888)),
          texture_(SDL_CreateTextureFromSurface(render, surface_.get())) {}

    void *refresh() {
        SDL_UpdateTexture(texture_.get(), NULL, surface_->pixels, surface_->pitch);
        SDL_FillRect(surface_.get(), NULL, 0x000000);
        return texture_.get();
    }

    constexpr size_t point2Index(Point2i p) {
        return  p.y_() * surface_->w + p.x_();
    }

    void drawPixel(Point2i p, const Color& color);

    template<typename Shader>
    void triangle(const std::array<Point3i, 3> &vertex, const Shader &shader, ZBuffer &zbuffer);

    void bresenhamLine(Point2i p0, Point2i p1, const Color &color);

private:
    std::unique_ptr<SDL_Surface, utils::FpWrapper<SDL_FreeSurface>> surface_;
    std::unique_ptr<SDL_Texture, utils::FpWrapper<SDL_DestroyTexture>> texture_;
};

struct CustomRendering {
    CustomRendering(SDL_Renderer *render) : render_(render) { }
    void draw();
    void updateWindowSize();

    enum RenderType: int {
        WireFrameDraw,
        TextureDraw,
    };

private:
    void wireFrameDraw();

private:
    void textureDraw();
    void dumpZbuffer(const ZBuffer& zbuffer);
    void dumpLight();
    template<typename Shader>
    void viewerController(const Shader& shader);

private:
    SDL_Renderer *render_ {};

    int width_ = 960;
    int height_ = 720;
    Point2i   viewO_{};
    Point3f   camera_{1, 1, 3};
    Point3f   cameraUp_{0, 1, 0};
    Point3f   cameraO_{0, 0, 0};
    Vec3f     light_{0., 0., -1.};

    Canvas canvas_ { width_, height_, render_ };
    Canvas zbufferCanvas_ { width_, height_, render_ };

    TextureShader textureShader_ {light_};
    Color color_ {255, 255, 255};
    RenderType renderType_ {TextureDraw};
};

