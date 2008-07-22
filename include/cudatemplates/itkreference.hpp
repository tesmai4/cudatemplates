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

#ifndef CUDA_ITK_REFERENCE_H
#define CUDA_ITK_REFERENCE_H


#include <itkImage.h>

#include <cudatemplates/hostmemoryreference.hpp>


#define CUDA_INIT_POINTER(args...) args


namespace Cuda {

/**
   Reference to existing ITK image.
*/
template <class Type, unsigned Dim>
class ItkReference: public HostMemoryReference<Type, Dim>
{
public:
  typedef typename itk::Image<Type, Dim> ImageType;

  /**
     Constructor.
     @param image ITK image to reference
  */
  inline ItkReference(typename ImageType::Pointer image):
    Layout<Type, Dim>(),
    Pointer<Type, Dim>(),
    HostMemoryReference<Type, Dim>()
  {
    setImage(image);
  }

  /**
     Constructor.
     This allocates memory for the ITK image of the given size.
     @param _size size of image
     @param image ITK image to reference
  */
  inline ItkReference(const Size<Dim> &_size, typename ImageType::Pointer image):
    Layout<Type, Dim>(),
    Pointer<Type, Dim>(),
    HostMemoryReference<Type, Dim>()
  {
    typename ImageType::IndexType istart;
    typename ImageType::SizeType isize;

    for(unsigned i = Dim; i--;) {
      istart[i] = 0;
      isize[i] = _size[i];
    }

    typename ImageType::RegionType region;
    region.SetSize(isize);
    region.SetIndex(istart);
    image->SetRegions(region);
    image->Allocate();
    setImage(image);
  }

private:
  /**
     Extract image information from ITK image.
     @param image ITK image
  */
  void setImage(typename ImageType::Pointer image)
  {
    typename ImageType::SizeType isize = image->GetLargestPossibleRegion().GetSize();
    typename ImageType::SpacingType ispacing = image->GetSpacing();

    for(unsigned i = Dim; i--;) {
      this->size[i] = isize[i];
      this->spacing[i] = ispacing[i];
    }

    this->buffer = image->GetBufferPointer();
    this->setPitch(0);
  }
};

CUDA_SPECIALIZE_DIM(ItkReference);

}


#undef CUDA_INIT_POINTER


#endif
