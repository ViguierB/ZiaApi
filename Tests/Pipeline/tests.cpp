#include "Zany.hpp"
#include <iostream>

int	testsPipeline() {
	auto 				&master = zany::Pipeline::master();
	zany::ThreadPool	pool(8);
	auto				&afterAccept = master.getHookSet<zany::Pipeline::Hooks::AFTER_ACCEPT>();

	master.linkTheadPool(pool);
	afterAccept.addTask([] (zany::Pipeline::Instance &) {
		std::cout << "Low" << std::endl;
	});
	afterAccept.addTask<zany::Pipeline::Priority::MEDIUM, zany::Pipeline::Rights::READ_ONLY>([] (zany::Pipeline::Instance const &) {
		std::cout << "Medium2" << std::endl;
	});
	afterAccept.addTask<zany::Pipeline::Priority::HIGH>([] (zany::Pipeline::Instance &) {
		std::cout << "High" << std::endl;
	});
	afterAccept.addTask<zany::Pipeline::Priority::MEDIUM, zany::Pipeline::Rights::READ_ONLY>([] (zany::Pipeline::Instance const &) {
		std::cout << "Medium4" << std::endl;
	});
	afterAccept.addTask<zany::Pipeline::Priority::MEDIUM, zany::Pipeline::Rights::READ_ONLY>([] (zany::Pipeline::Instance const &) {
		std::cout << "Medium1" << std::endl;
	});
	afterAccept.addTask<zany::Pipeline::Priority::MEDIUM, zany::Pipeline::Rights::READ_ONLY>([] (zany::Pipeline::Instance const &) {
		std::cout << "Medium3" << std::endl;
	});


	zany::Pipeline::Instance	newInstance;
	master.executeHook<zany::Pipeline::Hooks::AFTER_ACCEPT>(newInstance);
	return 0;
}