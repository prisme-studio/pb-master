//
//  NetworkManager.cpp
//  pb-common
//
//  Created by Valentin Dufois on 2019-09-21.
//

#include "NetworkManager.hpp"

namespace pb {
namespace master {

NetworkManager::NetworkManager(const std::string &interface):
_trackersServer(),
_trackersServerJSON(),
_receiversServer(serverPortReceiver, discoveryPortReceiver, "receiver"),
_terminalServer()
{}

void NetworkManager::startActivities() {
	_trackersServer.advertise();
	_trackersServer.open();

	_trackersServerJSON.open();

	_receiversServer.advertise();
	_receiversServer.open();

	_terminalServer.advertise();
	_terminalServer.open();
}

void NetworkManager::sendToTerminal(protobuf::Message * message) {
	_terminalServer.sendToAll(message);
}

void NetworkManager::
sendToReceivers(protobuf::Message * message) {
	_receiversServer.sendToAll(message);
}

} /* ::master */
} /* ::pb */
