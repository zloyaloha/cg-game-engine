#include "shape.h"
#include "iostream"

Shape::Shape(const glm::vec3& shapePosition, const glm::vec3& shapeColor, const std::string& shapeType) : 
    vertexBuffer(QOpenGLBuffer::VertexBuffer), color(shapeColor), position(shapePosition), type(shapeType) {}

void Shape::setViewMatrix(const glm::mat4 &viewMatrix) 
{
    _viewMatrix = viewMatrix;
}

void Shape::setShader(std::shared_ptr<QOpenGLShaderProgram> shader)
{
    shaderProgram = shader;
}

void Shape::setColor(const glm::vec3 &shapeColor)
{
    color = shapeColor;
}

void Shape::loadMatriciesToShader()
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    shaderProgram->setUniformValue("model", QMatrix4x4(glm::value_ptr(modelMatrix)).transposed());
    shaderProgram->setUniformValue("view", QMatrix4x4(glm::value_ptr(_viewMatrix)).transposed());
    shaderProgram->setUniformValue("projection", QMatrix4x4(glm::value_ptr(_projectionMatrix)).transposed());
}

void Shape::loadLightsToShader()
{
    shaderProgram->setUniformValue("numLights", int(lights.size()));
    for (int i = 0; i < lights.size(); ++i) {
        shaderProgram->setUniformValue(("lightPos[" + std::to_string(i) + "]").c_str(),
                                        QVector3D(lights[i]->position.x, lights[i]->position.y, lights[i]->position.z));
        shaderProgram->setUniformValue(("lightColor[" + std::to_string(i) + "]").c_str(),
                                        QVector3D(lights[i]->color.x, lights[i]->color.y, lights[i]->color.z));
        shaderProgram->setUniformValue(("lightPointTo[" + std::to_string(i) + "]").c_str(),
                                        QVector3D(lights[i]->pointTo.x, lights[i]->pointTo.y, lights[i]->pointTo.z));
    }
}

void Shape::loadObjectLightToShader()
{
    shaderProgram->setUniformValue("objectColor", QVector3D(color.r, color.g, color.b));
}

void Shape::setLights(const std::vector<std::shared_ptr<Light>> &shapeLights)
{
    lights = shapeLights;
}

std::string Shape::getType() const
{
    return type;
}

void Shape::setModelMatrix(const glm::mat4 &modelMatrix) 
{
    _modelMatrix = modelMatrix;
}

void Shape::setProjectionMatrix(const glm::mat4 &projectionMatrix) 
{
    _projectionMatrix = projectionMatrix;
}

bool ObjLoader::load(const std::string& filepath, 
              std::vector<glm::vec3>& vertices,
              std::vector<glm::vec3>& normals,
              std::vector<glm::vec2>& texCoords,
              std::vector<unsigned int>& indices,
              std::vector<Texture>& textures) 
{
     Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath, 
        aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene) {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0]; // Assuming single mesh

    // Load vertices, normals, and texture coordinates
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        const aiVector3D& aiVertex = mesh->mVertices[i];
        vertices.emplace_back(aiVertex.x, aiVertex.y, aiVertex.z);

        if (mesh->HasNormals()) {
            const aiVector3D& aiNormal = mesh->mNormals[i];
            normals.emplace_back(aiNormal.x, aiNormal.y, aiNormal.z);
        }

        if (mesh->HasTextureCoords(0)) {
            const aiVector3D& aiTexCoord = mesh->mTextureCoords[0][i];
            texCoords.emplace_back(aiTexCoord.x, aiTexCoord.y);
        } else {
            texCoords.emplace_back(0.0f, 0.0f); // Default UV
        }
    }

    // Load indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Load materials and textures
    if (scene->HasMaterials()) {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, i, &path) == AI_SUCCESS) {
                QOpenGLTexture* texture = loadTexture(path.C_Str());
                if (texture) {
                    textures.push_back({texture, aiTextureType_DIFFUSE});
                } else {
                    std::cerr << "Failed to load texture: " << path.C_Str() << std::endl;
                }
            }
        }
    }

    return true;
}

GLuint ObjLoader::createTexture(const QImage& image) {
    GLuint textureID;
    glGenTextures(1, &textureID); // Генерация ID текстуры
    glBindTexture(GL_TEXTURE_2D, textureID); // Привязка текстуры

    // Загрузка данных изображения в текстуру
    QImage formattedImage = image.convertToFormat(QImage::Format_RGBA8888); // Убедимся, что формат корректный
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, formattedImage.width(), formattedImage.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedImage.bits());

    // Установка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Обёртка по S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Обёртка по T
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Минификация
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Увеличение

    // Генерация MipMap для улучшения производительности
    // glGenerateMipmap(GL_TEXTURE_2D);

    // Отвязка текстуры
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

QOpenGLTexture* ObjLoader::loadTexture(const std::string& texturePath) {
    QImage image(QString::fromStdString(texturePath));
    if (image.isNull()) {
        qDebug() << "Failed to load image:" << QString::fromStdString(texturePath);
        return nullptr;
    }

    QOpenGLTexture* texture = new QOpenGLTexture(image.mirrored());
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}
