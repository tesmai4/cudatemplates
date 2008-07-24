/* 
  Cuda Templates.

  Copyright (C) 2008 Institute for Computer Graphics and Vision,
                     Graz University of Technology
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CUDA_ARRAY_H
#define CUDA_ARRAY_H


#include <cudatemplates/error.hpp>
#include <cudatemplates/staticassert.hpp>
#include <cudatemplates/storage.hpp>


#define CUDA_INIT_POINTER(...)


namespace Cuda {

/**
   Representation of CUDA array.
   CUDA arrays allow access to texture hardware.
*/
template <class Type, unsigned Dim>
class Array: public Storage<Type, Dim>
{
public:
#ifndef CUDA_NO_DEFAULT_CONSTRUCTORS
  /**
     Default constructor.
  */
  inline Array():
    array(0)
  {
  }
#endif

  /**
     Constructor.
     @param _size requested size of array
  */
  inline Array(const Size<Dim> &_size):
    Layout<Type, Dim>(_size),
    Storage<Type, Dim>(_size),
    array(0)
  {
    alloc();
  }

  /**
     Constructor.
     @param layout requested layout of array
  */
  inline Array(const Layout<Type, Dim> &layout):
    Layout<Type, Dim>(layout),
    Storage<Type, Dim>(layout),
    array(0)
  {
    alloc();
  }

  CUDA_COPY_CONSTRUCTOR(Array, Storage)

  /**
     Destructor.
  */
  ~Array()
  {
    free();
  }

  /**
     Allocate GPU memory.
  */
  void alloc();

  /**
     Free GPU memory.
  */
  void free();

  /**
     Get pointer to CUDA array structure.
     @return pointer to cudaArray
  */
  inline cudaArray *getArray() { return array; }

  /**
     Get pointer to CUDA array structure.
     @return pointer to cudaArray
  */
  inline const cudaArray *getArray() const { return array; }

  /**
     Initialize CUDA array pointer.
  */
  inline void init() { array = 0; }

private:
  cudaArray *array;
};

template <class Type, unsigned Dim>
void Array<Type, Dim>::
alloc()
{
  this->free();

  cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<Type>();
  cudaExtent extent;
  extent.width = this->size[0];

  if(Dim >= 2) {
    extent.height = this->size[1];

    if(Dim >= 3) {
      extent.depth = this->size[2];

      // map 4- and more-dimensional data sets to 3D data:
      for(unsigned i = 3; i < Dim; ++i)
	extent.depth *= this->size[i];
    }
  }

  if(Dim < 3) {
    extent.depth = 0;

    if(Dim < 2)
      extent.height = 0;
  }

  CUDA_CHECK(cudaMalloc3DArray(&array, &channelDesc, extent));
}

template <class Type, unsigned Dim>
void Array<Type, Dim>::
free()
{
  if(array == 0)
    return;
  
  CUDA_CHECK(cudaFreeArray(array));
  array = 0;
}

CUDA_SPECIALIZE_DIM(Array)

}


#undef CUDA_INIT_POINTER


#endif
