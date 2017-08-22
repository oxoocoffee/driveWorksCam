# To build on host create build-host/ folder and change path to it
$ cd {SOME_PATH}/driveWorksCam/build-host/
$ cmake ..
$ make VERBOSE=1

# To cross-compile create build-cross/ folder and change path to it
$ cd {SOME_PATH}/driveWorksCam/build-cross/
$ cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain-linux-driveworks.cmake ..
$ make VERBOSE=1

# To run USB mode

driveWorksCamTest -d usb:0

# To run GMSL mode

driveWorksCamTest -d gmls:camera-type=ar0231-rccb-ssc,csi-port=ab,fifo-size=3,interpolationDemosaic=0,offscreen=1,serializer-type=raw,slave=0,write-file=,output-format=raw+data


# USB call order
main()
    GmslTest::run()
        GmslTest::initializeSDK()
        GmslTest::processUSB()
            GmslTest::cameraReaderHandler()     # if -t switch used
                GmslTest::enterUSBLoop()
                    while(_keepRunning)
                        pullUSB()
                            GmslTest::onImageUpdate()

Ctrl-C  -> GmslTest::terminate()

GmslTest::shutdow()


# GMSL call order
main()
    GmslTest::run()
        GmslTest::initializeSDK()
        GmslTest::processGMSL()
            GmslTest::cameraReaderHandler()     # if -t switch used
                GmslTest::enterGMSLLoop()
                    while(_keepRunning)
                        pullGMSL()
                            GmslTest::onImageUpdate()

Ctrl-C  -> GmslTest::terminate()

GmslTest::shutdow()
