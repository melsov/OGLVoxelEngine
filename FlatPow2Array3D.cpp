//#include "FlatPow2Array3D.h"
//#include "MathHelper.cpp"
//
//using namespace glm;
//
//template<class T>
//FlatPow2Array3D<T>::FlatPow2Array3D()
//{
//	Init(0, 0, 0);
//}
//
//template<class T>
//FlatPow2Array3D<T>::FlatPow2Array3D(unsigned int logTwo)
//{
//	Init(logTwo, logTwo, logTwo);
//}
//
//template<class T>
//FlatPow2Array3D<T>::FlatPow2Array3D(unsigned int logX, unsigned int logY, unsigned int logZ)
//{
//	Init(logX, logY, logZ);
//}
//
//template<class T>
//FlatPow2Array3D<T>::~FlatPow2Array3D()
//{
//	delete [] storage;
//	storage = NULL;
//}
//
//template<class T>
//void FlatPow2Array3D<T>::Init(unsigned int logX, unsigned int logY, unsigned int logZ)
//{
//	logTwoDims = ivec3(logX, logY, logZ);
//	dimensions = ivec3(pow(2, logX), pow(2, logY), pow(2, logZ));
//	_size = (int)AreaXYZ(dimensions);
//	storage = new T[_size];
//}



