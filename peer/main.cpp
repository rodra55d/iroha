/*
#include <thread>
#include <signal.h>
#include <unistd.h>
#include <atomic>

#include "../core/server/http_server.hpp"
#include "../core/peer/connection.hpp"
#include "../core/consensus/sumeragi.hpp"

#include "../core/util/yaml_loader.hpp"
#include "../core/util/logger.hpp"
#include "../core/peer/connection.hpp"

std::atomic_bool running(true); 

void server(){
  http::server();
}

void sigIntHandler(int param){
  running = false;
  logger::info("main", "will halt");
}
*/
int main() {
  /*
  signal(SIGINT, sigIntHandler);

  if(getenv("IROHA_HOME") == nullptr){
    std::cout << "You must set IROHA_HOME!" << std::endl;
    return 1;
  }

  //std::cout<<"Process ID is "<< getpid() << std::endl;
  std::unique_ptr<connection::Config> config;
  std::unique_ptr<yaml::YamlLoader>   yamlLoader(new yaml::YamlLoader(std::string(getenv("IROHA_HOME"))+"/config/config.yml"));

  config->address = std::move(yamlLoader->get<std::string>("mediaDriver", "address"));
  config->port    = std::move(yamlLoader->get<std::string>("mediaDriver", "port"));
  config->name    = std::move(yamlLoader->get<std::string>("mediaDriver", "name"));
  config->publishStreamId    = yamlLoader->get<int>("mediaDriver", "publishStreamId");
  config->subscribeStreamId  = yamlLoader->get<int>("mediaDriver", "subscribeStreamId");
  
  connection::initialize_peer(std::move(config));

  std::thread http_th( server );

 // std::cout << "(Second) Process ID is " << getpid() << std::endl;
 sumeragi::initializeSumeragi(11, 2, 2);
 sumeragi::loop();

  while(running){}

  http_th.detach();
  */
  return 0;
}
