/*************************************************************************
    > File Name: Model.cpp
    > Author: Netcan
    > Blog: https://netcan.github.io/
    > Mail: netcan1996@gmail.com
    > Created Time: 2024-09-06 22:51
************************************************************************/
#include "Model.h"
#include <cassert>
#include <charconv>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ranges>

Model::Model(const char *filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail())
        return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        char trash;
        if (line.starts_with("v ")) {
            iss >> trash;
            Vec3f v{};
            iss >> v.x_() >> v.y_() >> v.z_();
            verts_.push_back(v);
        } else if (line.starts_with("vn ")) {
            iss >> trash >> trash;
            Vec3f v{};
            iss >> v.x_() >> v.y_() >> v.z_();
            normal_.push_back(v.normalize());
        } else if (line.starts_with("vt ")) {
            iss >> trash >> trash;
            Vec2f v{};
            iss >> v.x_() >> v.y_();
            uv_.push_back(v);
        } else if (line.starts_with("f ")) {
            handleFace(line);
        }
    }
}

void Model::handleFace(std::string_view line) {
    size_t nth = 0;
    // clang-format off
    auto f = line
        | std::views::drop(2)
        | std::views::split(' ')
        | std::views::transform([&](auto&& nface) {
            FaceIndex index{.nth = nth++};
            auto v = nface | std::views::split('/');
            auto vit = v.begin();
            if (vit != v.end()) {
                std::from_chars((*vit).begin(), (*vit).end(), index.vIndex);
                --index.vIndex;
            }

            if (++vit != v.end()) {
                std::from_chars((*vit).begin(), (*vit).end(), index.uvIndex);
                --index.uvIndex;
            }

            if (++vit != v.end()) {
                std::from_chars((*vit).begin(), (*vit).end(), index.nIndex);
                --index.nIndex;
            }
            return index;
        })
        | std::ranges::to<std::vector<FaceIndex>>();
    // clang-format on

    assert(f.size() == 3);
    faces_.push_back(f);

}
