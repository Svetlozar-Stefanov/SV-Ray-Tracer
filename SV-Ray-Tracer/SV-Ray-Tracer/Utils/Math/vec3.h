#pragma once

#include <iostream>
#include <cmath>

class vec3
{
public:
	vec3() : e{ 0, 0, 0 } {}
	vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const float t) {
        return *this *= 1 / t;
    }

    float length() const
    {
        return std::sqrt(sqrd_len());
    }

    float sqrd_len() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    inline static vec3 random() {
        return vec3(rand_float(), rand_float(), rand_float());
    }

    inline static vec3 random(float min, float max) {
        return vec3(rand_float(min, max), rand_float(min, max), rand_float(min, max));
    }

    bool near_zero() const {
        const auto s = 1e-8;
        return fabs(e[0] < s) && fabs(e[1] < s) && fabs(e[2] < s);
    }

public:
	float e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(float t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, float t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline float dot(const vec3& v1, const vec3& v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
    return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
        v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
        v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

vec3 random_in_unit_sphere() {
    while (true)
    {
        vec3 p = vec3::random(-1, 1);
        if (p.sqrd_len() >= 1)
        {
            continue;
        }
        return p;
    }
}

vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

vec3 random_in_unit_disk() {
    while (true)
    {
        auto p = vec3(rand_float(-1, 1), rand_float(-1, 1), 0);
        if (p.sqrd_len() >= 1)
        {
            continue;
        }
        return p;
    }
}

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_parallel = -sqrt(fabs(1.0 - r_perp.sqrd_len()))* n;
    return r_perp + r_parallel;
}

using point = vec3;
using color = vec3;

