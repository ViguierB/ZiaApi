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

	zany::Pipeline::Instance newInstance;
	zany::Pipeline::Hooks::forEach([&newInstance, &pipelineMaster] (auto hook) {
		pipelineMaster.getHookSet(hook).execute(newInstance);
	});

	l.unload(module);
	return (0);
}