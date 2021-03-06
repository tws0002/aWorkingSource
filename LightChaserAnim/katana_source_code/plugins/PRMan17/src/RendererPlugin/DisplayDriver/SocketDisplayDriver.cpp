// *******************************************************************
// This file contains copyrighted work from The Foundry,
// Sony Pictures Imageworks and Pixar, is intended for
// Katana and PRMan customers, and is not for distribution
// outside the terms of the corresponding EULA(s).
// *******************************************************************

#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <map>

#include <ndspy.h> // prman display driver header.

#include <FnDisplayDriver/FnKatanaDisplayDriver.h>
namespace FnKat = Foundry::Katana;

//#define DRIVER_DEBUG 1

#ifdef DRIVER_DEBUG
  #include <iostream>
#endif


#ifdef _WIN32
#define DisplayDriverAPI __declspec(dllexport)
#else
#define DisplayDriverAPI
#endif

/* This is the user structure that prman will hang onto
   per instance of this display driver.
*/
struct UserImageStruct
{
    FnKat::KatanaPipe *pipe;
    FnKat::NewChannelMessage *channel;
    int numChannels;
};

/*******************************************************************************
 *                      RENDERMAN DISPLAY DRIVER HELPER
 * This is a helper class that helps us manage Frame & Channels and a couple of
 * utility functions.
 ******************************************************************************/
class PRManKatanaDisplayDriverHelper
{
public:
  static PtDspyError parseHostName(const char* hostnameString, char* hostName, unsigned long* portNumber);

  static PRManKatanaDisplayDriverHelper* Instance();
  static void Destroy();

  // Do we already have a NewFrameMessage object for the frameID?
  bool haveObjectForFrame(int frameID);
  // Add an NewFrame object for the given frame ID
  void addNewFrameObject(int frameID, FnKat::NewFrameMessage* frame);
  // Get the NewFrame object for a given frameID
  FnKat::NewFrameMessage* getFrameObject(int frameID);

  // Get a channelID for the frame...
  int getNextChannelID(FnKat::NewFrameMessage* frame);

private:
  PRManKatanaDisplayDriverHelper() : _frameMap(), _channelMap() {};
  PRManKatanaDisplayDriverHelper(const PRManKatanaDisplayDriverHelper& rhs);
  ~PRManKatanaDisplayDriverHelper() {};

  static PRManKatanaDisplayDriverHelper* _instance;

  std::map<int, FnKat::NewFrameMessage* > _frameMap;
  std::map<FnKat::NewFrameMessage*, int > _channelMap;
};

PRManKatanaDisplayDriverHelper* PRManKatanaDisplayDriverHelper::_instance = NULL;

PRManKatanaDisplayDriverHelper* PRManKatanaDisplayDriverHelper::Instance()
{
  if(!_instance)
    _instance = new PRManKatanaDisplayDriverHelper();

  return _instance;
}

void PRManKatanaDisplayDriverHelper::Destroy()
{
  if(!_instance)
  {
    delete _instance;
    _instance = NULL;
  }
}

PtDspyError PRManKatanaDisplayDriverHelper::parseHostName(const char* hostnameString, char* hostName, unsigned long* portNumber)
{
  // Skip any leading _'s
  while (*hostnameString == '_') ++hostnameString;

  // Find the : after the hostname.
  const char* p = hostnameString;
  while ((p - hostnameString < 4096) && *p && ':' != *p)
  {
      ++p;
  }

  if (*p != ':')
  {
      return PkDspyErrorBadParams;
  }

  // Copy out the hostname, null terminate it
  memcpy(hostName, hostnameString, p - hostnameString);
  hostName[p - hostnameString] = '\0';

  // Get the port number as a number.
  char* dummy;
  *portNumber = strtoul(p+1, &dummy, 0);

  *portNumber += 100;

  return PkDspyErrorNone;
}

bool PRManKatanaDisplayDriverHelper::haveObjectForFrame(int frameID)
{
  std::map<int, FnKat::NewFrameMessage* >::iterator it;

  it = _frameMap.find(frameID);

  return (it != _frameMap.end());
}

