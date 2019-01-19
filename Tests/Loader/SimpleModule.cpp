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
}

extern "C" ZANY_DLL
zany::Loader::AbstractModule	*entrypoint() {
	return new SimpleModule();
}
