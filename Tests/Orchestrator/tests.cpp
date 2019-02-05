#include "iostream"
#include "Zany/Orchestrator.hpp"

class ImplOrch : public zany::Orchestrator {
public:
	ImplOrch(zany::Context &ctx): zany::Orchestrator(ctx) {
		test = &_pline.createInstance();

		test->properties["patate"] = zany::Property::make<std::string>("Patate");
	}

	virtual void routine() final {
		std::cout << ++counter << " ";

		if (counter > 32) {
			_ctx.waitUntilEmpty();
			_ctx.stop();
			std::cout << std::endl;
			std::cout << test->properties["patate"].get<std::string>() << std::endl;
			test->properties["patate"].set<int>(32);
			std::cout << test->properties["patate"].get<int>() << std::endl;
		}
	}
private:
	int 						counter = 0;
	zany::Pipeline::Instance	*test; //Property Test !
};

int testsOrchestrator() {
	zany::Context		ctx;
	ImplOrch			m(ctx);
	zany::ThreadPool	tp(8);

	m.linkThreadPool(tp);

	ctx.run();
	return (0);
}