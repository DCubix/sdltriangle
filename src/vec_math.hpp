#ifndef VEC_MATH_HPP
#define VEC_MATH_HPP

#include <cstdint>
#include <cmath>
#include <array>
#include <ostream>
#include <algorithm>

#if defined(__SSE__) || (_M_IX86_FP > 0) || (_M_X64 > 0)
	#define HAS_SSE
#endif

#if defined(HAS_SSE)
#	if defined(_M_IX86_FP)
#		if _M_IX86_FP >=2
#			define HAS_SSE4_1
#		endif
#	elif defined(__SSE4_1__)
#		define HAS_SSE4_1
#endif

#if defined(_MSC_VER)
#	include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#	include <x86intrin.h>
#endif
#	include <xmmintrin.h>
#	if defined(HAS_SSE4_1)
#		include <smmintrin.h>
#	endif
#endif

namespace consts {
	const float Pi = 3.14159265358f;
	const float InvPi = 1.0f / Pi;
	const float InvTwoPi = 2.0f / Pi;
	const float HalfPi = Pi / 2.0f;
	const float QuarPi = Pi / 4.0f;
	const float TwoPi = Pi * 2.0f;
	const float Epsilon = 1e-5f;
	const float E = 2.71828182845f;
}

namespace mathutils {
	inline float toRadians(float degs) {
		return degs * consts::Pi / 180.0f;
	}

	inline float toDegrees(float rads) {
		return rads / consts::Pi * 180.0f;
	}

	inline float random(float from, float to) {
		return from + (float(rand()) / RAND_MAX) * (to - from);
	}
}

class Vector2 {
public:
	inline Vector2() : x(0.0f), y(0.0f) {}
	inline Vector2(float v) : x(v), y(v) {}
	inline Vector2(float x, float y) : x(x), y(y) {}

	inline float dot(const Vector2& v) const {
		return x * v.x + y * v.y;
	}

	inline float length() const {
		return std::sqrt(this->dot(*this));
	}

	inline Vector2 normalized() const {
		float nlen = 1.0f / length();
		return (*this) * nlen;
	}

	inline Vector2 lerp(const Vector2& to, float factor) const {
		return (*this) * (1.0f - factor) + to * factor;
	}

	inline Vector2 operator +(const Vector2& o) const {
		return Vector2(x + o.x, y + o.y);
	}

	inline Vector2 operator -(const Vector2& o) const {
		return Vector2(x - o.x, y - o.y);
	}

	inline Vector2 operator *(const Vector2& o) const {
		return Vector2(x * o.x, y * o.y);
	}

	inline Vector2 operator /(const Vector2& o) const {
		return Vector2(x / o.x, y / o.y);
	}

	inline Vector2 operator *(float o) const {
		return Vector2(x * o, y * o);
	}

	inline Vector2 operator /(float o) const {
		return Vector2(x / o, y / o);
	}

	float x, y;
};

inline std::ostream& operator<<(std::ostream& o, const Vector2& v) {
	return o << "Vector2(" << v.x << ", " << v.y << ")";
}

class Vector3 {
public:
	inline Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	inline Vector3(float v) : x(v), y(v), z(v) {}
	inline Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	inline Vector3(const Vector2& v, float z = 0.0f) : x(v.x), y(v.y), z(z) {}

	inline Vector2 toVector2() const { return Vector2(x, y); }

	inline float dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline Vector3 cross(const Vector3& v) const {
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}

	inline Vector3 lerp(const Vector3& to, float factor) const {
		return (*this) * (1.0f - factor) + to * factor;
	}

	inline float length() const {
		return std::sqrt(this->dot(*this));
	}

	inline Vector3 normalized() const {
		float nlen = 1.0f / length();
		return (*this) * nlen;
	}

	inline Vector3 operator +(const Vector3& o) const {
		return Vector3(x + o.x, y + o.y, z + o.z);
	}

	inline Vector3 operator -(const Vector3& o) const {
		return Vector3(x - o.x, y - o.y, z - o.z);
	}

