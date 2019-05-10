#pragma once

#include <stdint.h>

template<typename T> struct Coordinates {
  Coordinates(T _x, T _y) : x(_x), y(_y) {}
  T x;
  T y;
};

struct VirtualCoordinates: Coordinates<uint8_t> {
  VirtualCoordinates(uint8_t _x, uint8_t _y) : Coordinates(_x, _y) {}
};

struct PhysicalCoordinates: Coordinates<uint8_t> {
  PhysicalCoordinates(uint8_t _x, uint8_t _y) : Coordinates(_x, _y) {}
};

template<typename T> struct Vector3d {
  Vector3d(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
  Vector3d() : Vector3d(0, 0, 0) {}
  T x;
  T y;
  T z;

  T magnitude() const {
      return abs(max(x, max(y, z)));
  }

  Vector3d<T> operator-(const Vector3d<T>& other) const {
    return Vector3d<T>(x - other.x, y - other.y, z - other.z);
  }

  Vector3d<T> absolute() const {
    return Vector3d<T>(abs(x), abs(y), abs(z));
  }
};

typedef Vector3d<uint8_t> Vec3;
