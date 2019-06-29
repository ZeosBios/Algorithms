#include <cstdlib>
#include <cstdio>
#include <cmath>

static const double EPS = 1e-7;

struct vector3
{
	double x, y, z;

	inline vector3(double a, double b, double c)
	{
		x = a;
		y = b;
		z = c;
	}

	inline vector3(const vector3 &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	vector3 set(double a, double b, double c)
	{
		x = a;
		y = b;
		z = c;
	}

	double norm()
	{
		return (double)sqrt(x * x + y * y + z * z);
	}

	const vector3 operator *(const double &scalar)
	{
		return vector3(x * scalar, y * scalar, z * scalar);
	}
};

struct matrix33
{
	double m[3][3];
};

class quaternion
{
public:

	double x, y, z, w;

public:

	inline quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {};
	inline quaternion(double x_, double y_, double z_, double w_) : x(x_), y(y_), z(z_), w(w_) {};
	
	//	создание кватерниона
	inline void set(double x_, double y_, double z_, double w_) 
	{
		x = x_; 
		y = y_; 
		z = z_; 
		w = w_;
	};

	inline void set(const quaternion& q) 
	{
		x = q.x; 
		y = q.y; 
		z = q.z; 
		w = q.w;
	};

	//	основные операции

	//	единичный кватернион (нулевое вращение)
	inline void ident(void) 
	{
		x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f;
	};

	//	алгебраическое дополнение
	inline void conjugate(void) {
		x = -x; y = -y; z = -z;
	};

	inline quaternion conjugated(void) const 
	{
		return quaternion(-x, -y, -z, w);
	}

	//	умножение на скаляр
	inline void scale(double s) 
	{
		x *= s; y *= s; z *= s; w *= s;
	};

	//	квадрат нормы кватерниона
	inline double norm(void) const 
	{
		return x*x + y*y + z*z + w*w;
	};

	//	норма кватерниона
	inline double magnitude(void) const 
	{
		return (double)sqrt(norm());
	};

	//	обратный кватернион
	inline quaternion inverted(void) const 
	{
		double in = 1.0 / norm();
		return quaternion(-x*in, -y*in, -z*in, w*in);
	}

	inline void invert(void) 
	{
		double in = 1.0f / norm();
		set(-x*in, -y*in, -z*in, w*in);
	}

	/*
	стабилизация кватерниона
	(эта операция работает быстрее нормализации, и предотвращает уход длины кватерниона в 0 или бесконечность)
	*/
	inline void stabilize_length() 
	{
		double cs = (double)(fabs(x) + fabs(y) + fabs(z) + fabs(w));
		if (cs > 0.0f)
			set(x /= cs, y /= cs, z /= cs, w /= cs);
		else
			ident();
	}

	/*
	нормализация кватерниона
	(умножение на такой скаляр, что длина кватерниона становится равной 1)
	*/
	inline void normalize(void) 
	{
		double m = magnitude();
		if (m < EPS)
		{
			stabilize_length();
			m = magnitude();
		}
		scale(1.0f / m);
	}

	//	операторы

	inline quaternion operator-()const 
	{
		return quaternion(-x, -y, -z, -w);
	}

	inline quaternion operator+(const quaternion& q)const 
	{
		return quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
	}

	inline quaternion operator-(const quaternion& q)const 
	{
		return quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
	}

	inline quaternion operator*(const double& s)const 
	{
		return quaternion(x*s, y*s, z*s, w*s);
	}

	inline quaternion operator*(const quaternion& q)const 
	{
		return quaternion(w*q.x + x*q.w + y*q.z - z*q.y,
			w*q.y + y*q.w + z*q.x - x*q.z,
			w*q.z + z*q.w + x*q.y - y*q.x,
			w*q.w - x*q.x - y*q.y - z*q.z);
	}

	inline const quaternion& operator+=(const quaternion& q) 
	{
		x += q.x; y += q.y; z += q.z; w += q.w;
		return *this;
	};

