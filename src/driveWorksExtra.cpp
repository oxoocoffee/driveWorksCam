#include "driveWorksCamTest.h"
#include <algorithm>
#include <sstream>
#include <cstring>

#if DRIVE_WORK_FOUND

void GmslTest::splitParam(const string& param,
                          string&       devType,
                          string&       camParam)
{
    TStringVec vec;

    split(vec, param, ':');

    if( vec.size() != 2)
        throw std::runtime_error(string("Invalid devType:param"));

    devType  = vec[0];
    camParam = vec[1];
}

GmslTest::TStringVec& GmslTest::split(TStringVec&   vec,
                                      const string& param,
                                      const char    delim)
{
    std::stringstream ss;
    ss.str(param);
    std::string item;

    vec.clear();

    while (std::getline(ss, item, delim))
        if( ! item.empty() )
            vec.push_back(item);

    return vec;
}

bool GmslTest::findArg(const string& param ) const
{
    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter != _argVec.end() )
        return true;

    return false;
}

bool GmslTest::findArg(const string& param, string& paramVal) const
{
    paramVal.clear();

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter == _argVec.end() )
        return false;

    ++iter;

    // Check if there is another item in list (paramVal)
    if( iter != _argVec.end() )
    {
        if( iter->empty() == false)
        {
            if( (*iter)[0] != '-' )
            {
                paramVal = *iter;

                if(paramVal.empty())
                    throw std::runtime_error("Failed to parse " + param +
                                             " parameter: empty");
            }
        }
    }

    return true;
}

bool GmslTest::findArg(const string& param, int32_t& paramVal) const
{
    paramVal = 0;

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter == _argVec.end() )
        return false;

    ++iter;

    // Check if there is another item in list (paramVal)
    if( iter != _argVec.end() )
    {
        if( iter->empty() == false )
        {
            if( (*iter)[0] != '-' )
            {
                istringstream  a2i(*iter);

                if( ! (a2i >> paramVal) )
                    throw std::runtime_error("Failed to parse " + param +
                                             " parameter (int): " +
                                             *iter);
            }
        }
    }

    return true;
}

void GmslTest::printUsage(void)
{
    std::cout << "Usage: " << _argVec[0] << " -d {usb|gmsl}:{param} -t" << std::endl
              << "   -d  : device info"      << std::endl
              << "   -t  : enable threading" << std::endl
              << "   -v  : verbose on"       << std::endl
              << "Examples"                  << std::endl
              << "   -d usb:0"               << std::endl
              << "   -d gmls:camera-type=ar0231-rccb,csi-port=ab,camera-count=1,camera-mask=0001,output-format=yuv\"" << std::endl
              << "Future support"            << std::endl
              << "   -f {rgb|yuv} enable output" << std::endl;
}

#endif // DRIVE_WORK_FOUND
