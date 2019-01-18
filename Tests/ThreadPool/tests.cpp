#include <iostream>
#include "Zany/ThreadPool.hpp"

extern "C" int testsThreadPool() {
	zany::ThreadPool	pool(80);
	std::mutex			mtx;

	for (auto i = 0; i < 100; ++i) {
		pool.runTask([i, &mtx, &pool] {
			for (auto j = 0; j < 10; ++j) {
				//std::this_thread::sleep_for(std::chrono::milliseconds(100));
				std::lock_guard<decltype(mtx)>	_guard(mtx);
				std::cout << "[" << std::this_thread::get_id() << "] Task-" << i << ": " << j << std::endl;
				std::cout << "\t" << "pool.available() -> " << pool.available() << std::endl;
			}
		});
	}
	pool.waitForEmpty();
	std::cout << "done!" << std::endl;
	return (0);
}