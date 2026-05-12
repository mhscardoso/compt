using namespace std;

template <typename T, typename F>
auto operator|(const T& arr, F func) {

	using Elem = decay_t<decltype(*begin(arr))>;
	using Ret  = decay_t<invoke_result_t<F, Elem>>;

	if constexpr (is_same_v<Ret, void>) {
		// print
		for_each(begin(arr), end(arr), func);

		return;
	}
	else if constexpr (is_same_v<Ret, bool>) {
		// filter
		vector<Elem> result;

		for (const auto& e : arr) {
			if (invoke(func, e)) {
				result.push_back(e);
			}
		}

		return result;

	} else {
		// map
		vector<Ret> result;

		for (const auto& e : arr) {
			result.push_back(invoke(func, e));
		}

		return result;
	}
}
