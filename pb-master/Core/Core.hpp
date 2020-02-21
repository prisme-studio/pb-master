//
//  Core.hpp
//  pb-tracker
//
//  Created by Valentin Dufois on 2019-09-21.
//

#ifndef Core_hpp
#define Core_hpp

#include <common/flags.hpp>
#include <pb-common/messages.hpp>

#include "Network/NetworkManager.hpp"
#include "../LayoutEngine/LayoutEngine.hpp"
#include "../TrackingEngine/TrackingEngine.hpp"
#include "../TrackingEngine/TrackingEngineDelegate.hpp"

namespace pb {
struct RawBody;

namespace master {

class Core: public TrackingEngineDelegate {
public:
	Core(): _networkManager(flags::args.count("interface") ? flags::args["interface"].as<std::string>() : "") {}

	void init();

	void run();
private:

	bool _isRunning = true;

	NetworkManager _networkManager;

	LayoutEngine _layoutEngine;

	TrackingEngine _trackingEngine;

	// MARK: TrackingEngineDelegate

	void trackingEngineUpdatedBody(TrackingEngine *, Body *);

	void trackingEngineFinishedCycle(TrackingEngine *);

	std::vector<pb::messages::PartialBody *> _partialBodies;
};

} /* ::master */
} /* ::pb */

#endif /* Core_hpp */