	inline const quaternion& operator-=(const quaternion& q) 
	{
		x -= q.x; y -= q.y; z -= q.z; w -= q.w;
		return *this;
	};

	inline const quaternion& operator*=(const quaternion& q) 
	{
		set((*this)*q);
	};

	//	методы

	//	получить угол поворота от -PI до PI
	inline double get_rotation()const 
	{
		if (w < 0)
			return 2.0f * (double)atan2(-sqrt(x * x + y * y + z * z), -w);
		else
			return 2.0f * (double)atan2(sqrt(x * x + y * y + z * z), w);
	}

	//	поворот вектора кватернионом
	inline vector3 rotate(const vector3& v)const 
	{
		quaternion q(v.x * w + v.z * y - v.y * z,
			v.y * w + v.x * z - v.z * x,
			v.z * w + v.y * x - v.x * y,
			v.x * x + v.y * y + v.z * z);

		return vector3(w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y + y * q.w + z * q.x - x * q.z,
			w * q.z + z * q.w + x * q.y - y * q.x) * (1. / norm());
	};
	
	//	получение кватерниона единичной длины из векторного представления угла поворота (вектор + угол)
	inline void unit_from_axis_angle(const vector3& axis, const double& angle) 
	{
		vector3 v(axis);
		v.norm();
		double half_angle = angle * 0.5;
		double sin_a = sin(half_angle);
		set(v.x * sin_a, v.y * sin_a, v.z * sin_a, cos(half_angle));
	};
	
	//	получить векторное представление угла поворота из кватерниона
	//	сохранить направление вектора, угол в пределах от -PI до PI
	inline void to_axis_angle(vector3& axis, double& angle)const 
	{
		double vl = sqrt(x*x + y*y + z*z);
		if (vl > EPS)
		{
			double ivl = 1.0f / vl;
			axis.set(x*ivl, y*ivl, z*ivl);
			if (w < 0)
				angle = 2.0f * (double)atan2(-vl, -w); //	(-PI, 0)
			else
				angle = 2.0f * (double)atan2(vl, w); //		(0, PI) 
		}
		else 
		{
			axis = vector3(0, 0, 0);
			angle = 0;
		}
	};
	
