#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include <string>
#include <iostream>

class ObjLoader {
public:
    std::vector<std::shared_ptr<Mesh>> load(const std::string& filepath);
    void parseMaterial(const aiMaterial *material, std::shared_ptr<Mesh> &mesh);
    QOpenGLTexture *loadTexture(const std::string &texturePath);
};

