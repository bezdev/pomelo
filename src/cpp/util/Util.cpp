#include <fstream>

#include "app/App.h"
#include "util/Util.h"

#ifdef BUILD_ANDROID
#include "AndroidApp.h"
#endif

std::vector<char> Util::ReadFile(const char* filename)
{
#ifdef BUILD_ANDROID
    std::vector<char> data;

    AAssetManager* assetManager = AndroidApp::GetInstance()->GetAndroidApp()->activity->assetManager;
    AAsset* assetFile = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);
    if (!assetFile)
    {
        LOGE("file does not exist: %s", filename);
        THROW_FILE_NOT_FOUND();
    }

    char* rawData = (char*)AAsset_getBuffer(assetFile);
    int32_t size = AAsset_getLength(assetFile);
    if (rawData == NULL)
    {
        AAsset_close(assetFile);
        LOGE("error reading file: %s", filename);
    }

    data.reserve(size);
    data.assign(rawData, rawData + size);

    AAsset_close(assetFile);
#endif

#ifdef BUILD_DESKTOP
    std::ifstream ifs(filename, std::ios::binary);
    ifs.seekg(0, std::ios_base::end);
    int size = (int)ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    std::vector<char> data(static_cast<size_t>(size));
    ifs.read(&data[0], size);
    ifs.close();
#endif

    return data;
}

std::vector<std::string> Util::SplitString(std::string string, std::string delimiter)
{
    std::vector<std::string> result;

    size_t pos = 0;
    while ((pos = string.find(delimiter)) != std::string::npos)
    {
        result.push_back(string.substr(0, pos));
        string.erase(0, pos + delimiter.length());
    }

    result.push_back(string.substr(0, pos));

    return result;
}
