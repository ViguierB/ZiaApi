extern "C" void testsEntity();
extern "C" void testsLoader();
extern "C" void testsThreadPool();

int main() {
	testsEntity();
	testsLoader();
	testsThreadPool();
}