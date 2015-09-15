#pragma once

/// An interface for objects that should receive updates on every tick.
class IUpdateable
{
public:

	/// Update the object.
	/// @param	deltaTime	Seconds since last update.
	virtual void update(float deltaTime) = 0;
};