	//	создание кватерниона единичной длины из матрицы поворота
	//	матрица должна содержать только поворот, а не растяжение или сжатие
	//	для стабильности решения сначала находится наибольший компонент кватерниона,
	//	а все остальные выражаются через него
	inline void unit_from_matrix(const matrix33& mtx)
	{
		double m[3][3];
		m[0][0] = mtx.m[0][0];	m[0][1] = mtx.m[0][1];	m[0][2] = mtx.m[0][2];
		m[1][0] = mtx.m[1][0];	m[1][1] = mtx.m[1][1];	m[1][2] = mtx.m[1][2];
		m[2][0] = mtx.m[0][0];	m[2][1] = mtx.m[2][1];	m[2][2] = mtx.m[2][2];
		double tr = m[0][0] + m[1][1] + m[2][2];
		if (tr > 0.0f) 
		{                         // наибольший компонент кватерниона = W
			set(m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0], tr + 1.0f);
			scale(0.5f / (double)sqrt(w));
		}
		else                                   //	наибольшей является один из векторных компонентов
			if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) 
			{
				set(1.0f + m[0][0] - m[1][1] - m[2][2], m[1][0] + m[0][1], m[2][0] + m[0][2], m[1][2] - m[2][1]);
				scale(0.5f / (double)sqrt(x));
			}
			else
				if (m[1][1] > m[2][2]) 
				{
					set(m[1][0] + m[0][1], 1.0f + m[1][1] - m[0][0] - m[2][2], m[2][1] + m[1][2], m[2][0] - m[0][2]);
					scale(0.5f / (double)sqrt(y));
				}
				else 
				{
					set(m[2][0] + m[0][2], m[2][1] + m[1][2], 1.0f + m[2][2] - m[0][0] - m[1][1], m[0][1] - m[1][0]);
					scale(0.5f / (double)sqrt(z));
				}
	}
	
	//	получение кватерниона не-единичной длины из матрицы поворота
	//	матрица должна содержать только поворот (без растяжения или сжатия)
	inline void from_matrix(const matrix33& mtx)
	{
		double m[3][3];
		m[0][0] = mtx.m[0][0];	m[0][1] = mtx.m[0][1];	m[0][2] = mtx.m[0][2];
		m[1][0] = mtx.m[1][0];	m[1][1] = mtx.m[1][1];	m[1][2] = mtx.m[1][2];
		m[2][0] = mtx.m[0][0];	m[2][1] = mtx.m[2][1];	m[2][2] = mtx.m[2][2];
		double tr = m[0][0] + m[1][1] + m[2][2];
		if (tr > 0.0f) 
		{                         // наибольший компонент кватерниона = W
			set(m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0], tr + 1.0f);
		}
		else                                   // наибольшей является один из векторных компонентов
			if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2])) 
			{
				set(1.0f + m[0][0] - m[1][1] - m[2][2], m[1][0] + m[0][1], m[2][0] + m[0][2], m[1][2] - m[2][1]);
			}
			else
				if (m[1][1] > m[2][2]) 
				{
					set(m[1][0] + m[0][1], 1.0f + m[1][1] - m[0][0] - m[2][2], m[2][1] + m[1][2], m[2][0] - m[0][2]);
				}
				else 
				{
					set(m[2][0] + m[0][2], m[2][1] + m[1][2], 1.0f + m[2][2] - m[0][0] - m[1][1], m[0][1] - m[1][0]);
				}
	}

	//	получение матрицы поворота из кватерниона
	inline void to_matrix(matrix33& m)const 
	{
		double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
		double s = 2.0f / norm();
		x2 = x * s;    y2 = y * s;    z2 = z * s;
		xx = x * x2;   xy = x * y2;   xz = x * z2;
		yy = y * y2;   yz = y * z2;   zz = z * z2;
		wx = w * x2;   wy = w * y2;   wz = w * z2;

		m.m[0][0] = 1.0f - (yy + zz);
		m.m[1][0] = xy - wz;
		m.m[2][0] = xz + wy;

		m.m[0][1] = xy + wz;
		m.m[1][1] = 1.0f - (xx + zz);
		m.m[2][1] = yz - wx;

		m.m[0][2] = xz - wy;
		m.m[1][2] = yz + wx;
		m.m[2][2] = 1.0f - (xx + yy);
	}

	//	представление кватерниона длины 1 в виде матрицы поворота
	inline void unit_to_matrix(matrix33& m)const 
	{
		double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
		x2 = x + x;    y2 = y + y;    z2 = z + z;
		xx = x * x2;   xy = x * y2;   xz = x * z2;
		yy = y * y2;   yz = y * z2;   zz = z * z2;
		wx = w * x2;   wy = w * y2;   wz = w * z2;

		m.m[0][0] = 1.0f - (yy + zz);
		m.m[1][0] = xy - wz;
		m.m[2][0] = xz + wy;

		m.m[0][1] = xy + wz;
		m.m[1][1] = 1.0f - (xx + zz);
		m.m[2][1] = yz - wx;

		m.m[0][2] = xz - wy;
		m.m[1][2] = yz + wx;
		m.m[2][2] = 1.0f - (xx + yy);
	}
};

//	скалярное произведение кватернионов
static inline double inner_product(const quaternion& q0, const quaternion& q1) 
{
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

//	квадратный корень
static inline quaternion sqrt(const quaternion& q) 
{
	double m = q.magnitude();
	double w = q.w + m;
	return quaternion(q.x, q.y, q.z, w) * (double)sqrt(0.5 / w);
}

int main(int argc, char *argv[])
{
	return EXIT_SUCCESS;
}