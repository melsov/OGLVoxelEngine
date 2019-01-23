#pragma once

#include <glm/glm.hpp>
#include <stdio.h>
#include <iostream>
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
		printf("flat pow arr dtor: size: %d stor is nullptr %d \n", _size, storage == nullptr);
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

	// queasy feeling
	void TakePointer(T* _stor, unsigned int log2)
	{
		if (storage != nullptr) { delete[] storage; }

		SetDims(log2, log2, log2);
		storage = _stor;
	}

	void ResetStorageEmpty(unsigned int log2)
	{
		if (storage != nullptr) { delete[] storage; }

		Init(log2, log2, log2);
	}

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

	T& get(int i) const
	{
		return storage[i];
	}

	T& get(glm::ivec3 v) { return get(v.x, v.y, v.z); }
	
	void set(int x, int y, int z, T val) {
		storage[IndexFor(x, y, z)] = val;
	}

	void set(glm::ivec3 v, T val) {
		set(v.x, v.y, v.z, val);
	}

	unsigned int size() const { return _size; }

	T* data() { return storage; }

#pragma endregion

	bool deepEquals(const FlatPow2Array3D<T>& other) const
	{
		if (size() != other.size()) { 
			std::cout << "size different this: " << size() << "other: " << other.size() << std::endl;
			return false; 
		}
		for (int i = 0; i < size(); ++i)
		{
			if (get(i).type != other.get(i).type)
			{
				std::cout << "dif types: this: " << get(i).type << " other: " << other.get(i).type << std::endl;
				return false;
			}
		}
		return true;
	}

	bool hasNonEmpty() const
	{
		for (int i = 0; i < size(); ++i)
		{
			if (get(i).type > 0 )
			{
				if (get(i).type <= 4)
				{
					return true;
				} 
				std::cout << "found type: " << get(i).type << std::endl;
				return false;
			}
		}
		return false;
	}

	void debug()
	{
		printf("flat array 3d: size: %d . stor addr: %x. non empty: %d \n", _size, &storage, hasNonEmpty());
	}
	

private:
	void SetDims(unsigned int logX, unsigned int logY, unsigned int logZ)
	{
		logTwoDims = ivec3(logX, logY, logZ);
		dimensions = ivec3(pow(2, logX), pow(2, logY), pow(2, logZ));
		if (logX > 0 && logY > 0 && logZ > 0)
		{
			_size = (int)(dimensions.x * dimensions.y * dimensions.z);
		}
		else
		{
			_size = 0;
		}
	}

	void Init(unsigned int logX, unsigned int logY, unsigned int logZ) {
		SetDims(logX, logY, logZ);
		/*logTwoDims = ivec3(logX, logY, logZ);
		dimensions = ivec3(pow(2, logX), pow(2, logY), pow(2, logZ));*/
		if (logX > 0 && logY > 0 && logZ > 0)
		{
			//_size = (int)(dimensions.x * dimensions.y * dimensions.z);
			storage = new T[_size];
		}
		else
		{
			//_size = 0;
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




