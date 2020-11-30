#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

#include "stb_image.h"
#include "Shader.h"
#include "texture.h"

class ResourceManager {
private:
    ResourceManager();

    static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile);
    static Texture loadTextureFromFile(const GLchar* file, GLboolean alpha);

public:
    static std::map<std::string, Texture> textures;
    static std::map<std::string, Shader> shaders;

    static Shader loadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
    static Shader getShader(std::string name);

    static Texture loadTexture(const GLchar* file, GLboolean alpha, std::string name);
    static Texture getTexture(std::string name);

    static void clear();

};
