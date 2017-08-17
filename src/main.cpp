#include "driveWorksCamTest.h"
#include <csignal>
#include <cstring>

GmslTest gmslTest;

extern "C" {

void    sigHandler(int sig)
{
    NV_UNUSED(sig)

    gmslTest.terminate();
}

} // extern "C"

int main(int argc, char* argv[])
{
    try
    {
        // Ctrl-C
        if( signal(SIGINT, sigHandler) == SIG_ERR )
            throw runtime_error(string("Failed register SIGINT. ") +
                                ", Reason: " + std::strerror(errno));

        gmslTest.run(argc, argv);
    }
    catch(std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
    }

    gmslTest.shutdown();

    return 0;
}
