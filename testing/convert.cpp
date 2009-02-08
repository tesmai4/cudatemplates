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

#include <cstdlib>
#include <iostream>

#include <cudatemplates/hostmemoryheap.hpp>
#include <cudatemplates/devicememorylinear.hpp>

#include <cudatemplates/convert.hpp>


using namespace std;


int
main()
{
  const int SIZE = 10;
  const Cuda::Size<1> size(SIZE);

  Cuda::HostMemoryHeap<float, 1> obj1(size);

  for(int i = SIZE; i--;)
    obj1[i] = random() / 65536.0;

  Cuda::HostMemoryHeap<double, 1> obj2(obj1);

  Cuda::DeviceMemoryLinear<float, 1> obj3(obj1);
  Cuda::DeviceMemoryLinear<int, 1> obj4(size);
  Cuda::HostMemoryHeap<int, 1> obj5(size);
  copy(obj4, obj3);
  copy(obj5, obj4);

  for(int i = 0; i < SIZE; ++i)
    cout << i << ": " << obj1[i] << ' ' << obj5[i] << endl;

  return 0;
}
