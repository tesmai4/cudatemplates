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

#include <iostream>

#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <cudatemplates/copy.hpp>
#include <cudatemplates/itkreference.hpp>
#include <cudatemplates/devicememorylinear.hpp>

using namespace std;


int
main()
{
  try {
    // prepare some ITK data types:
    typedef unsigned char PixelType;
    typedef itk::Image<PixelType, 2> ImageType;
    typedef itk::ImageFileReader<ImageType> ReaderType;
    typedef itk::ImageFileWriter<ImageType> WriterType;

    // read input image:
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName("cameraman.png");
    reader->Update();
    ImageType::Pointer image_input = reader->GetOutput();

    // create reference to ITK input image for use with CUDA classes:
    Cuda::ItkReference<PixelType, 2> ref_image_input(image_input);

    // create empty ITK output image:
    ImageType::Pointer image_output = ImageType::New();

    // create reference to ITK output image and initialize with same size as input image:
    Cuda::ItkReference<PixelType, 2> ref_image_output(ref_image_input.size, image_output);

    // copy input image to GPU and back to output image (i.e., CPU/GPU roundtrip):
    Cuda::DeviceMemoryLinear<PixelType, 2> tmp(ref_image_input.size);
    Cuda::copy(tmp, ref_image_input);
    Cuda::copy(ref_image_output, tmp);

    // write output image:
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName("cameraman.jpg");
    writer->SetInput(image_output);
    writer->Update();

    return 0;
  }
  catch(const exception &e) {
    cerr << e.what() << endl;
    return 1;
  }
}
