#pragma once

#include "math/math.hpp"

class InputControl {
	public:
		InputControl();

		void setAmt(float amt);
		void addAmt(float amtDelta);
		float getAmt() const;
	private:
		float amt;
};

inline InputControl::InputControl()
	: amt(0.f)
 {}

inline void InputControl::addAmt(float amtDelta) {
	amt += amtDelta;
}

inline float InputControl::getAmt() const {
	return Math::clamp(amt, -1.f, 1.f);
}

inline void InputControl::setAmt(float amt) {
	this->amt = amt;
}
