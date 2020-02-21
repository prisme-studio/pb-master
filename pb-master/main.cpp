//
//  main.cpp
//  pb-Master
//
//  Created by Valentin Dufois on 2019-09-17.
//

#include <common/flags.hpp>
#include <common/thread.hpp>

#include <network/Engine.hpp>

#include "Core/Core.hpp"

void signalHandler(int signum) {
	LOG_INFO("Closing services...");

	// Properly terminate the application services
//	pb::master::Core::instance()->terminate();

	exit(signum);
}

int main(const int argc, const char * argv[]) {
	// Register our closing mechanisms
	signal(SIGINT, signalHandler);

	// Set app parameters
	po::options_description desc("pb-master");
	desc.add_options()("layout", po::value<std::string>(), "Layout to open on start");
	flags::parse(argc, argv, desc);

	thread::setName("pb.master");

	network::Engine::thisMachineType = "master";

	// Start the core
	pb::master::Core core;
	core.init();

	core.run();

	return 0;
}
