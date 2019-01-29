#include <iostream>
#include "Zany.hpp"

int testsLoader() {
	zany::Loader	l;
	zany::Pipeline	pipelineMaster;

	std::string		libPath;

	if constexpr (zany::isUnix) {
		libPath = "build-Linux/libSimpleModule.so";
	} else {
		libPath = "SimpleModule.dll";
	}

	auto &module = l.load(libPath);

	module.linkMasterPipeline(pipelineMaster);
	module.init();

	zany::ThreadPool	pool(8);

	pipelineMaster.linkThreadPool(pool);
	std::cout << (void*) &pipelineMaster << std::endl;

	zany::Pipeline::Instance newInstance;
	zany::Pipeline::Hooks::forEach([&newInstance, &pipelineMaster] (auto hook) {
		pipelineMaster.getHookSet(hook).execute(newInstance);
	});

	return (0);
}