#pragma once

#include <vector>

template <typename T>
class Array : public std::vector<T> {
	public:
		Array<T>() : std::vector<T>() {}
		Array<T>(size_t n) : std::vector<T>(n) {}

		inline void swap_remove(size_t i) {
			//T& temp = (*this)[i];
			//(*this)[i] = (*this)[this->size() - 1];
			//(*this)[this->size() - 1] = temp;

			std::swap((*this)[i], (*this)[this->size() - 1]);
			this->pop_back();
		}
};
