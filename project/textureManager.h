// TextureManager.h

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include "texture.h"
#include <iostream>

using namespace std;

class TextureManager {
public:
    static Texture2D LoadTexture(const char* file, string name);
    static Texture2D GetTexture(string name);
    static void Clear();
    static void SaveImageToDownloads(const string& sourcePath, const string& filename);

private:
    static map<string, Texture2D> Textures;
    TextureManager() {}
};

#endif