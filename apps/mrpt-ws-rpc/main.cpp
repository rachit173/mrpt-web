#include <jsonrpccpp/server/abstractserverconnector.h>

#include <mrpt/poses/CPoint2D.h>
#include <mrpt/poses/CPose3DQuat.h>
#include <mrpt/poses/CPose3D.h>
#include <mrpt/poses/CPose2DInterpolator.h>
#include <mrpt/poses/CPose3DPDFParticles.h>
#include <mrpt/poses/CPose3DPDFGaussian.h>
#include <mrpt/poses/CPoint2DPDFGaussian.h>
#include <mrpt/poses/CPose3DPDFSOG.h>
#include <mrpt/poses/CPointPDFSOG.h>
#include <mrpt/poses/CPoses3DSequence.h>
#include <mrpt/math/lightweight_geom_data.h>

#include <mrpt/web/CWebSocketUtility.hpp>
#include <mrpt/web/CWebSocketAdvanced.h>
#include <mrpt/web/CModularServer.h>
#include <mrpt/web/CSchemeArchive.h>

#include "CRPCRawLogFace.h"
#include "CRPCPubSubFace.h"

#include <cstdlib>
#include <functional>
#include <thread>
#include <string>
#include <mutex>
#include <memory>
#include <iostream>

using namespace mrpt::poses;
using namespace mrpt::math;
using namespace mrpt::web;

class CRPCPubSub : public CRPCPubSubAbstract
{
public:
  virtual RPCModules implementedModules() const override
  {
    return RPCModules{RPCModule{"CRPCPubSub", "1.0"}};
  }
  Json::Value Publisher_Advertise(std::string const& topic, std::string const& type) override
  {
    Json::Value ch;
    return ch;
  }
  Json::Value Publish(const Json::Value& message, const std::string& topic) override
  {
    CPoint2D point;
    Json::Value jsonv = message;
    CSchemeArchive<Json::Value> out(jsonv);
    out.asSerializableObject(point);
    std::cout<<point.x()<<" "<<point.y()<<std::endl;
    return jsonv;
  }
  Json::Value Publisher_Unadvertise(std::string const& topic, std::string const& type) override
  {
    Json::Value ch;
    return ch;
  }
  Json::Value Subscriber_subscribe(int queue_length, int throttle_rate, std::string const& topic) override
  {
        Json::Value ch;
    return ch;
  }
  Json::Value Subscriber_unsubscribe(std::string const& topic) override
  {
        Json::Value ch;
    return ch;
  }
};

class CRPCRawLog : public CRPCRawLogAbstract
{
public:
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
    Json::Value jsonv;
    // CPoses3DSequence seq;
    // CPose3D ps;
    // seq.appendPose(ps);
    // CPose3DPDFSOG particles(5);
    // CPose3DPDFParticles particles(5);
    // CPose3DPDFGaussian ps;
    // CPose2DInterpolator path;
    // TPose2D t;
    // path.insert(1,t);
    // CPoint2D pt;
    // auto jsonv = pt.serializeTo<Json::Value>();
    return jsonv;
  }
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
      CRPCRawLog,
      CRPCPubSub
    >;
    jsonrpcIpcServer.reset(new FullServer(
      new CRPCRawLog,
      new CRPCPubSub
    ));

    auto server = new CWebSocketAdvanced(address , ssl::context::sslv23 , port , doc_root , threads);

    jsonrpcIpcServer->addConnector(server);

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