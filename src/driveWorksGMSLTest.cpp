#include "driveWorksCamTest.h"
#include <algorithm>
#include <sstream>
#include <cstring>

#if DRIVE_WORK_FOUND

void GmslTest::processGMSL(const string& camParam)
{
    dwSensorParams params;

    params.protocol        = "camera.gmsl";
    params.parameters      = camParam.c_str();

    dwStatus result = dwSAL_createSensor(&_cameraSensor, params, _salHandle);

    if (result != DW_SUCCESS)
        throw std::runtime_error(string("Failed dwSAL_createSensor: camera.gmsl, Reason: ")
                                 + dwGetStatusName(result) );

    dwImageProperties cameraImageProperties;

    std::memset(&cameraImageProperties, 0, sizeof(dwImageProperties));

    result = dwSensorCamera_getImageProperties(&cameraImageProperties, DW_CAMERA_RAW_IMAGE, _cameraSensor);

    if (result != DW_SUCCESS)
        throw std::runtime_error(string("Failed dwSensorCamera_getImageProperties: camera.gmsl, Reason: ")
                                        + dwGetStatusName(result) );

    _imageWidth  = cameraImageProperties.width;
    _imageHeight = cameraImageProperties.height;

    _camImageType = cameraImageProperties.type;

    dwCameraProperties cameraProperties;

    result = dwSensorCamera_getSensorProperties(&cameraProperties, _cameraSensor);

    if (result != DW_SUCCESS)
        throw std::runtime_error(string("Failed dwSensorCamera_getSensorProperties: camera.gmsl, Reason: ")
                                 + dwGetStatusName(result) );

    if (cameraImageProperties.pxlFormat == DW_IMAGE_RCCB || cameraImageProperties.pxlFormat == DW_IMAGE_CRBC ||
        cameraImageProperties.pxlFormat == DW_IMAGE_CBRC || cameraImageProperties.pxlFormat == DW_IMAGE_BCCR)
        _RCCB = true;

#ifdef VIBRANTE
    if(cameraImageProperties.type != DW_IMAGE_NVMEDIA)
        throw std::runtime_error(string("Expected nvmedia image type. camera.gmsl, Reason: ")
                                 + dwGetStatusName(result) );
#endif // VIBRANTE

    std::ostringstream buff;

    buff << "GMSL camera image with " << _imageWidth << "x" << _imageHeight
         << " at " << cameraProperties.framerate << " FPS";

    std::cout << buff.str() << std::endl;

    if(_enableThreading)
    {
        try
        {
            _threadId = std::thread(&GmslTest::cameraReaderHandler, this);

            while(_keepRunning)
                std::this_thread::sleep_for(std::chrono::seconds(1));

            if(_threadId.joinable())
                _threadId.join();
        }
        catch(const std::system_error& ex) {
            throw runtime_error(string("Failed to spawn threadHandler, Reason: ") + ex.what() );
        }
    }
    else
        enterGMSLLoop();
}

void GmslTest::enterGMSLLoop(void)
{
    std::cout << "Entering camera.gmsl image pump" << std::endl;


    dwStatus result = dwSensor_start(_cameraSensor);

    if (result != DW_SUCCESS)
    {
        std::cerr << "Failed to enable streaming: camera.gmsl, Reason: "
                  << dwGetStatusName(result);
        _keepRunning = false;
        return;
    }

    while(_keepRunning)
        pullGMSL();

    std::cout << "Entering camera.gmsl image pump" << std::endl;
}

void GmslTest::pullGMSL(void)
{
    dwStatus result = dwSensorCamera_readFrame(&_frameHandle, 0, 1000000, _cameraSensor);

    if (DW_NOT_AVAILABLE == result)
    {
        std::cerr << "Failed to read camera.gmsl frame, Reason: " << dwGetStatusName(result);
        return;
    }

    const dwCameraDataLines* dataLines = nullptr;

    result = dwSensorCamera_getDataLines(&dataLines, _frameHandle);

    if( result == DW_SUCCESS )
    {
        if( dataLines->topLines > 0 ) {
            std::cout << "First 16 bytes from top data lines:\n";
            std::cout << std::hex;
            for(uint32_t i = 0; i < 16; ++i)
                std::cout << static_cast<int>(dataLines->topLineData[i]) << " ";
            std::cout << std::dec;
            std::cout << "\n\n";
        }

        if( dataLines->bottomLines > 0 ) {
            std::cout << "First 16 bytes from bottom data lines:\n";
            std::cout << std::hex;
            for(uint32_t i = 0; i < 16; ++i)
                std::cout << static_cast<int>(dataLines->bottomLineData[i]) << " ";
            std::cout << std::dec;
            std::cout << "\n\n";
        }
    }

    onImageUpdate(_imageWidth, _imageHeight, 0L);

    std::this_thread::yield();
}

#endif // DRIVE_WORK_FOUND
