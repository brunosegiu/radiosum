#pragma once
class Component
{
public:
	Component();
	virtual void render() = 0;
	void enable();
	virtual ~Component();
protected:
	bool enabled;
};

