#include "mrpt/web/CWebSocketUtility.hpp"

#include <CRPCRawLogAbstract.h>
#include <mrpt/web/CWebSocketJsonRpcServer.h>
#include <mrpt/web/CWebSocket.h>
#include <mrpt/web/CWebSocketAdvanced.h>

#include <cstdlib>
#include <functional>
#include <thread>
#include <string>

class CRPCRawLog : public CRPCRawLogAbstract
{
public:
  Json::Value Playlist_GetPlaylists() override
  {

  }
  Json::Value Playlist_GetItems() override
  {

  }
};
int main()
{
  return 0;
}