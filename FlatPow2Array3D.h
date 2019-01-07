#pragma once

#include <glm/glm.hpp>
#include <stdio.h>
#include <exception>

using namespace glm;

template<class T>
class FlatPow2Array3D
{
public:

	FlatPow2Array3D(unsigned int logOfTwo) { 
		Init(logOfTwo, logOfTwo, logOfTwo); 
	}

	FlatPow2Array3D(unsigned int logX, unsigned int logY, unsigned int logZ) {
		Init(logX, logY, logZ);
	}

	~FlatPow2Array3D() {
		printf("flat pow arr dtor: size: %d stor is nullptr %d stor addr: %x \n", _size, storage == nullptr, &storage);
		if (storage != nullptr)
		{
			delete[] storage;
		}
		storage = nullptr;
	}

#pragma region copy swap region

	//safest? are we violating the rule of the big three?
	FlatPow2Array3D(const FlatPow2Array3D& a) = delete;

	FlatPow2Array3D& operator=(FlatPow2Array3D other) = delete;

	/*
	friend void swap(FlatPow2Array3D& a, FlatPow2Array3D& b)
	{
		using std::swap;

		swap(a.dimensions, b.dimensions);
		swap(a._size, b._size);
		swap(a.logTwoDims, b.logTwoDims);
		swap(a.data, b.data);
	}

	FlatPow2Array3D(const FlatPow2Array3D& a)
	{
		//copy all of the data??
	}

	FlatPow2Array3D& operator=(FlatPow2Array3D other)
	{
		swap(*this, other);
		return *this;
	}
	*/
#pragma endregion

#pragma region access

	T& operator[] (int i) {
		return storage[i];
	}

	T& get (int x, int y, int z) {
		return storage[IndexFor(x, y, z)];
	}

	T& get(glm::ivec3 v) { return get(v.x, v.y, v.z); }
	
	void set(int x, int y, int z, T val) {
		storage[IndexFor(x, y, z)] = val;
	}

	void set(glm::ivec3 v, T val) {
		set(v.x, v.y, v.z, val);
	}

	unsigned int size() { return _size; }

#pragma endregion

	void debug()
	{
		printf("flat array 3d: size: %d . stor addr: %x \n", _size, &storage);
	}
	

private:
	void Init(unsigned int logX, unsigned int logY, unsigned int logZ) {
		logTwoDims = ivec3(logX, logY, logZ);
		dimensions = ivec3(pow(2, logX), pow(2, logY), pow(2, logZ));
		if (logX > 0 && logY > 0 && logZ > 0)
		{
			_size = (int)(dimensions.x * dimensions.y * dimensions.z);
			storage = new T[_size];
		}
		else
		{
			_size = 0;
			storage = nullptr;
			printf("stor is a nullptr \n");
		}
	}

	int IndexFor(int x, int y, int z) {
		return (x << (logTwoDims.y + logTwoDims.z)) + (y << logTwoDims.z) + z;
	}

	T * storage;

	glm::ivec3 dimensions;
	glm::ivec3 logTwoDims;
	unsigned int _size;

};




