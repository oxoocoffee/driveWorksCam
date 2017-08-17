#include "driveWorksCamTest.h"
#include <algorithm>
#include <sstream>
#include <cstring>

#if DRIVE_WORK_FOUND

GmslTest::GmslTest(void)
    : _keepRunning(true), _enableThreading(false),
      _RCCB(false), _frameCount(0), _imageWidth(0), _imageHeight(0),
      _sdkHandle(DW_NULL_HANDLE), _salHandle(DW_NULL_HANDLE),
      _cameraSensor(DW_NULL_HANDLE), _frameHandle(DW_NULL_HANDLE)
{
}

GmslTest::~GmslTest(void)
{
}

void GmslTest::run(int argc, char* argv[])
{
    _argVec.clear();

    for(int32_t i = 0; i < argc; i++)
        _argVec.push_back( argv[i] );

    if( _argVec.size() == 1 or findArg("-h") )
    {
        printUsage();
        return;
    }

    // Check if running in threaded mode
    if(findArg("-t"))
        _enableThreading = true;

    bool verbose(false);

    // Check if running in vebose mode
    if(findArg("-v"))
        verbose = true;

    string param, camParam;

    if(findArg("-d", param) == false)
        throw std::runtime_error("Invalid or missing -t param");

    splitParam(param, _devType, camParam);

    if(_devType.empty() or camParam.empty())
        throw std::runtime_error("Invalid -t format");

    if(_devType != "usb" and _devType != "gmsl")
        throw std::runtime_error("Invalid device type: usb or gmsl only alloed");

    initializeSDK(verbose);

    if(_devType == "gmsl")
        processGMSL(camParam);
    else if(_devType == "usb")
        processUSB(camParam);
    else
        throw std::runtime_error(string("Unknown camera type: ") + _devType +
                                        ", Reason: Invalid/missing usb or gmsl" );
}

void GmslTest::initializeSDK(bool verbose)
{
    dwLogger_initialize(&GmslTest::nvLogCallback);

    if(verbose)
        dwLogger_setLogLevel(DW_LOG_VERBOSE);
    else
        dwLogger_setLogLevel(DW_LOG_WARN);

    // instantiate Driveworks SDK context
    dwContextParameters sdkParams;

    std::memset(&sdkParams, 0, sizeof(dwContextParameters));

    dwStatus result = dwInitialize(&_sdkHandle, DW_VERSION, &sdkParams);

    if(DW_SUCCESS != result)
        throw std::runtime_error("Cannot initialize DriveWorks SDK, Reason: " +
                                 string(dwGetStatusName(result)));

    result = dwSAL_initialize(&_salHandle, _sdkHandle);

    if (result != DW_SUCCESS)
        throw std::runtime_error("Cannot initialize DriveWorks SAL, Reason: " +
                                 string(dwGetStatusName(result)));
}

void GmslTest::cameraReaderHandler(void)
{
    std::cout << _devType << " thread started" << std::endl;

    if(_devType == "usb")
        enterUSBLoop();
    else if(_devType == "gmsl")
        enterGMSLLoop();

    std::cout << _devType << " thread finishing" << std::endl;
}

void GmslTest::onImageUpdate(uint32_t       width,
                             uint32_t       height,
                             const uint8_t* pBuffer)
{
    NV_UNUSED(pBuffer)

    _frameCount++;

    if((_frameCount % 50) == 0)
    {
        std::cout << _devType + " got: " << width << "x" << height
                  << " " << std::to_string(_frameCount) << " frames" << std::endl << std::flush;
        _frameCount = 0;
    }
}

void GmslTest::shutdown(void)
{
    if(_sdkHandle != nullptr)
    {
        dwStatus result = dwRelease(&_sdkHandle);

        if(DW_SUCCESS != result)
            std::cerr << "Release SDK FAILED. Reason: " << dwGetStatusName(result) << std::endl;
        else
            std::cout << "Release SDK OK" << std::endl;
    }
}

void GmslTest::nvLogCallback(dwContextHandle_t context,
                             dwLoggerVerbosity type,
                             const char *msg)
{
    NV_UNUSED(context);

    switch (type)
    {
        case DW_LOG_VERBOSE:
            std::cout << "DW_LOG_VERBOSE: " << msg;
            break;

        case DW_LOG_DEBUG:
            std::cout << "DW_LOG_DEBUG: " << msg;
            break;

        case DW_LOG_WARN:
            std::cout << "DW_LOG_WARN: " << msg;
            break;

        case DW_LOG_ERROR:
            std::cout << "DW_LOG_ERROR: " << msg;
            break;
    }
}

#endif // DRIVE_WORK_FOUND