void PRManKatanaDisplayDriverHelper::addNewFrameObject(int frameID, FnKat::NewFrameMessage* frame)
{
  // Add the frame to the _frameMap...
  _frameMap[frameID] = frame;

  // Add the frame to the _channelMap.
  _channelMap[frame] = 0;
}

FnKat::NewFrameMessage* PRManKatanaDisplayDriverHelper::getFrameObject(int frameID)
{
  return _frameMap[frameID];
}

int PRManKatanaDisplayDriverHelper::getNextChannelID(FnKat::NewFrameMessage *frame)
{
  // First find the std::set for the frame...
  std::map<FnKat::NewFrameMessage*, int>::iterator it = _channelMap.find(frame);

  // Return a new channel ID
  return ++(it->second);
}

/*******************************************************************************
 *                      END RENDERMAN DISPLAY DRIVER HELPER
 ******************************************************************************/

/**************************************
 * EXTERNAL ENTRY POINT DspyImageOpen *
 **************************************/
extern "C" DisplayDriverAPI PtDspyError DspyImageOpen
(
    PtDspyImageHandle* handle_p,
    const char* drivername,
    const char* filename,
    int width, int height,
    int paramCount,
    const UserParameter* parameters,
    int formatCount,
    PtDspyDevFormat* format,
    PtFlagStuff* flagstuff
)
{
#ifdef DRIVER_DEBUG
    std::cerr << "DspyImageOpen(): "
              << "handle_p = " << handle_p << ", "
              << "drivername = \"" << drivername << "\", "
              << "filename = \"" << filename << "\", "
              << "width = " << width << ", "
              << "height = " << height << ", "
              << "paramCount = " << paramCount << ", "
              << "formatCount = " << formatCount << std::endl;

    for (int formatIndex = 0; formatIndex < formatCount; ++formatIndex)
    {
        std::cerr << "    Format " << formatIndex << ": "
                  << "name = \"" << format[formatIndex].name << "\", "
                  << "type = " << format[formatIndex].type
                  << std::endl;
    }
#endif

    // Format Renderman parameters in the order we expect to receive them.
    if ((0 == width)||(0 == height)||(0 == formatCount))
        return PkDspyErrorBadParams;

    // Ask for everything in float
    for (int formatIndex = 0; formatIndex < formatCount; ++formatIndex)
        format[formatIndex].type = PkDspyFloat32;
    flagstuff->flags |= PkDspyFlagsWantsEmptyBuckets;

    // Set up the Image structure that will be passed in each time we get data
    UserImageStruct* imgStruct = new UserImageStruct;
    *handle_p = imgStruct;

    PtDspyError result = PkDspyErrorNone;

    // Connect the KatanaPipe to our server...
    char hostName[4096];
    unsigned long portNumber;
    result = PkDspyErrorNone;
    if(PkDspyErrorNone != (result = PRManKatanaDisplayDriverHelper::parseHostName(filename, hostName, &portNumber)))
      return result;
    imgStruct->pipe = FnKat::PipeSingleton::Instance(hostName, portNumber);

    if(imgStruct->pipe->connect())
      return PkDspyErrorUndefined;

    // Get the frameID out of the parameter list.
    int frameID = 0;
    DspyFindIntInParamList("frameID", &frameID, paramCount, parameters);

    imgStruct->numChannels = formatCount;

    // First thing we need to do is check if we have a unique frame ID for
    // this render (remember a frame is made up of many channels)
    FnKat::NewFrameMessage *frameObject = NULL;
    if(PRManKatanaDisplayDriverHelper::Instance()->haveObjectForFrame(frameID))
    {
      frameObject = PRManKatanaDisplayDriverHelper::Instance()->getFrameObject(frameID);
    }
    else
    {
      // We are going to build one...
      frameObject = new FnKat::NewFrameMessage();

      // Frame Time...
      float frameTime = 0;
      DspyFindFloatInParamList("frameTime", &frameTime, paramCount, parameters);
      frameObject->setFrameTime(frameTime);

      // Frame dimensions
      int nvals = 2;
      int originalDimensions[2];
      originalDimensions[0] = 0;
      originalDimensions[1] = 0;
      DspyFindIntsInParamList("OriginalSize", &nvals, originalDimensions, paramCount, parameters);
      frameObject->setFrameDimensions(originalDimensions[0], originalDimensions[1]);

      // Origin is (0,0) - not sure how much we need this?
      frameObject->setFrameOrigin(0, 0);

      // Frame Name...
      char* nameStr = NULL;
      DspyFindStringInParamList("frameName", &nameStr, paramCount, parameters);

      // Version 1.0 of the RDP requires the frame name field is encoded using
      // the following format...
      std::string legacyEncoding;
      if(nameStr)
      {
        FnKat::encodeLegacyName(nameStr, frameID, legacyEncoding);
        frameObject->setFrameName(legacyEncoding);
      }

      // Store the frame in the map...
      PRManKatanaDisplayDriverHelper::Instance()->addNewFrameObject(frameID, frameObject);

      // Send the frame down the pipe...
      imgStruct->pipe->send(*frameObject);
    }

    // Now we need to create a channel that will hold the data...
    int channelID = PRManKatanaDisplayDriverHelper::Instance()->getNextChannelID(frameObject);
    FnKat::NewChannelMessage *channelObject = new FnKat::NewChannelMessage(*frameObject);

    // Set channelID
    channelObject->setChannelID((FnKat::uint16_t)channelID);

    // Origin of pixels relative to frame.
    int nvals = 2;
    int origin[2];
    origin[0] = 0;
    origin[1] = 0;
    DspyFindIntsInParamList("origin", &nvals, origin, paramCount, parameters);

    channelObject->setChannelOrigin(origin[0], origin[1]);

    // Set the height and width
    channelObject->setChannelDimensions(channelObject->channelXOrigin() + width,
                                        frameObject->frameHeight() - channelObject->channelYOrigin());

    // Readjust the origin for renderman...
    channelObject->setChannelOrigin(channelObject->channelXOrigin(),
                                    channelObject->channelHeight() - height);

    // Set sample rate.
    nvals = 2;
    float sampleRate[2];
    sampleRate[0] = 1.0;
    sampleRate[1] = 1.0;
    DspyFindFloatsInParamList("sampleRate", &nvals, sampleRate, paramCount, parameters);
    channelObject->setSampleRate(sampleRate);

    // Data Size... everything is 32 bit float, so multiply by num values
    channelObject->setDataSize(4 * formatCount);


    // Channel Name...
    char* nameStr = NULL;
    DspyFindStringInParamList("frameName", &nameStr, paramCount, parameters);
    // Version 1.0 of the RDP requires the channel name field is encoded using
    // the following format...
    std::string legacyEncoding;
    if(nameStr)
    {
      FnKat::encodeLegacyName(nameStr, frameID, legacyEncoding);
      channelObject->setChannelName(legacyEncoding);
    }

    // Pack the channel down the pipe to and save to our image structure.
    imgStruct->pipe->send(*channelObject);
    imgStruct->channel = channelObject;

    return PkDspyErrorNone;
}


