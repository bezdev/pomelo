#include <fstream>

#include "util.h"
#include "app.h"

std::vector<char> Util::ReadFile(const char* filename)
{
#ifdef BUILD_ANDROID
    std::vector<char> data;

    AAssetManager* assetManager = App::GetInstance()->GetApp()->activity->assetManager;
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
    std::vector<char> data(size);

    ifs.read(&data[0], size);
    ifs.close();
#endif

    return data;
}