	inline Vector3 operator *(const Vector3& o) const {
		return Vector3(x * o.x, y * o.y, z * o.z);
	}

	inline Vector3 operator /(const Vector3& o) const {
		return Vector3(x / o.x, y / o.y, z / o.z);
	}

	inline Vector3 operator *(float o) const {
		return Vector3(x * o, y * o, z * o);
	}

	inline Vector3 operator /(float o) const {
		return Vector3(x / o, y / o, z / o);
	}

	float x, y, z;
};

inline std::ostream& operator<<(std::ostream& o, const Vector3& v) {
	return o << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

class Vector4 {
public:
	inline Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	inline Vector4(float v) : x(v), y(v), z(v), w(z) {}
	inline Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	inline Vector4(const Vector2& v, float z = 0.0f, float w = 0.0f) : x(v.x), y(v.y), z(z), w(w) {}
	inline Vector4(const Vector3& v, float w = 0.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

	inline Vector2 toVector2() const { return Vector2(x, y); }
	inline Vector3 toVector3() const { return Vector3(x, y, z); }

	inline float dot(const Vector4& v) const {
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	inline float length() const {
		return std::sqrt(this->dot(*this));
	}

	inline Vector4 normalized() const {
		float nlen = 1.0f / length();
		return (*this) * nlen;
	}

	inline Vector4 lerp(const Vector4& to, float factor) const {
		return (*this) * (1.0f - factor) + to * factor;
	}

	inline Vector4 operator +(const Vector4& o) const {
		return Vector4(x + o.x, y + o.y, z + o.z, w + o.w);
	}

	inline Vector4 operator -(const Vector4& o) const {
		return Vector4(x - o.x, y - o.y, z - o.z, w - o.w);
	}

	inline Vector4 operator *(const Vector4& o) const {
		return Vector4(x * o.x, y * o.y, z * o.z, w * o.w);
	}

	inline Vector4 operator /(const Vector4& o) const {
		return Vector4(x / o.x, y / o.y, z / o.z, w / o.w);
	}

	inline Vector4 operator *(float o) const {
		return Vector4(x * o, y * o, z * o, w * o);
	}

	inline Vector4 operator /(float o) const {
		return Vector4(x / o, y / o, z / o, w / o);
	}

	inline float& operator [](size_t i) {
		switch (i) {
			default:
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
		}
	}

	inline const float& operator [](size_t i) const {
		switch (i) {
			default:
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
		}
	}

	float x, y, z, w;
};

inline std::ostream& operator<<(std::ostream& o, const Vector4& v) {
	return o << "Vector4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}

class Matrix4 {
public:
	inline Matrix4() {}
	inline Matrix4(const std::array<float, 16>& m) {
		m_rows[0] = Vector4(m[0], m[1], m[2], m[3]);
		m_rows[1] = Vector4(m[4], m[5], m[6], m[7]);
		m_rows[2] = Vector4(m[8], m[9], m[10], m[11]);
		m_rows[3] = Vector4(m[12], m[13], m[14], m[15]);
	}

	inline float* data() { return &m_rows[0][0]; }

	inline static Matrix4 identity() {
		return Matrix4({
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 translation(const Vector3& t) {
		return Matrix4({
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 scale(const Vector3& s) {
		return Matrix4({
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 uniformScale(float s) {
		return Matrix4({
			s, 0, 0, 0,
			0, s, 0, 0,
			0, 0, s, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 rotationX(float angle) {
		const float s = std::sin(angle), c = std::cos(angle);
		return Matrix4({
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 rotationY(float angle) {
		const float s = std::sin(angle), c = std::cos(angle);
		return Matrix4({
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 rotationZ(float angle) {
		const float s = std::sin(angle), c = std::cos(angle);
		return Matrix4({
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 rotation(const Vector3& forward, const Vector3& up, const Vector3& right) {
		const Vector3& f = forward, u = up, r = right;
		return Matrix4({
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			f.x, f.y, f.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		});
	}

	inline static Matrix4 rotation(const Vector3& forward, const Vector3& up) {
		const Vector3 f = forward.normalized();
		Vector3 r = up.normalized();
		r = r.cross(f);

		Vector3 u = f.cross(r);
		return rotation(f, u, r);
	}

	inline static Matrix4 lookAt(const Vector3& eye, const Vector3& at, const Vector3& up) {
		const Vector3 z = (eye - at).normalized();
		const Vector3 x = up.cross(z).normalized();
		const Vector3 y = z.cross(x).normalized();

		const Matrix4 r({
			x.x, x.y, x.z, -x.dot(eye),
			y.x, y.y, y.z, -y.dot(eye),
			z.x, z.y, z.z, -z.dot(eye),
			0.0f, 0.0f, 0.0f, 1.0f
		});
		return r;
	}

	inline static Matrix4 angleAxis(float angle, const Vector3& axis) {
		const float s = std::sin(angle),
					c = std::cos(angle),
					t = 1.0f - c;

		Vector3 ax = axis.normalized();
		float x = ax.x, y = ax.y, z = ax.z;
		return Matrix4({
			t * x * x + c, t * x * y - z * s, t * x * z + y * s, 0,
			t * x * y + z * s, t * y * y + c, t * y * z + x * s, 0,
			t * x * z - y * s, t * y * z + x * s, t * z * z + c, 0,
			0, 0, 0, 1
		});
	}

	inline static Matrix4 ortho(float left, float right, float bottom, float top, float near, float far) {
		const float w = right - left;
		const float h = top - bottom;
		const float d = far - near;
		return Matrix4({
			2.0f / w, 0.0f, 0.0f, -((right + left) / w),
			0.0f, 2.0f / h, 0.0f, -((top + bottom) / h),
			0.0f, 0.0f, -2.0f / d, -((far + near) / d),
			0.0f, 0.0f, 0.0f, 1.0f
		});
	}

	inline static Matrix4 perspective(float fov, float aspect, float near, float far) {
		const float thf = std::tan(fov / 2.0f);
		const float w = 1.0f / thf;
		const float h = aspect / thf;
		return Matrix4({
			w   , 0.0f, 0.0f, 0.0f,
			0.0f, h   , 0.0f, 0.0f,
			0.0f, 0.0f, far / (near - far), far * near / (near - far),
			0.0f, 0.0f, -1.0f, 0.0f
		});
	}

	inline Matrix4 operator *(const Matrix4& mat) {
		Matrix4 ret{};
#if defined(HAS_SSE)
		__m128 out0x = lincomb_SSE(s_rows[0], mat);
		__m128 out1x = lincomb_SSE(s_rows[1], mat);
		__m128 out2x = lincomb_SSE(s_rows[2], mat);
		__m128 out3x = lincomb_SSE(s_rows[3], mat);
		ret.s_rows[0] = out0x;
		ret.s_rows[1] = out1x;
		ret.s_rows[2] = out2x;
		ret.s_rows[3] = out3x;
#else
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				ret[i][j] = 0.0f;
				for (size_t k = 0; k < 4; k++) {
					ret[i][j] += (*this)[i][k] * mat[k][j];
				}
			}
		}
#endif
		return ret;
	}

	inline Vector4 operator *(const Vector4& v) const {
		const Matrix4& m = (*this);
		return Vector4(
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
			m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		);
	}

	inline Vector3 operator *(const Vector3& v) {
		return ((*this) * Vector4(v, 1.0f)).toVector3();
	}

	inline Matrix4 transposed() const {
		Matrix4 res{};
#if defined(HAS_SSE)
		res = (*this);
		_MM_TRANSPOSE4_PS(res.s_rows[0], res.s_rows[1], res.s_rows[2], res.s_rows[3]);
#else
		res = Matrix4({
			m_rows[0].x, m_rows[1].x, m_rows[2].x, m_rows[3].x,
			m_rows[0].y, m_rows[1].y, m_rows[2].y, m_rows[3].y,
			m_rows[0].z, m_rows[1].z, m_rows[2].z, m_rows[3].z,
			m_rows[0].w, m_rows[1].w, m_rows[2].w, m_rows[3].w
		});
#endif
		return res;
	}

	inline Matrix4 inverse() {
		Matrix4 res{};
#if 0
		// TODO: Implement SSE maybe?
#else
		float* m = data();
		float inv[16], det;

		inv[0] = m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		inv[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return (*this);

		det = 1.0f / det;

		for (int i = 0; i < 16; i++) res.data()[i] = inv[i] * det;
#endif
		return res;
	}

	Vector4& operator [](size_t i) { return m_rows[i]; }
	const Vector4& operator [](size_t i) const { return m_rows[i]; }

private:
	union {
		Vector4 m_rows[4];
#if defined(HAS_SSE)
		__m128 s_rows[4];
#endif
	};

#if defined(HAS_SSE)
	inline __m128 lincomb_SSE(const __m128 &a, const Matrix4& b) {
		__m128 result;
		result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), b.s_rows[0]);
		result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), b.s_rows[1]));
		result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), b.s_rows[2]));
		result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), b.s_rows[3]));
		return result;
	}
#endif
};

inline std::ostream& operator<<(std::ostream& o, const Matrix4& v) {
	return o << "Matrix4(\n" <<
		"\t" << v[0] << "\n" <<
		"\t" << v[1] << "\n" <<
		"\t" << v[2] << "\n" <<
		"\t" << v[3] << "\n)";
}

class Quaternion {
public:
	inline Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	inline Quaternion(float v) : x(v), y(v), z(v), w(z) {}
	inline Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	inline Quaternion(const Vector2& v, float z = 0.0f, float w = 1.0f) : x(v.x), y(v.y), z(z), w(w) {}
	inline Quaternion(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}
	inline Quaternion(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	inline Quaternion(const Quaternion& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	inline Quaternion(const Matrix4& a) {
		float trace = a[0][0] + a[1][1] + a[2][2];

		if(trace > 0) {
			float s = 0.5f / std::sqrt(trace + 1.0f);
			w = 0.25f / s;
			x = (a[1][2] - a[2][1]) * s;
			y = (a[2][0] - a[0][2]) * s;
			z = (a[0][1] - a[1][0]) * s;
		} else {
			if(a[0][0] > a[1][1] && a[0][0] > a[2][2]) {
				float s = 2.0f * std::sqrt(1.0f + a[0][0] - a[1][1] - a[2][2]);
				w = (a[1][2] - a[2][1]) / s;
				x = 0.25f * s;
				y = (a[1][0] + a[0][1]) / s;
				z = (a[2][0] + a[0][2]) / s;
			} else if(a[1][1] > a[2][2]) {
				float s = 2.0f * std::sqrt(1.0f + a[1][1] - a[0][0] - a[2][2]);
				w = (a[2][0] - a[0][2]) / s;
				x = (a[1][0] + a[0][1]) / s;
				y = 0.25f * s;
				z = (a[2][1] + a[1][2]) / s;
			} else {
				float s = 2.0f * std::sqrt(1.0f + a[2][2] - a[0][0] - a[1][1]);
				w = (a[0][1] - a[1][0] ) / s;
				x = (a[2][0] + a[0][2] ) / s;
				y = (a[1][2] + a[2][1] ) / s;
				z = 0.25f * s;
			}
		}

		float length = std::sqrt(x * x + y * y + z * z + w * w);
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	inline static Quaternion axisAngle(const Vector3& axis, float angle) {
		Quaternion q{};
		const float hs = std::sin(angle / 2.0f),
					hc = std::cos(angle / 2.0f);
		q.x = hs * axis.x;
		q.y = hs * axis.y;
		q.z = hs * axis.z;
		q.w = hc;
		return q;
	}

	inline static Quaternion lookAt(const Vector3& eye, const Vector3& at, const Vector3& up) {
		return Quaternion(Matrix4::lookAt(eye, at, up));
	}

	inline Vector2 toVector2() const { return Vector2(x, y); }
	inline Vector3 toVector3() const { return Vector3(x, y, z); }
	inline Vector4 toVector4() const { return Vector4(x, y, z, w); }

	inline Quaternion conjugated() const {
		return Quaternion(-x, -y, -z, w);
	}

	inline float dot(const Quaternion& q) const {
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}

	inline float length() const {
		return std::sqrt(this->dot(*this));
	}

	inline Quaternion normalized() const {
		float nlen = 1.0f / length();
		return (*this) * nlen;
	}

	inline Quaternion operator *(const Quaternion& r) const {
		float w_ = w * r.w - x * r.x - y * r.y - z * r.z;
		float x_ = x * r.w + w * r.x + y * r.z - z * r.y;
		float y_ = y * r.w + w * r.y + z * r.x - x * r.z;
		float z_ = z * r.w + w * r.z + x * r.y - y * r.x;
		return Quaternion(x_, y_, z_, w_);
	}

	inline Quaternion operator *(const Vector3& r) const {
		float w_ = -x * r.x - y * r.y - z * r.z;
		float x_ = w * r.x + y * r.z - z * r.y;
		float y_ = w * r.y + z * r.x - x * r.z;
		float z_ = w * r.z + x * r.y - y * r.x;
		return Quaternion(x_, y_, z_, w_);
	}

	inline Quaternion operator *(float r) const {
		return Quaternion(x * r, y * r, z * r, w * r);
	}

	inline Quaternion operator +(const Quaternion& r) const {
		return Quaternion(x + r.x, y + r.y, z + r.z, w + r.w);
	}
	
	inline Quaternion operator -(const Quaternion& r) const {
		return Quaternion(x - r.x, y - r.y, z - r.z, w - r.w);
	}

	inline Vector3 rotate(const Vector3& v) const {
		const Quaternion cq = conjugated();
		const Quaternion r = ((*this) * v) * cq;
		return r.toVector3();
	}

	inline Matrix4 toMatrix4() const {
		const Vector3 forward = Vector3(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
		const Vector3 up = Vector3(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
		const Vector3 right = Vector3(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

		return Matrix4::rotation(forward, up, right);
	}

	inline Quaternion nLerp(const Quaternion& to, float factor, bool shortest = true) {
		Quaternion correctTo = to;
		if (shortest && dot(to) < 0.0f) {
			correctTo = Quaternion(-to.x, -to.y, -to.z, -to.w);
		}
		return (correctTo - (*this) * factor + (*this)).normalized();
	}

	inline Quaternion sLerp(const Quaternion& to, float factor, bool shortest = true) {
		const float FACTOR = 1e-3f;

		float cos = dot(to);
		Quaternion correctTo = to;

		if (shortest && cos < 0.0f) {
			cos = -cos;
			correctTo = Quaternion(-to.x, -to.y, -to.z, -to.w);
		}

		if (std::abs(cos) >= 1.0f - FACTOR)
			return nLerp(to, factor, shortest);

		const float sin = std::sqrt(1.0f - cos * cos);
		const float angle = std::atan2(sin, cos);
		const float invSin = 1.0f / sin;

		const float srcFactor = std::sin((1.0f - factor) * angle) * invSin;
		const float destFactor = std::sin(factor * angle) * invSin;

		return (*this) * srcFactor + correctTo * destFactor;
	}

	float x, y, z, w;
};

inline std::ostream& operator<<(std::ostream& o, const Quaternion& v) {
	return o << "Quaternion(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}

class AABB {
public:
	Vector3 min{}, max{};

	AABB() = default;
	AABB(const Vector3& min, const Vector3& max)
		: min(min), max(max) {}
	
	// TODO: Implement AABB things
};

#endif // VEC_MATH_HPP