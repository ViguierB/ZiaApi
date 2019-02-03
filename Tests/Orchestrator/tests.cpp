#include "Zany/Orchestrator.hpp"

int testsOrchestrator() {
	zany::Context		ctx;
	zany::Orchestrator	m(ctx);
	zany::ThreadPool	tp(8);

	m.linkThreadPool(tp);

	ctx.run();
	return (0);
}