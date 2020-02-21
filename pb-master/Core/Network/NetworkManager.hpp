//
//  NetworkManager.hpp
//  pb-common
//
//  Created by Valentin Dufois on 2019-09-21.
//

#ifndef NetworkManager_hpp
#define NetworkManager_hpp

#include <vector>
#include <functional>

#include <network/Server/Server.hpp>

#include "TerminalServer.hpp"
#include "TrackersServer.hpp"
#include "TrackersJSONServer.hpp"

using namespace network;

namespace pb {
namespace master {

class LayoutEngine;
class TrackingEngine;

class NetworkManager {
public:

	// MARK: LifeCycle
	NetworkManager(const std::string &interface);

	/// Start all activities of the network manager
	void startActivities();


	// MARK: - Setters

	inline void setLayoutEngine(LayoutEngine * layoutEngine) {
		_terminalServer.layoutEngine = layoutEngine;
	}


	inline void setTrackingEngine(TrackingEngine * trackingEngine) {
		_trackersServer.trackingEngine = trackingEngine;
		_trackersServerJSON.trackingEngine = trackingEngine;
		_terminalServer.trackingEngine = trackingEngine;
	}

	// MARK: - Emitters

	void sendToReceivers(protobuf::Message * message);

	void sendToTerminal(protobuf::Message * message);

private:

	/// The trackers server
	TrackersServer _trackersServer;

	/// The trackers server
	TrackersJSONServer _trackersServerJSON;

	/// The receivers server
	Server<> _receiversServer;

	/// The terminal server
	TerminalServer _terminalServer;

};

} /* ::master */
} /* ::pb */

#endif /* NetworkManager_hpp */
