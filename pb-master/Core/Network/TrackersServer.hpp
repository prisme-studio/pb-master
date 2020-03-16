//
//  TrackersServer.hpp
//  pb-common (shared)
//
//  Created by Valentin Dufois on 2019-11-30.
//

#ifndef TrackersServer_hpp
#define TrackersServer_hpp

#include <network/Server/Server.hpp>
#include <network/Socket/BaseSocket.hpp>
#include <network/Socket/Socket.hpp>

#include <pb-common/common.hpp>

namespace pb {

class RawBody;

namespace master {

class TrackingEngine;

using namespace network;

class TrackersServer: public Server<> {
public:
	TrackersServer(const std::string &interface): Server(serverPortTracker, discoveryPortTracker, "tracker", interface) {}

	TrackingEngine * trackingEngine;

protected:
	void socketDidOpen(BaseSocket * newSocket) override;

	void socketDidClose(BaseSocket * closedSocket) override;

private:

	void socketDidReceive(BaseSocket * socket, const protobuf::Message * datagram) override;

	void onBodyStream(const protobuf::Any &data);
};

} /* ::master */
} /* ::pb */

#endif /* TrackersServer_hpp */
