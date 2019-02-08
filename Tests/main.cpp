#include "../lib/Zany.hpp"

int testsEntity();
int testsLoader();
int testsThreadPool();
int testsPipeline();

int main() {
	/*testsEntity();
	testsLoader();
	testsThreadPool();
	testsPipeline();*/
	zany::Header non("NOT A DATE");
	zany::Header oui("Sun, 06 Nov 1994 08:49:37 GMT");

	non.isDate();
	oui.isDate();
}