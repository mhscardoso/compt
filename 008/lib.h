using namespace std;

template <typename T, typename F>
void operator|(T& arr, F func) {

	for (auto x: arr) {
		func(x);
	}

}
