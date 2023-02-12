#pragma once

#include <functional>

template<typename T>
class WrappedPointerTo {
public:

	typedef std::function<void()> DtorFunc;

	WrappedPointerTo(DtorFunc dtorFunc, T testValue = 0) :
		_dtorFunc(dtorFunc),
		_testValue(testValue)
	{}

	~WrappedPointerTo()
	{
		_dtorFunc();
	}

	T& getTestValue()
	{
		return _testValue;
	}

private:

	DtorFunc _dtorFunc;
	T _testValue;

};
