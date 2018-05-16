#include <CRPCRawLogAbstract.h>
#include <mrpt/web/CWebSocketJsonRpcServer.h>
#include <mrpt/web/CWebSocket.h>

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
int main(int argc, char* argv[])
{
    try
    {
        // Check the commmand line arguments
        if(argc != 3)
        {
            std::cerr <<
                "Usage: web <address> <port>\n" <<
                "Example:\n" <<
                "   web 0.0.0.0 8080\n";
                return EXIT_FAILURE;
        }
        // auto const address = boost::asio::ip::make_address(argv[1]);
        auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
        CWebSocketServer ws_server(argv[1], port);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}