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
#include <string>

using namespace std;


const char coord[] = { 0, 'x', 'y', 'z', 'w' };


int
main()
{
  cout <<
    "/*\n"
    "  NOTE: THIS FILE HAS BEEN CREATED AUTOMATICALLY,\n"
    "  ANY CHANGES WILL BE OVERWRITTEN WITHOUT NOTICE!\n"
    "*/\n"
    "\n"
    "/* \n"
    "  Cuda Templates.\n"
    "\n"
    "  Copyright (C) 2008 Institute for Computer Graphics and Vision,\n"
    "                     Graz University of Technology\n"
    "  \n"
    "  This program is free software; you can redistribute it and/or modify\n"
    "  it under the terms of the GNU General Public License as published by\n"
    "  the Free Software Foundation; either version 3 of the License, or\n"
    "  (at your option) any later version.\n"
    "  \n"
    "  This program is distributed in the hope that it will be useful,\n"
    "  but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "  GNU General Public License for more details.\n"
    "  \n"
    "  You should have received a copy of the GNU General Public License\n"
    "  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
    "*/\n"
    "\n"
    "#ifndef CUDA_PACK_AUTO_H\n"
    "#define CUDA_PACK_AUTO_H\n"
    "\n"
    "\n";

  // template kernels:
  for(int dim_data = 1; dim_data <= 3; ++dim_data) {
    for(int dim_vec = 2; dim_vec <= 4; ++dim_vec) {
      // function header:
      cout << "template <class VectorType, class ScalarType>\n__global__ void\npack_nocheck_kernel(VectorType *dst";

      for(int i = 1; i <= dim_vec; ++i)
	cout << ", const ScalarType *src" << i;

      cout <<
	",\n                    "
	"Cuda::Size<" << dim_data << "> dst_size, "
	"Cuda::Size<" << dim_data << "> dst_stride, "
	"Cuda::Size<" << dim_data << "> src_size, "
	"Cuda::Size<" << dim_data << "> src_stride)\n{\n";

      // compute coordinates:
      for(int i = 1; i <= dim_data; ++i) {
	char c = coord[i];
	cout << "  int " << c << " = threadIdx." << c << " + blockIdx." << c << " * blockDim." << c << ";\n";
      }

      // compute destination offset:
      cout << "  int dst_ofs = x";

      for(int i = 2; i <= dim_data; ++i)
	cout << " + " << coord[i] << " * dst_stride[" << (i - 2) << "]";
      
      cout << ";\n";

      // compute source offset:
      cout << "  int src_ofs = x";

      for(int i = 2; i <= dim_data; ++i)
	cout << " + " << coord[i] << " * src_stride[" << (i - 2) << "]";

      cout << ";\n";

      // read data:
      cout << "  VectorType vec;\n";

      for(int i = 1; i <= dim_vec; ++i)
	cout << "  vec." << coord[i] << " = src" << i << "[src_ofs];\n";

      // write data:
      cout << "  dst[dst_ofs] = vec;\n";

      cout << "}\n\n";
    }
  }

  // template functions:
  cout <<
    "namespace Cuda {\n"
    "\n";

  for(int dim_vec = 2; dim_vec <= 4; ++dim_vec) {
    cout <<
      "template<class VectorType, class ScalarType, unsigned Dim>\n"
      "void\n"
      "pack(DeviceMemory<VectorType, Dim> &dst";

    for(int i = 1; i <= dim_vec; ++i)
      cout << ",\n     const DeviceMemory<ScalarType, Dim> &src" << i;

    cout <<
      ")\n"
      "{\n"
      "  // TODO: size check\n"
      "  Size<Dim> dst_ofs, size(dst.size);\n"
      "  // dst.checkBounds(dst_ofs, size);\n"
      "  dim3 gridDim, blockDim;\n"
      "  bool aligned;\n"
      "  size_t dofs;\n"
      "  Size<Dim> rmin, rmax;\n"
      "  dst.getExecutionConfiguration(gridDim, blockDim, aligned, dofs, rmin, rmax, dst_ofs, size);\n"
      "  // typename DeviceMemory<Type, Dim>::KernelData kdst(dst);\n"
      "  // kdst.data += dofs;\n"
      "\n"
      "  if(aligned)\n"
      "    pack_nocheck_kernel<<<gridDim, blockDim>>>(dst.getBuffer(), ";

    for(int i = 1; i <= dim_vec; ++i)
      cout << "src" << i << ".getBuffer(), ";

    cout << "dst.size, dst.stride, src1.size, src1.stride);\n";

    cout <<
      "  else\n"
      "    abort();  // pack_check_kernel<<<gridDim, blockDim>>>(kdst, val, rmin, rmax);\n"
      "\n"
      "  CUDA_CHECK_LAST;\n"
      "}\n\n";
  }

  cout <<
    "}  // namespace Cuda\n"
    "\n"
    "\n"
    "#endif\n";

  return 0;
}
