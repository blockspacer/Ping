#pragma once
#include <cstdlib>
#include <cmath>
#include <random>
#include <SFML/System/Vector2.hpp>
typedef sf::Vector2f Vector2;

/// Common math functions that are used throughout the program.
namespace PingMath
{
	// Math constants
	const float Pi = 3.14159f;
	const float PiOver2 = 1.5708f;
	const float PiOver4 = 0.785398f;
	const float TwoPi = 6.28319f;

	/// Clamp the given value.
	/// @param	value	The value.
	/// @param	min  	The minimum.
	/// @param	max  	The maximum.
	/// @return	max if value > max, min if value < min, value otherwise
	inline float clamp(float value, float min, float max)
	{
		return value < min ? min : (value > max ? max : value);
	}

	/// Clamp the given value.
	/// @param	value	The value.
	/// @param	min  	The minimum.
	/// @param	max  	The maximum.
	/// @return	max if value > max, min if value < min, value otherwise.
	inline int clamp(int value, int min, int max)
	{
		return value < min ? min : (value > max ? max : value);
	}

	/// Determine whether the given value is between two other values.
	/// @param	value	The value.
	/// @param	low  	The low.
	/// @param	high 	The high.
	/// @return	true if it is between the low and high, false otherwise.
	inline bool between(float value, float low, float high)
	{
		return value <= high && value >= low;
	}

	/// Determine whether the given value is between two other values.
	/// @param	value	The value.
	/// @param	low  	The low.
	/// @param	high 	The high.
	/// @return	true if it is between the low and high, false otherwise.
	inline bool between(int value, int low, int high)
	{
		return value <= high && value >= low;
	}

	/// Generate a random number between (inclusive) two values.
	/// @param	min	The minimum number.
	/// @param	max	The maximum number.
	/// @return	The random number generated.
	inline float random(float min, float max)
	{
        static std::minstd_rand engine(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
		return dist(engine);
	}

	/// Normalizes the given Vector to a unit vector.
	/// @param [in,out]	v	The Vector2 to process.
	/// @return	The magnitude of the vector, prior to normalization.
	inline float normalize(Vector2 &v)
	{
		float length = sqrt(v.x * v.x + v.y * v.y);
		if (length < 0.0001) {
			return 0.0f;
		}
		float invLength = 1.0f / length;
		v.x *= invLength;
		v.y *= invLength;

		return length;
	}
};

