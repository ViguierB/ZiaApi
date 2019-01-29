#include <iostream>
#include "Zany.hpp"

class SimpleModule: public zany::Loader::AbstractModule {
public:
	SimpleModule() = default;
	~SimpleModule() { printf("SimpleModule Deleted!\n"); };

	virtual void	init();
private:
	/* data */
};

void	SimpleModule::init() {
	std::cout << "SimpleModule init()" << std::endl;

	// garbage << master.getHookSet<zany::Pipeline::Hooks::ON_DATA_AVAILABLE>().addTask([this] (zany::Pipeline::Instance &) {
	// std::cout << (void*) &master << std::endl;
	// 	std::cout << "test" << std::endl;
	// });
}

extern "C" ZANY_DLL
zany::Loader::AbstractModule	*entrypoint() {
	return new SimpleModule();
}

