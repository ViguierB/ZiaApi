#include "iostream"
#include "Zany/Orchestrator.hpp"

class ImplOrch : public zany::Orchestrator {
public:
	ImplOrch(zany::Context &ctx): zany::Orchestrator(ctx) {}

	virtual void routine() final {
		std::cout << ++counter << " ";

		if (counter > 32) {
			_ctx.waitUntilEmpty();
			_ctx.stop();
			std::cout << std::endl;
		}
	}
private:
	int counter = 0;
};

int testsOrchestrator() {
	zany::Context		ctx;
	ImplOrch			m(ctx);
	zany::ThreadPool	tp(8);

	m.linkThreadPool(tp);

	ctx.run();
	return (0);
}