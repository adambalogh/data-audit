#include <iostream>

#include <gflags/gflags.h>
#include <folly/Memory.h>
#include <folly/Portability.h>
#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <unistd.h>

#include "audit/server/proof_handler.h"

using namespace proxygen;
using namespace audit::server;

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

using Protocol = HTTPServer::Protocol;

DEFINE_int32(http_port, 8080, "Port to listen on with HTTP protocol");
DEFINE_string(ip, "10.0.0.4", "IP/Hostname to bind to");
DEFINE_int32(threads, 0,
             "Number of threads to listen on. Numbers <= 0 "
             "will use the number of cores on this machine.");

class ProofHandlerFactory : public RequestHandlerFactory {
 public:
  virtual void onServerStart() noexcept {}
  virtual void onServerStop() noexcept {}

  RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override {
    std::cout << "new request" << std::endl;
    return new ProofHandler();
  }
};

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  std::vector<HTTPServer::IPConfig> IPs = {
      {SocketAddress(FLAGS_ip, FLAGS_http_port, true), Protocol::HTTP},
  };

  if (FLAGS_threads <= 0) {
    FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
    CHECK(FLAGS_threads > 0);
  }

  HTTPServerOptions options;
  options.threads = static_cast<size_t>(FLAGS_threads);
  options.idleTimeout = std::chrono::milliseconds(60000);
  options.shutdownOn = {SIGINT, SIGTERM};
  options.enableContentCompression = true;
  options.handlerFactories =
      RequestHandlerChain().addThen<ProofHandlerFactory>().build();

  HTTPServer server(std::move(options));
  server.bind(IPs);

  // Start HTTPServer mainloop in a separate thread
  std::thread t([&]() { server.start(); });

  t.join();
  return 0;
}
