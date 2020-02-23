//
//  TrackersJSONServer.hpp
//  pb-common (shared)
//
//  Created by Valentin Dufois on 2020-02-05.
//

#ifndef TrackersJSONServer_hpp
#define TrackersJSONServer_hpp

#include <network/Server/Server.hpp>
#include <network/Socket/Socket.hpp>
#include <network/Socket/SocketDelegate.hpp>

#include <pb-common/common.hpp>
#include <pb-common/messages.hpp>

using namespace network;

namespace pb {

class RawBody;

namespace master {

class TrackingEngine;

class TrackersJSONServer: public Server<messages::RawBody> {
public:
	TrackersJSONServer(const std::string &interface): Server(serverPortTrackerJSON, discoveryPortTrackerJSON, "tracker", interface) {
		Server::setEmissionFormat(json);
	}

	TrackingEngine * trackingEngine;

protected:
	void socketDidOpen(BaseSocket * newSocket) override;

	void socketDidClose(BaseSocket * closedSocket) override;

private:

	void socketDidReceive(BaseSocket * socket, protobuf::Message * datagram) override;
};

} /* ::master */
} /* ::pb */

#endif /* TrackersJSONServer_hpp */
