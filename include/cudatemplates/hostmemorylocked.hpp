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

#ifndef CUDA_HOSTMEMORYLOCKED_H
#define CUDA_HOSTMEMORYLOCKED_H


#include <cuda_runtime.h>

#include <cudatemplates/error.hpp>
#include <cudatemplates/hostmemory.hpp>


#define CUDA_INIT_POINTER(args...) args


namespace Cuda {

/**
   Representation of page-locked CPU memory.
   This can be used for DMA transfers between CPU and GPU memory.
*/
template <class Type, unsigned Dim>
class HostMemoryLocked: public HostMemoryStorage<Type, Dim>
{
public:
#ifndef CUDA_NO_DEFAULT_CONSTRUCTORS
  /**
     Default constructor.
  */
  inline HostMemoryLocked()
  {
  }
#endif

  /**
     Constructor.
     @param _size requested size of memory block
  */
  inline HostMemoryLocked(const Size<Dim> &_size):
    Layout<Type, Dim>(_size),
    Pointer<Type, Dim>(_size),
    HostMemoryStorage<Type, Dim>(_size)
  {
    alloc();
  }

  /**
     Constructor.
     @param layout requested layout of memory block
  */
  inline HostMemoryLocked(const Layout<Type, Dim> &layout):
    Layout<Type, Dim>(layout),
    Pointer<Type, Dim>(layout),
    HostMemoryStorage<Type, Dim>(layout)
  {
    alloc();
  }

  CUDA_COPY_CONSTRUCTOR(HostMemoryLocked, HostMemoryStorage)
  
  /**
     Destructor.
  */
  ~HostMemoryLocked()
  {
    free();
  }

  /**
     Allocate page-locked CPU memory.
  */
  void alloc();

  /**
     Free page-locked CPU memory.
  */
  void free();
};

template <class Type, unsigned Dim>
void HostMemoryLocked<Type, Dim>::
alloc()
{
  this->setPitch(0);
  CUDA_CHECK(cudaMallocHost((void **)&this->buffer, this->getSize() * sizeof(Type)));
}

template <class Type, unsigned Dim>
void HostMemoryLocked<Type, Dim>::
free()
{
  if(this->buffer == 0)
    return;

  CUDA_CHECK(cudaFreeHost(this->buffer));
  this->buffer = 0;
}

CUDA_SPECIALIZE_DIM(HostMemoryLocked)

}


#undef CUDA_INIT_POINTER


#endif
