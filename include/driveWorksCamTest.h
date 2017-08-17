#ifndef GMSL_TEST_H
#define GMSL_TEST_H

#include <iostream>
#include <vector>
#include <thread>
#include <system_error>
#include "buildVersion.h"

using namespace std;

#if DRIVE_WORK_FOUND

#include <dw/renderer/Renderer.h>       // dwRendererHandle_t
#include <dw/sensors/Sensors.h>         // dwSALHandle_t
#include <dw/sensors/camera/Camera.h>   // dwCameraFrameHandle_t
#include <dw/core/Logger.h>

class GmslTest
{
    public:
        typedef vector<string>   TStringVec;
        typedef TStringVec       TArgVec;

                 GmslTest(void);
        virtual ~GmslTest(void);

        void    run(int argc, char* argv[]);
        void    shutdown(void);

        inline void terminate(void) {
            _keepRunning = false;
        }


    private:
        void    initializeSDK(bool verbose);
        void    processUSB(const string& camParam);
        void    processGMSL(const string& camParam);

        void    cameraReaderHandler(void);
        void    enterUSBLoop(void);
        void    enterGMSLLoop(void);

        void    pullUSB(void);
        void    pullGMSL(void);

        void    onImageUpdate(uint32_t width,
                              uint32_t height,
                              char*    pBuffer);

        static void  nvLogCallback(dwContextHandle_t context,
                                   dwLoggerVerbosity type,
                                   const char *msg);

        // Extra
        bool    findArg(const string& param ) const;
        bool    findArg(const string& param, string& paramVal) const;
        bool    findArg(const string& param, int32_t& paramVal) const;

        void    splitParam(const string& param,
                                 string& devType,
                                 string& camParam);

        TStringVec& split(TStringVec&   vec,
                          const string& param,
                          const char    delim);

        void    printUsage(void);

    private:
        bool                    _keepRunning;
        bool                    _enableThreading;
        uint32_t                _frameCount;
        uint32_t                _imageWidth;
        uint32_t                _imageHeight;
        string                  _devType;
        TArgVec                 _argVec;
        std::thread             _threadId;
        dwContextHandle_t       _sdkHandle;
        dwSALHandle_t           _salHandle;
        dwSensorHandle_t        _cameraSensor;
        dwCameraFrameHandle_t   _frameHandle;
        dwImageType             _camImageType;
};

#define NV_UNUSED(x) (void)x;

#endif // DRIVE_WORK_FOUND

#endif // GMSL_TEST_H
