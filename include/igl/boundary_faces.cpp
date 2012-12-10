#include "boundary_faces.h"
#include "face_occurences.h"

// IGL includes
#include "sort.h"

// STL includes
#include <map>
#include <iostream>

template <typename IntegerT, typename IntegerF>
IGL_INLINE void igl::boundary_faces(
  const std::vector<std::vector<IntegerT> > & T,
  std::vector<std::vector<IntegerF> > & F)
{
  using namespace std;
  using namespace igl;

  if(T.size() == 0)
  {
    F.clear();
    return;
  }

  int simplex_size = T[0].size();
  // Get a list of all faces
  vector<vector<IntegerF> > allF(
    T.size()*simplex_size,
    vector<IntegerF>(simplex_size-1));

  // Gather faces, loop over tets
  for(int i = 0; i< (int)T.size();i++)
  {
    assert(T[i].size() == simplex_size);
    switch(simplex_size)
    {
      case 4:
        // get face in correct order
        allF[i*simplex_size+0][0] = T[i][1];
        allF[i*simplex_size+0][1] = T[i][3];
        allF[i*simplex_size+0][2] = T[i][2];
        // get face in correct order
        allF[i*simplex_size+1][0] = T[i][0];
        allF[i*simplex_size+1][1] = T[i][2];
        allF[i*simplex_size+1][2] = T[i][3];
        // get face in correct order
        allF[i*simplex_size+2][0] = T[i][0];
        allF[i*simplex_size+2][1] = T[i][3];
        allF[i*simplex_size+2][2] = T[i][1];
        // get face in correct order
        allF[i*simplex_size+3][0] = T[i][0];
        allF[i*simplex_size+3][1] = T[i][1];
        allF[i*simplex_size+3][2] = T[i][2];
        break;
      case 3:
        allF[i*simplex_size+0][0] = T[i][0];
        allF[i*simplex_size+0][1] = T[i][1];
        allF[i*simplex_size+1][0] = T[i][1];
        allF[i*simplex_size+1][1] = T[i][2];
        allF[i*simplex_size+2][0] = T[i][2];
        allF[i*simplex_size+2][1] = T[i][0];
        break;
    }
  }
  // Counts
  vector<int> C;
  face_occurences(allF,C);

  // Q: Why not just count the number of ones?
  int twos = (int) count(C.begin(),C.end(),2);
  // Resize output to fit number of ones
  F.resize(allF.size() - twos);
  int k = 0;
  for(int i = 0;i< (int)allF.size();i++)
  {
    if(C[i] == 1)
    {
      assert(k<(int)F.size());
      F[k] = allF[i];
      k++;
    }
  }

}

#ifndef IGL_NO_EIGEN
#include "list_to_matrix.h"
#include "matrix_to_list.h"

template <typename DerivedT, typename DerivedF>
IGL_INLINE void igl::boundary_faces(
  const Eigen::PlainObjectBase<DerivedT>& T,
  Eigen::PlainObjectBase<DerivedF>& F)
{
  assert(T.cols() == 0 || T.cols() == 4 || T.cols() == 3);
  using namespace std;
  using namespace Eigen;
  using namespace igl;
  // Cop out: use vector of vectors version
  vector<vector<typename Eigen::PlainObjectBase<DerivedT>::Scalar> > vT;
  matrix_to_list(T,vT);
  vector<vector<typename Eigen::PlainObjectBase<DerivedF>::Scalar> > vF;
  boundary_faces(vT,vF);
  list_to_matrix(vF,F);
}

template <typename DerivedT, typename DerivedF>
IGL_INLINE Eigen::PlainObjectBase<DerivedF> igl::boundary_faces(
  const Eigen::PlainObjectBase<DerivedT>& T)
{
  Eigen::Matrix<typename DerivedF::Scalar, Eigen::Dynamic, Eigen::Dynamic> F;
  igl::boundary_faces(T,F);
  return F;
}
#endif


#ifndef IGL_HEADER_ONLY
// Explicit template specialization
template void igl::boundary_faces<Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> >&);
template void igl::boundary_faces<int, int>(std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > > const&, std::vector<std::vector<int, std::allocator<int> >, std::allocator<std::vector<int, std::allocator<int> > > >&);
template Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > igl::boundary_faces(Eigen::PlainObjectBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&);
#endif

