/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include <iostream>

namespace NCL
{
	namespace Maths
	{
		class Vector3;
		class Vector4;

		class Vector2
		{
		public:
			union
			{
				struct
				{
					float x;
					float y;
				};

				float array[2];
			};

		public:
			constexpr Vector2(void) : x(0.0f), y(0.0f)
			{
			}

			constexpr Vector2(float xVal, float yVal) : x(xVal), y(yVal)
			{
			}

			Vector2(const Vector3& v3);
			Vector2(const Vector4& v4);

			~Vector2(void)
			{
			}

			Vector2 Normalised() const
			{
				Vector2 temp(x, y);
				temp.Normalise();
				return temp;
			}

			void Normalise()
			{
				float length = Length();

				if (length != 0.0f)
				{
					length = 1.0f / length;
					x = x * length;
					y = y * length;
				}
			}

			float Length() const
			{
				return sqrt((x * x) + (y * y));
			}

			constexpr float LengthSquared() const
			{
				return ((x * x) + (y * y));
			}

			constexpr float GetMaxElement() const
			{
				float v = x;
				if (y > v)
				{
					v = y;
				}
				return v;
			}

			float GetAbsMaxElement() const
			{
				float ax = abs(x);
				float ay = abs(y);

				if (ax > ay)
				{
					return ax;
				}
				return ay;
			}

			const float* as_float_array() const;

			static constexpr float Dot(const Vector2& a, const Vector2& b)
			{
				return (a.x * b.x) + (a.y * b.y);
			}

			Vector2 operator+(const Vector2& a) const
			{
				return Vector2(x + a.x, y + a.y);
			}

			Vector2 operator-(const Vector2& a) const
			{
				return Vector2(x - a.x, y - a.y);
			}

			Vector2 operator-() const
			{
				return Vector2(-x, -y);
			}

			Vector2 operator*(float a) const
			{
				return Vector2(x * a, y * a);
			}

			Vector2 operator*(const Vector2& a) const
			{
				return Vector2(x * a.x, y * a.y);
			}

			Vector2 operator/(const Vector2& a) const
			{
				return Vector2(x / a.x, y / a.y);
			};

			Vector2 operator/(float v) const
			{
				return Vector2(x / v, y / v);
			};

			constexpr void operator+=(const Vector2& a)
			{
				x += a.x;
				y += a.y;
			}

			void operator-=(const Vector2& a)
			{
				x -= a.x;
				y -= a.y;
			}


			void operator*=(const Vector2& a)
			{
				x *= a.x;
				y *= a.y;
			}

			void operator/=(const Vector2& a)
			{
				x /= a.x;
				y /= a.y;
			}

			void operator*=(float f)
			{
				x *= f;
				y *= f;
			}

			void operator/=(float f)
			{
				x /= f;
				y /= f;
			}

			float operator[](int i) const
			{
				return array[i];
			}

			float& operator[](int i)
			{
				return array[i];
			}

			bool operator==(const Vector2& A) const { return (A.x == x && A.y == y) ? true : false; };
			bool operator!=(const Vector2& A) const { return (A.x == x && A.y == y) ? false : true; };

			friend std::ostream& operator<<(std::ostream& o, const Vector2& v)
			{
				o << "Vector2(" << v.x << "," << v.y << ")" << std::endl;
				return o;
			}
		};
	}
}
