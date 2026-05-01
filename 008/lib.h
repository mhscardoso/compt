using namespace std;

template <typename T, typename F>
void operator|(T& arr, F func) {

	for (auto i = begin(arr); i != end(arr); ++i) {
		func(*i);
	}

}
