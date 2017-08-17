#include "driveWorksCamTest.h"
#include <algorithm>
#include <sstream>
#include <cstring>

#if DRIVE_WORK_FOUND

void GmslTest::processGMSL(const string& camParam)
{
    NV_UNUSED(camParam)

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
    std::cout << "Entering " << _devType << " image pump" << std::endl;

    while(_keepRunning)
        pullGMSL();

    std::cout << "Entering " << _devType << " image pump" << std::endl;
}

void GmslTest::pullGMSL(void)
{

}

#endif // DRIVE_WORK_FOUND