/**************************************
 * EXTERNAL ENTRY POINT DspyImageData *
 **************************************/
extern "C" DisplayDriverAPI PtDspyError DspyImageData(
    PtDspyImageHandle handle,
    int xmin,
    int xmax_plusone,
    int ymin,
    int ymax_plusone,
    int entrysize,
    const unsigned char *data
) 
{
#ifdef DRIVER_DEBUG
    std::cerr << "DspyImageData(): "
              << "handle = " << handle << ", "
              << "xmin = " << xmin << ", "
              << "xmax_plusone = " << xmax_plusone << ", "
              << "ymin = " << ymin << ", "
              << "ymax_plusone = " << ymax_plusone << ", "
              << "entrysize = " << entrysize
              << std::endl;
#endif

    UserImageStruct* imgStruct = reinterpret_cast<UserImageStruct*>(handle);

    FnKat::DataMessage *dataMsg = new FnKat::DataMessage(*(imgStruct->channel));
    dataMsg->setStartCoordinates(xmin, ymin);
    dataMsg->setDataDimensions((xmax_plusone - xmin), (ymax_plusone - ymin));

    int blockW = xmax_plusone - xmin;
    int blockH = ymax_plusone - ymin;
    size_t blockSize = static_cast<size_t>(blockW * blockH * entrysize);

    char *dataArray = new char[blockSize];
    char *array = dataArray;
    memset(dataArray, 0, blockSize);

    // We're running the data pointer through a for loop as the old method of
    // copying the array would bottom out when large numbers of AOVs were output
    // (believed to be due to memory alignment) issues.
    int oldx;
    oldx = xmin;
    for (;ymin < ymax_plusone; ymin++)
    {
       for (xmin = oldx; xmin < xmax_plusone; xmin++)
       {
         memcpy(array, data, imgStruct->numChannels * sizeof(float));
         array += entrysize;
         data += entrysize;
       }
    }

    dataMsg->setData(dataArray, (dataMsg->width() * dataMsg->height() * entrysize) );
    dataMsg->setByteSkip(entrysize);

    // Pack it down the pipe.
    imgStruct->pipe->send(*dataMsg);

    // Clean up the data array we used to do the copy...
    delete [] dataArray;
    dataArray = NULL;

    delete dataMsg;
    dataMsg = NULL;

    return PkDspyErrorNone;
}


