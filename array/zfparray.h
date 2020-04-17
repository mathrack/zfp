#ifndef ZFP_ARRAY_H
#define ZFP_ARRAY_H

#include <algorithm>
#include <climits>
#include <stdexcept>
#include <string>
#include "zfp.h"
#include "zfp/exception.h"
#include "zfp/header.h"

namespace zfp {

// abstract base class for compressed array of scalars
class array {
public:

  // factory function
  static zfp::array* construct(const zfp::header& header, const void* buffer = 0, size_t buffer_size_bytes = 0);

protected:
  // default constructor
  array() :
    type(zfp_type_none),
    dims(0),
    nx(0), ny(0), nz(0)
  {}

  // generic array with 'dims' dimensions and scalar type 'type'
  explicit array(uint dims, zfp_type type) :
    type(type),
    dims(dims),
    nx(0), ny(0), nz(0)
  {}

  // constructor from previously-serialized compressed array
  array(uint dims, zfp_type type, const zfp::header& header) :
    type(type), dims(dims),
    nx(header.size_x()), ny(header.size_y()), nz(header.size_z())
  {
    if (header.scalar_type() != type)
      throw zfp::exception("zfp array scalar type does not match header");
    if (header.dimensionality() != dims)
      throw zfp::exception("zfp array dimensionality does not match header");
  }

  // copy constructor--performs a deep copy
  array(const array& a)
  {
    deep_copy(a);
  }

  // assignment operator--performs a deep copy
  array& operator=(const array& a)
  {
    deep_copy(a);
    return *this;
  }

public:
  // public virtual destructor (can delete array through base class pointer)
  virtual ~array() {}

  // underlying scalar type
  zfp_type scalar_type() const { return type; }

  // dimensionality
  uint dimensionality() const { return dims; }

  // compressed data size and buffer
  virtual size_t compressed_size() const = 0;
  virtual void* compressed_data() const = 0;

  // header for array serialization
  virtual zfp::header* header() const = 0;

protected:
  // perform a deep copy
  void deep_copy(const array& a)
  {
    // copy metadata
    type = a.type;
    dims = a.dims;
    nx = a.nx;
    ny = a.ny;
    nz = a.nz;
  }

  zfp_type type;   // scalar type
  uint dims;       // array dimensionality (1, 2, or 3)
  uint nx, ny, nz; // array dimensions
};

}

#endif
