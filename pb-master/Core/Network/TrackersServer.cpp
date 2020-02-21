//
//  TrackersServer.cpp
//  pb-common (shared)
//
//  Created by Valentin Dufois on 2019-11-30.
//

#include <pb-common/Structs/RawBody.hpp>

#include "TrackersServer.hpp"
#include "../../TrackingEngine/TrackingEngine.hpp"

namespace pb {
namespace master {

void TrackersServer::socketDidOpen(BaseSocket * socket) {
	Server::socketDidOpen(socket);
}

void TrackersServer::socketDidReceive(BaseSocket * socket, protobuf::Message * aMessage) {
	Server::socketDidReceive(socket, aMessage);

	Datagram * datagram = dynamic_cast<Datagram *>(aMessage);
	DatagramType datagramType = static_cast<DatagramType>(datagram->type());

	// Dispatch based on the datagram type
	switch (datagramType) {
			// Tracker
		case DatagramType::rawBody:
			onBodyStream(datagram->mutable_data()); break;
		default:
			LOG_WARN("Received unimplemented datagram type : " + std::to_string(datagramType));
	}

	delete datagram;
}

void TrackersServer::socketDidClose(BaseSocket * socket) {
	Server::socketDidClose(socket);
}

void TrackersServer::onBodyStream(const protobuf::Any * data) {
	// Decode the data
	messages::RawBody messageBody;

	try {
		data->UnpackTo(&messageBody);
	} catch (protobuf::FatalException * e) {
		LOG_ERROR("Error while deserializing a body. Ignoring...");
		LOG_DEBUG(e->message());

		return;
	}

	try{
		trackingEngine->onRawBody(new RawBody(messageBody));
	} catch(std::runtime_error &e) {
		return;
	}
}

} /* ::master */
} /* ::pb */