/***************************************
 * EXTERNAL ENTRY POINT DspyImageClose *
 ***************************************/
extern "C" DisplayDriverAPI PtDspyError DspyImageClose
(
    PtDspyImageHandle handle
)
{
#ifdef DRIVER_DEBUG
    std::cerr << "DspyImageClose(): "
              << "handle = " << handle
              << std::endl;
#endif

    // Close the socket and delete the user structure.
    if (0 != handle)
    {
        UserImageStruct* imgStruct = reinterpret_cast<UserImageStruct*>(handle);

        // We are done sending data to this channel so let Katana clean up...
        imgStruct->pipe->flushPipe(*imgStruct->channel);
        imgStruct->pipe->closeChannel(*imgStruct->channel);

        delete imgStruct;
    }

    return PkDspyErrorNone;
}


/***************************************
 * EXTERNAL ENTRY POINT DspyImageQuery *
 ***************************************/
 
/* My understanding is that this is an archaic call,
   not really useful in a modern context.
   It has to be here, but it doesn't really need to do
   anything useful.
*/
extern "C" DisplayDriverAPI PtDspyError DspyImageQuery
(
    PtDspyImageHandle handle,
    PtDspyQueryType querytype,
    int datalen,
    void* data
) 
{
#ifdef DRIVER_DEBUG
    std::cerr << "DspyImageQuery(): "
              << "handle = " << handle << ", "
              << "querytype = " << querytype << ", "
              << "datalen = " << datalen
              << std::endl;
#endif

    if (0 >= datalen || 0 == data)
        return PkDspyErrorBadParams;

    switch (querytype)
    {
        case PkOverwriteQuery:
        {
            PtDspyOverwriteInfo overwriteInfo;
            if (size_t(datalen) > sizeof(overwriteInfo))
                datalen = sizeof(overwriteInfo);
            overwriteInfo.overwrite = 1;
            overwriteInfo.interactive = 0;
            memcpy(data, &overwriteInfo, datalen);
            return PkDspyErrorNone;
        }
        
        case PkSizeQuery:
        {
            PtDspySizeInfo sizeInfo;

            if (size_t(datalen) > sizeof(sizeInfo))
                datalen = sizeof(sizeInfo);

            sizeInfo.width = 0;
            sizeInfo.height = 0;
            sizeInfo.aspectRatio = 1.0f;

            memcpy(data, &sizeInfo, datalen);
            return PkDspyErrorNone;
        }

        default:
        {
            return PkDspyErrorUnsupported;
        }
    }
}
