#include <Kokkos_Core.hpp>
#include "util.hpp"

#include "crs_matrix_base.hpp"
#include "crs_matrix_view.hpp"
#include "crs_row_view.hpp"

using namespace std;

typedef double value_type;
typedef int    ordinal_type;
typedef size_t size_type;

typedef Kokkos::OpenMP host_type;

typedef Example::CrsMatrixBase<value_type,ordinal_type,size_type,host_type> CrsMatrixBase;
typedef Example::CrsMatrixView<CrsMatrixBase> CrsMatrixView;
typedef Example::CrsRowView<CrsMatrixBase> CrsRowView;

int main (int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " filename" << endl;
    return -1;
  }

  Kokkos::initialize();
  cout << "Default execution space initialized = "
       << typeid(Kokkos::DefaultExecutionSpace).name()
       << endl; 

  CrsMatrixBase Abase("Abase");

  ifstream in;
  in.open(argv[1]);
  if (!in.good()) {
    cout << "Error in open the file: " << argv[1] << endl;
    return -1;
  }
  Abase.importMatrixMarket(in);

  {
    CrsMatrixView A(Abase, 2, 6, 
                    /**/   3, 8);
  
    CrsRowView row = A.extractRow(2);
    cout << row << endl;

    cout << "Densified row view = " << endl;
    for (ordinal_type j=0;j<row.NumCols();++j) 
      cout << row.ValueAtColumn(j) << "  ";
    cout << endl;
  }

  Kokkos::finalize(); 

  return 0;
}
