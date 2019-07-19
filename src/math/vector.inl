FORCEINLINE float Vector3f::dot(const Vector3f& other) const {
	return vec.dot3(other.vec)[0];
}

FORCEINLINE Vector3f Vector3f::dotToVector(const Vector3f& other) const {
	return Vector3f(vec.dot3(other.vec));
}

FORCEINLINE Vector3f Vector3f::cross(const Vector3f& other) const {
	return Vector3f(vec.cross3(other.vec));
}

FORCEINLINE float Vector3f::dist(const Vector3f& other) const {
	return Math::sqrt(distSquared(other));
}

FORCEINLINE float Vector3f::distSquared(const Vector3f& other) const {
	Vector3f temp = other - *this;
	return temp.vec.dot3(temp.vec)[0];
}

FORCEINLINE Vector3f Vector3f::operator+(const Vector3f& other) const {
	return Vector3f(vec + other.vec);
}

FORCEINLINE Vector3f Vector3f::operator-(const Vector3f& other) const {
	return Vector3f(vec - other.vec);
}

FORCEINLINE Vector3f Vector3f::operator*(const Vector3f& other) const {
	return Vector3f(vec * other.vec);
}

FORCEINLINE Vector3f Vector3f::operator/(const Vector3f& other) const {
	return Vector3f(vec / other.vec);
}

FORCEINLINE Vector3f Vector3f::operator+(float amt) const {
	return Vector3f(vec + Vector::load1f(amt));
}

FORCEINLINE Vector3f Vector3f::operator-(float amt) const {
	return Vector3f(vec - Vector::load1f(amt));
}

FORCEINLINE Vector3f Vector3f::operator*(float amt) const {
	return Vector3f(vec * Vector::load1f(amt));
}

FORCEINLINE Vector3f Vector3f::operator/(float amt) const {
	return Vector3f(vec * Vector::load1f(Math::reciprocal(amt)));
}

FORCEINLINE Vector3f Vector3f::operator-() const {
	return Vector3f(-vec);
}

FORCEINLINE Vector3f Vector3f::operator+=(const Vector3f& other) {
	vec = vec + other.vec;
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator-=(const Vector3f& other) {
	vec = vec - other.vec;
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator*=(const Vector3f& other) {
	vec = vec * other.vec;
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator/=(const Vector3f& other) {
	vec = vec / other.vec;
	return *this;
}

FORCEINLINE float Vector3f::length() const {
	return Math::sqrt(lengthSquared());
}

FORCEINLINE float Vector3f::lengthSquared() const {
	return vec.dot3(vec)[0];
}

FORCEINLINE Vector3f Vector3f::operator+=(float val) {
	vec = vec + Vector::load1f(val);
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator-=(float val) {
	vec = vec - Vector::load1f(val);
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator*=(float val) {
	vec = vec * Vector::load1f(val);
	return *this;
}

FORCEINLINE Vector3f Vector3f::operator/=(float val) {
	vec = vec * Vector::load1f(Math::reciprocal(val));
	return *this;
}

FORCEINLINE Vector Vector3f::toVector() const {
	return vec;
}

FORCEINLINE float Vector2f::dot(const Vector2f& other) const {
	return vals[0]*other.vals[0]+vals[1]*other.vals[1];
}

FORCEINLINE Vector2f Vector2f::dotToVector(const Vector2f& other) const {
	return Vector2f(dot(other));
}

FORCEINLINE float Vector2f::cross(const Vector2f& other) const {
	return vals[0]*other.vals[1]-vals[1]*other.vals[0];
}

FORCEINLINE float Vector2f::dist(const Vector2f& other) const {
	return Math::sqrt(distSquared(other));
}

FORCEINLINE float Vector2f::distSquared(const Vector2f& other) const {
	return (other - *this).lengthSquared();
}

FORCEINLINE float Vector2f::length() const {
	return Math::sqrt(lengthSquared());
}

FORCEINLINE float Vector2f::lengthSquared() const {
	return dot(*this);
}

FORCEINLINE Vector2f Vector2f::operator+(const Vector2f& other) const {
	return Vector2f(vals[0] + other.vals[0], vals[1] + other.vals[1]);
}

FORCEINLINE Vector2f Vector2f::operator-(const Vector2f& other) const {
	return Vector2f(vals[0] - other.vals[0], vals[1] - other.vals[1]);
}

FORCEINLINE Vector2f Vector2f::operator*(const Vector2f& other) const {
	return Vector2f(vals[0] * other.vals[0], vals[1] * other.vals[1]);
}

FORCEINLINE Vector2f Vector2f::operator/(const Vector2f& other) const {
	return Vector2f(vals[0] / other.vals[0], vals[1] / other.vals[1]);
}

FORCEINLINE Vector2f Vector2f::operator+(float amt) const {
	return Vector2f(vals[0] + amt, vals[1] + amt);
}
FORCEINLINE Vector2f Vector2f::operator-(float amt) const {
	return Vector2f(vals[0] - amt, vals[1] - amt);
}
FORCEINLINE Vector2f Vector2f::operator*(float amt) const {
	return Vector2f(vals[0] * amt, vals[1] * amt);
}
FORCEINLINE Vector2f Vector2f::operator/(float amt) const {
	return Vector2f(vals[0] / amt, vals[1] / amt);
}

FORCEINLINE Vector2f Vector2f::operator-() const {
	return Vector2f(-vals[0], -vals[1]);
}

FORCEINLINE Vector2f Vector2f::operator+=(const Vector2f& other) {
	vals[0] += other.vals[0];
	vals[1] += other.vals[1];
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator-=(const Vector2f& other) {
	vals[0] -= other.vals[0];
	vals[1] -= other.vals[1];
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator*=(const Vector2f& other) {
	vals[0] *= other.vals[0];
	vals[1] *= other.vals[1];
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator/=(const Vector2f& other) {
	vals[0] /= other.vals[0];
	vals[1] /= other.vals[1];
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator+=(float val) {
	vals[0] += val;
	vals[1] += val;
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator-=(float val) {
	vals[0] -= val;
	vals[1] -= val;
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator*=(float val) {
	vals[0] *= val;
	vals[1] *= val;
	return *this;
}

FORCEINLINE Vector2f Vector2f::operator/=(float val) {
	vals[0] /= val;
	vals[1] /= val;
	return *this;
}
