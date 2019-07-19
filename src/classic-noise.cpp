#include "classic-noise.hpp"
#include "math/vecmath.hpp"

static inline Vector permute(const Vector& x) {
	return Math::mod(((x * 34.f) + 1.f) * x, Vector::load1f(289.f));
}

static inline Vector fade(const Vector& t) {
	return t * t * t * (t * (t * 6.f - 15.f) + 10.f);
}

float cnoise(float x, float y) { // TODO: swizzle the shit out of this
	Vector xyxy = Vector::make(x, y, x, y);
	Vector pi = Math::floor(xyxy) + Vector::make(0.f, 0.f, 1.f, 1.f);
	Vector pf = Math::fract(xyxy) - Vector::make(0.f, 0.f, 1.f, 1.f);
	pi = Math::mod(pi, Vector::load1f(289.f));

	Vector ix = Vector::make(pi[0], pi[2], pi[0], pi[2]);
	Vector iy = Vector::make(pi[1], pi[1], pi[3], pi[3]);
	Vector fx = Vector::make(pf[0], pf[2], pf[0], pf[2]);
	Vector fy = Vector::make(pf[1], pf[1], pf[3], pf[3]);

	Vector i = permute(permute(ix) + iy);
	Vector gx = Math::fract(i * 0.0243902439f) * 2.f - 1.f; // 1/41 = 0.024...
	Vector gy = gx.abs() - 0.5f;
	Vector tx = Math::floor(gx + 0.5f);
	gx = gx - tx;

	Vector g00 = Vector::make(gx[0], gy[0], 0.f, 0.f);
	Vector g10 = Vector::make(gx[1], gy[1], 0.f, 0.f);
	Vector g01 = Vector::make(gx[2], gy[2], 0.f, 0.f);
	Vector g11 = Vector::make(gx[3], gy[3], 0.f, 0.f);
	Vector norm = Vector::load1f(1.79284291400159f) - (Vector::make(g00.dot4(g00)[0],
				g01.dot4(g01)[0], g10.dot4(g10)[0], g11.dot4(g11)[0]) * 0.85373472095314f);

	g00 = g00 * norm[0];
	g01 = g01 * norm[1];
	g10 = g10 * norm[2];
	g11 = g11 * norm[3];

	float n00 = g00.dot4(Vector::make(fx[0], fy[0], 0.f, 0.f))[0];
	float n10 = g10.dot4(Vector::make(fx[1], fy[1], 0.f, 0.f))[0];
	float n01 = g01.dot4(Vector::make(fx[2], fy[2], 0.f, 0.f))[0];
	float n11 = g11.dot4(Vector::make(fx[3], fy[3], 0.f, 0.f))[0];

	Vector fadeXY = fade(Vector::make(pf[0], pf[1], 0.f, 0.f));
	Vector nx = Math::lerp(Vector::make(n00, n01, 0.f, 0.f), Vector::make(n10, n11, 0.f, 0.f),
			fadeXY[0]);

	return 2.3f * Math::lerp(nx[0], nx[1], fadeXY[1]);
}

/*float snoise(float vx, float vy) {
	const Vector C = Vector::make(0.211324865405187f, 0.366025403784439f,
           -0.577350269189626f, 0.024390243902439f);
	Vector v = Vector::make(vx, vy, 0, 0);
	Vector i = Math::floor( v + v.dot4(Vector::load1f(C[1])) );
	Vector x0 = v - i + i.dot4(Vector::load1f(C[0]));
	Vector i1 = (x0[0] > x0[1]) ? Vector::make(1.f, 0.f, 0.f, 0.f)
			: Vector::make(0.f 1.f, 0.f, 0.f);
	Vector x12 = Vector::make(x0[0] + C[0], x0[1] + C[0],
			x0[0] + C[2], x0[1] + C[2]);
	x12 = x12 - Vector::make(i1[0], i1[1], 0.f, 0.f);
	i = Math::mod(i, Vector::load1f(289.f));
	Vector p = permute(permute(Vector::make(0.f, i1[1], 1.f, 0.f) + i[1])
			+ i[0] + Vector::make(0.f, i1[0], 1.f, 0.f));

}*/
