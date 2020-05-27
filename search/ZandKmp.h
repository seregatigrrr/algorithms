#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

std::vector<int> Z(string str) {
	std::vector<int> result(str.size());
	result[0] = 0;
	int l = -1;
	int r = -1;
	for (int i = 1; i < str.size(); ++i) {
		if (r < i) {
			int j;
			l = i;
			for (r = i; r < str.size() && str[r - i] == str[r]; ++r)
				;
			result[i] = r - l;
		}
		else if ( result[i - l] < r - i) {
			result[i] = result[i - l];
		}
		else {
			l = i;
			for (; r < str.size() && str[r] == str[r - l]; ++r)
				;
			result[i] = r - l;
		}
	}
	return result;
}

std::vector<int> FindZ(const string& str, const string& pattern) {
	string temp = pattern + '#' + str;
	std::vector<int> z = Z(temp);
	std::vector<int> res;
	for (int i = 0; i < z.size(); ++i) {
		if (z[i] == pattern.size()) {
			res.push_back(i - z[i] - 1);
		}
	}
	return res;
}

std::vector<int> Sp_Z(const std::string& str) {
	std::vector<int> z = Z(str);
	std::vector<int> res(str.size());
	res[0] = 0;
	for (int j = str.size() - 1; j > 0; --j) {
		int i = j + z[j] - 1;
		res[i] = z[j];
	}
	return res;
}

std::vector<int> Sp(const std::string& str) {
	std::vector<int> res(str.size());
	res[0] = 0;
	for (int i = 1; i < str.size(); ++i) {
		int k = res[i - 1];
		for (; k > 0 && str[k] != str[i]; k = res[k - 1])
			;
		if (str[k] == str[i]) {
			++k;
		}
		res[i] = k;
	}
	return res;
}

std::vector<int> SpZ(const std::string& str) {
	std::vector<int> sp_ = Sp_Z(str);
	std::vector<int> res(str.size());
	res[str.size()- 1] = sp_.back();
	res[0] = 0;
	for (int i = str.size() - 2; i >= 1; --i) {
		res[i] = std::max(res[i + 1] - 1, sp_[i]);
	}
	return res;
}
std::vector<int> Kmp1(const std::string& str, const std::string& pattern) {
	string temp = pattern + '#' + str;
	std::vector<int> sp = Sp(temp);
	std::vector<int> res;
	for (int i = pattern.size() + 1; i < sp.size(); ++i) {
		if (sp[i] == pattern.size()) {
			res.push_back(i - 2*sp[i] - 1);
		}
	}
	return res;
}

std::vector<int> Kmp2(const std::string& str, const std::string& pattern) {
	auto sp_ = Sp_Z(pattern);
	std::vector<int> res;
	int c = 0;
	int p = 0;
	while (c + (pattern.size() - p) <= str.size()) {
		for (; str[c] == pattern[p] && p < pattern.size(); ++p, ++c)
			;
		if (p == pattern.size()) {
			res.push_back(c - p);
		}
		if (p == 0) {
			++c;
		}
		if (p > 0) {
			p = sp_[p - 1];
		}
	}
	return res;
}
