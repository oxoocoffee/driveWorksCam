#include "driveWorksCamTest.h"
#include <algorithm>
#include <sstream>
#include <cstring>

#if DRIVE_WORK_FOUND

void GmslTest::processUSB(const string& camParam)
{
    dwSensorParams params;

    string parameters("device=");
           parameters += camParam;

    params.protocol        = "camera.usb";
    params.parameters      = parameters.c_str();

    dwStatus result = dwSAL_createSensor(&_cameraSensor, params, _salHandle);

    if (result != DW_SUCCESS)
        throw std::runtime_error(string("Failed dwSAL_createSensor: ") + camParam +
                                        ", Reason: " + dwGetStatusName(result) );

    dwImageProperties cameraImageProperties;

    std::memset(&cameraImageProperties, 0, sizeof(dwImageProperties));

    result = dwSensorCamera_getImageProperties(&cameraImageProperties, DW_CAMERA_RAW_IMAGE, _cameraSensor);

    if (result != DW_SUCCESS)
        throw std::runtime_error(string("Failed dwSensorCamera_getImageProperties: ") + camParam +
                                        ", Reason: " + dwGetStatusName(result) );

    _imageWidth  = cameraImageProperties.width;
    _imageHeight = cameraImageProperties.height;

    _camImageType = cameraImageProperties.type;

    dwCameraProperties cameraProperties;

    result = dwSensorCamera_getSensorProperties(&cameraProperties, _cameraSensor);

    if (result != DW_SUCCESS)
        throw std::runtime_error(string("Failed dwSensorCamera_getSensorProperties: ") + camParam +
                                        ", Reason: " + dwGetStatusName(result) );

    std::ostringstream buff;

    buff << "USB camera image with " << _imageWidth << "x" << _imageHeight
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
        enterUSBLoop();
}

void GmslTest::enterUSBLoop(void)
{
    std::cout << "Entering " << _devType << " image pump" << std::endl;

    dwStatus result = dwSensor_start(_cameraSensor);

    if (result != DW_SUCCESS)
    {
        std::cerr << "Failed to enable streaming: "
                  << _devType << ", Reason: " << dwGetStatusName(result);
        _keepRunning = false;
        return;
    }

    while(_keepRunning)
        pullUSB();

    std::cout << "Entering " << _devType << " image pump" << std::endl;
}

void GmslTest::pullUSB(void)
{
    const dwStatus result = dwSensorCamera_readFrame(&_frameHandle, 0, 50000, _cameraSensor);

    if (DW_NOT_AVAILABLE == result)
    {
        std::cerr << "Failed to read frame, Reason: " << dwGetStatusName(result);
        return;
    }

    onImageUpdate(_imageWidth, _imageHeight, 0L);
}

#endif // DRIVE_WORK_FOUND
