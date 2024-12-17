#include "obj_loader.h"

std::vector<std::shared_ptr<Mesh>> ObjLoader::load(const std::string& filepath) {
    std::vector<std::shared_ptr<Mesh>> meshes;
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return meshes;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        const aiMesh* mesh = scene->mMeshes[i];

        std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>(glm::vec3(0.0f), glm::vec3(1.0f));

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            const aiVector3D& aiVertex = mesh->mVertices[j];
            newMesh->vertices.emplace_back(aiVertex.x, aiVertex.y, aiVertex.z);

            if (mesh->HasNormals()) {
                const aiVector3D& aiNormal = mesh->mNormals[j];
                newMesh->normals.emplace_back(aiNormal.x, aiNormal.y, aiNormal.z);
            }

            if (mesh->HasTextureCoords(0)) {
                const aiVector3D& aiTexCoord = mesh->mTextureCoords[0][j];
                newMesh->texCoords.emplace_back(aiTexCoord.x, aiTexCoord.y);
            } else {
                newMesh->texCoords.emplace_back(0.0f, 0.0f);
            }
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                newMesh->indices.push_back(face.mIndices[k]);
            }
        }

        if (scene->HasMaterials()) {
            const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            for (unsigned int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); ++j) {
                aiString path;
                if (material->GetTexture(aiTextureType_DIFFUSE, j, &path) == AI_SUCCESS) {
                    QOpenGLTexture* texture = loadTexture(path.C_Str()); // Ваша функция загрузки текстур
                    if (texture) {
                        newMesh->textures.push_back({texture, aiTextureType_DIFFUSE});
                    } else {
                        std::cerr << "Failed to load texture: " << path.C_Str() << std::endl;
                    }
                }
            }
        }
        meshes.push_back(newMesh);
    }
    return meshes;
}


QOpenGLTexture* ObjLoader::loadTexture(const std::string& texturePath) {
    QImage image(QString::fromStdString(texturePath));
    if (image.isNull()) {
        qDebug() << "Failed to load image:" << QString::fromStdString(texturePath);
        return nullptr;
    }

    QOpenGLTexture* texture = new QOpenGLTexture(image);
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}