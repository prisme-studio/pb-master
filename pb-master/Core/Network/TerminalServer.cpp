//
//  TerminalServer.cpp
//  pb-common
//
//  Created by Valentin Dufois on 2019-10-02.
//

#include <boost/bind.hpp>

#include "TerminalServer.hpp"

#include "../../LayoutEngine/LayoutEngine.hpp"
#include "../../LayoutEngine/Structs/Layout.hpp"
#include "../../TrackingEngine/TrackingEngine.hpp"

namespace pb {
namespace master {

void TerminalServer::socketDidOpen(BaseSocket * newSocket) {

	newSocket->setEmissionType(EmissionType::async);

	Server::socketDidOpen(newSocket);
	// Stop advertising once we have one connection
	endAdvertising();
}

void TerminalServer::socketDidClose(BaseSocket * closedSocket) {
	Server::socketDidClose(closedSocket);

	if(trackingEngine != nullptr)
		trackingEngine->setCalibrationDevices("", "");

	if(socketsCount() == 0) {
		// No more connections to this server, restart the advertiser
		advertise();
	}
}

void TerminalServer::socketDidReceive(BaseSocket * socket, protobuf::Message * aMessage) {
	// LOG_DEBUG("Received a datagram");

	Datagram * datagram = dynamic_cast<Datagram *>(aMessage);
	DatagramType dType = static_cast<DatagramType>(datagram->type());

	// Dispatch the datagram
	switch (dType) {
			// Commons
		case DatagramType::status:
			onStatusRequest(socket);
			break;
			// Layout
		case DatagramType::layoutList:
			onLayoutList(socket);
			break;
		case DatagramType::layoutCreate:
			onLayoutCreate(datagram->mutable_data(), socket);
			break;
		case DatagramType::layoutOpen:
			onLayoutOpen(datagram->mutable_data(), socket);
			break;
		case DatagramType::layoutClose:
			onLayoutClose(socket);
			break;
		case DatagramType::layoutRename:
			onLayoutRename(datagram->mutable_data(), socket);
			break;
		case DatagramType::layoutUpdate:
			onLayoutUpdate(datagram->mutable_data(), socket);
			break;
		case DatagramType::layoutDelete:
			onLayoutDelete(datagram->mutable_data(), socket);
			break;

			// Calibration
		case DatagramType::calibrationSet:
			onCalibrationSet(datagram->mutable_data());
			break;
		default:
			LOG_WARN("Received an unimplemented datagram type : " + std::to_string(dType));
	}

	delete datagram;
}

void TerminalServer::onStatusRequest(BaseSocket * socket) {
	// Build the status
	messages::MasterStatus status;

	std::string layoutName = layoutEngine->activeLayout() == nullptr ? "" : layoutEngine->activeLayout()->name;

	// Fill in the status
	status.set_activelayout(layoutName);

	for(pb::deviceUID uid: trackingEngine->getConnectedDevicesUID()) {
		status.add_connecteddevices(uid);
	}
	
	// Build the datagram
	protobuf::Message * datagram = makeDatagram(DatagramType::status, status);

	socket->send(datagram);
}

// MARK: - Layout methods

void TerminalServer::onLayoutList(BaseSocket * socket) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}

	// Format the list to protobuf
	messages::LayoutList message;
	for (const std::string &layoutName: layoutEngine->getLayouts()) {
		message.add_layoutname(layoutName);
	}

	// Build the datagram for the response
	protobuf::Message * datagram = makeDatagram(DatagramType::layoutList, message);

	socket->send(datagram);
}

void TerminalServer::onLayoutCreate(google::protobuf::Any *data, BaseSocket *socket) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}

	messages::LayoutName layoutNameMessage;
	data->UnpackTo(&layoutNameMessage);

	layout::Layout * layout = layoutEngine->createLayout(layoutNameMessage.name());

	// Build the datagram for the response
	protobuf::Message * datagram = makeDatagram(DatagramType::layoutOpen, *layout);

	socket->send(datagram);
}

void TerminalServer::onLayoutOpen(google::protobuf::Any *data, BaseSocket *socket) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}

	messages::LayoutName layoutNameMessage;
	data->UnpackTo(&layoutNameMessage);

	layout::Layout * layout = layoutEngine->openLayout(layoutNameMessage.name());

	// Build the datagram for the response
	protobuf::Message * datagram = makeDatagram(DatagramType::layoutOpen, *layout);

	socket->send(datagram);
}

void TerminalServer::onLayoutClose(BaseSocket *) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}

	// No response here
}

void TerminalServer::onLayoutRename(protobuf::Any *data, BaseSocket *) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}

	messages::LayoutName layoutNameMessage;
	data->UnpackTo(&layoutNameMessage);

	layoutEngine->renameLayout(layoutNameMessage.name());

	// No response here
}

void TerminalServer::onLayoutUpdate(protobuf::Any *data, BaseSocket *) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}

	messages::Layout * layoutMessage = new messages::Layout();
	data->UnpackTo(layoutMessage);

	layout::Layout * layout = new layout::Layout(layoutMessage);

	layoutEngine->updateLayout(layout);

	// No response here
}

void TerminalServer::onLayoutDelete(google::protobuf::Any *data, BaseSocket *socket) {
	if(layoutEngine == nullptr) {
		LOG_ERROR("Cannot perform layout operations if not layout engine is defined.");
		return;
	}


	messages::LayoutName layoutNameMessage;
	data->UnpackTo(&layoutNameMessage);

	std::vector<std::string> layoutsList = layoutEngine->deleteLayout(layoutNameMessage.name());

	// Format the list to protobuf
	messages::LayoutList message;
	for (const std::string &layoutName: layoutsList) {
		message.add_layoutname(layoutName);
	}

	// Build the datagram for the response
	protobuf::Message * datagram = makeDatagram(DatagramType::layoutList, message);

	socket->send(datagram);
}

void TerminalServer::onCalibrationSet(protobuf::Any * data) {
	messages::CalibrationDevices devices;
	data->UnpackTo(&devices);

	if(trackingEngine != nullptr) {
		trackingEngine->setCalibrationDevices(devices.devicea(), devices.deviceb());
	}
}

} /* ::master */
} /* ::pb */
