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

/*
#include <iostream>
#include <stdexcept>

#include <cuda.h>
#include <cutil.h>
*/

#include <cudatemplates/opengl/bufferobject.hpp>

using namespace std;


int
main()
{
  Cuda::Size<2> size(256, 256);
  Cuda::OpenGL::BufferObject<float, 2> buf(size);
}
