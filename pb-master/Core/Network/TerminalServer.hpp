//
//  TerminalServer.hpp
//  pb-common
//
//  Created by Valentin Dufois on 2019-10-02.
//

#ifndef MasterServer_hpp
#define MasterServer_hpp

#include <network/Socket/BaseSocket.hpp>
#include <network/Server/Server.hpp>

#include <pb-common/common.hpp>

namespace pb {
namespace master {

using namespace network;

class TrackingEngine;
class LayoutEngine;

class TerminalServer: public Server<> {
public:

	TerminalServer(const std::string &interface): Server<network::messages::Datagram>(serverPortTerminal, discoveryPortTerminal, "terminal", interface) {}

	/// Handle to the layout engine
	LayoutEngine * layoutEngine = nullptr;

	TrackingEngine * trackingEngine = nullptr;

protected:
	void socketDidOpen(BaseSocket * newSocket) override;

	void socketDidClose(BaseSocket * closedSocket) override;

private:

	void socketDidReceive(BaseSocket * socket, const protobuf::Message * datagram) override;

	void socketDidSendAsynchronously(BaseSocket * socket, const protobuf::Message * datagram) override;

	void onStatusRequest(BaseSocket * socket);

	// MARK: - Layout methods

	void onLayoutList(BaseSocket * socket);

	void onLayoutCreate(const protobuf::Any &ata, BaseSocket * socket);

	void onLayoutOpen(const protobuf::Any &data, BaseSocket * socket);

	void onLayoutClose(BaseSocket * socket);

	void onLayoutRename(const protobuf::Any &data, BaseSocket * socket);

	void onLayoutUpdate(const protobuf::Any &data, BaseSocket * socket);

	void onLayoutDelete(const protobuf::Any &data, BaseSocket * socket);

	void onCalibrationSet(const protobuf::Any &data);
};

} /* ::master */
} /* ::pb */

#endif /* MasterServer_hpp */
