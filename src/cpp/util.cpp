#include <fstream>

#include "util.h"
#include "app.h"

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
    std::vector<char> data(size);

    ifs.read(&data[0], size);
    ifs.close();
#endif

    return data;
}

std::vector<std::string> Util::StringSplit(std::string string, std::string delimiter)
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

std::vector<std::string> Util::StringSplit(const char* string, const char* delimiter)
{
    std::vector<std::string> result;

    const int bufferSize = 1000;
    char currentSplitBuffer[bufferSize];
    int currentSplitLength = 0;
    bool isInDelimiter = false;
    int delimiterIndex = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (i >= bufferSize) throw;

        char currentChar = string[i];
        currentSplitBuffer[currentSplitLength++] = currentChar;


        if (!isInDelimiter)
        {
            if (currentChar == delimiter[0])
            {
                delimiterIndex = 0;
                isInDelimiter = true;
            }
        }

        if (isInDelimiter)
        {
            if (currentChar != delimiter[delimiterIndex])
            {
                isInDelimiter = false;

                continue;
            }

            if (delimiter[delimiterIndex + 1] == '\0')
            {
                isInDelimiter = false;

                int actualSplitLength = currentSplitLength - (delimiterIndex + 1);
                if (actualSplitLength > 0)
                {
                    result.push_back(std::string(currentSplitBuffer, actualSplitLength));
                }
                currentSplitLength = 0;

                continue;
            }

            delimiterIndex++;
        }

        if (string[i + 1] == '\0')
        {
            int actualSplitLength = currentSplitLength;
            if (actualSplitLength > 0)
            {
                result.push_back(std::string(currentSplitBuffer, actualSplitLength));
            }
        }
    }

    return result;
}
