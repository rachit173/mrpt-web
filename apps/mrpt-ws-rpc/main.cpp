#include <jsonrpccpp/server/abstractserverconnector.h>
#include <mrpt/web/CWebSocketUtility.hpp>
#include <mrpt/web/CWebSocketJsonRpcServer.h>
#include <mrpt/web/CModularServer.h>

#include <CRPCRawLogFace.h>

#include <cstdlib>
#include <functional>
#include <thread>
#include <string>
#include <mutex>
#include <memory>

class CRPCRawLog : public CRPCRawLogAbstract
{
public:
  // CRPCRawLog(jsonrpc::AbstractServerConnector &w):CRPCRawLogAbstract(w){}
  virtual RPCModules implementedModules() const override
  {
    return RPCModules{RPCModule{"CRPCRawLog","1.0"}};
  }
  Json::Value Playlist_GetPlaylists() override
  {
    Json::Value ch;
    ch[0]["name"] = "Jabberwock";
    ch[0]["chapter"] = 1;
    ch[1]["name"] = "Cheshire Cat";
    ch[1]["chapter"] = 6;
    ch[2]["name"] = "Mad Hatter";
    ch[2]["chapter"] = 7;

    // create the main object
    Json::Value val;
    val["book"] = "Alice in Wonderland";
    val["year"] = 1865;
    val["characters"] = ch;
  return ch;
  }
  Json::Value Playlist_GetItems() override
  {
    Json::Value ch;
    return ch;
  }
};
class Waste: public jsonrpc::AbstractServerConnector
{
  public:
  bool StartListening() override {}
  bool StopListening() override {}
  bool SendResponse(const std::string& response, void* addInfo = NULL) override {}

};
int main(int argc,char* argv[])
{
  //Check the command line arguments,
  if (argc != 5)
  {
      std::cerr <<
          "Usage: mrpt-ws-rpc <address> <port> <doc_root> <threads>\n" <<
          "Example:\n" <<
          "    mrpt-ws-rpc 127.0.0.1 8080 . 1\n";
      return EXIT_FAILURE;
  }
  auto const address = boost::asio::ip::make_address(argv[1]);
  auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
  auto const doc_root = std::make_shared<std::string>(argv[3]);
  auto const threads = std::max<int>(1, std::atoi(argv[4]));
  try{
    std::unique_ptr<CModularServer<>> jsonrpcIpcServer;
    // Waste w;
    using FullServer = CModularServer<
      CRPCRawLog   
    >;
    jsonrpcIpcServer.reset(new FullServer(
      new CRPCRawLog
    ));

    // CWebSocketAdvanced server(address , ssl::context::sslv23 , port , doc_root , threads);
    auto server = new CWebSocketJsonRpcServer(address, port);

    jsonrpcIpcServer->addConnector(server);
        std::cout<<"Herjbkje"<<std::endl;

    server->StartListening();
    getchar();
    server->StopListening();
  }
  catch(const std::exception& e)
  {
    std::cerr <<"Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}