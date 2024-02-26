
// textureManager.cpp
#include "textureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/include/stb_image/stb_image.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <string>

map<string, Texture2D> TextureManager::Textures;

Texture2D TextureManager::LoadTexture(const char* file, string name) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data) {
        Textures[name].Initialize(width, height, data);
        stbi_image_free(data);
    }
    else {
        cerr << "Texture failed to load at path: " << file << endl;
    }

    return Textures[name];
}

Texture2D TextureManager::GetTexture(string name) {
    return Textures.at(name);
}

void TextureManager::Clear() {
    for (auto& iter : Textures) {
        GLuint textureID = iter.second.getTextureID();
        glDeleteTextures(1, &textureID);
    }
    Textures.clear();
}

void TextureManager::SaveImageToDownloads(const string& sourcePath, const string& filename) {
    PWSTR path = NULL;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path);

    if (SUCCEEDED(hr)) {
        wstring ws(path);
        string downloadsFolder(ws.begin(), ws.end());
        downloadsFolder += "\\" + filename;

        ifstream src(sourcePath, ios::binary);
        ofstream dst(downloadsFolder, ios::binary);

        if (!src || !dst) {
            cerr << "Error opening files!" << endl;
        }
        else {
            dst << src.rdbuf();
        }
    }

    CoTaskMemFree(path);
}