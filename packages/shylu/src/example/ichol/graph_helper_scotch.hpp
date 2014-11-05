#pragma once
#ifndef __GRAPH_HELPER_SCOTCH_HPP__
#define __GRAPH_HELPER_SCOTCH_HPP__

#include "scotch.h"
#include "util.hpp"

namespace Example { 
  
  using namespace std;

  template<class CrsMatrixType>
  class GraphHelper_Scotch : public Disp {
  public:
    typedef typename CrsMatrixType::ordinal_type ordinal_type;
    typedef typename CrsMatrixType::size_type    size_type;

    typedef typename CrsMatrixType::ordinal_type_array ordinal_type_array;
    typedef typename CrsMatrixType::size_type_array    size_type_array;

  private:
    string _label; 

    // scotch main data structure
    SCOTCH_Graph _graph;

    // scotch input has no diagonal contribution
    ordinal_type _base,_m;
    ordinal_type_array _cidx;

    size_type _nnz;
    size_type_array _rptr;

    // scotch output 
    ordinal_type _cblk;
    ordinal_type_array _perm,_peri,_range,_tree;

    // status flag
    bool _is_ordered;

  public:
    
    void setLabel(string label) { _label = label; }
    string Label() const { return _label; }

    ordinal_type_array PermVector()    const { return _perm; }
    ordinal_type_array InvPermVector() const { return _peri; }

    GraphHelper_Scotch(CrsMatrixType& A) {

      _label = "GraphHelper_Scotch::" + A.Label();

      _is_ordered = false;
      _cblk  = 0;

      // scotch does not allow self-contribution (diagonal term in sparse matrix)
      _base  = 0; //A.BaseVal();
      _m     = A.NumRows();
      _nnz   = A.NumNonZeros();       

      _rptr  = size_type_array(_label+"::RowPtrArray", _m+1);
      _cidx  = ordinal_type_array(_label+"::ColIndexArray", _nnz);

      _perm  = ordinal_type_array(_label+"::PermutationArray", _m);
      _peri  = ordinal_type_array(_label+"::InvPermutationArray", _m);
      _range = ordinal_type_array(_label+"::RangeArray", _m);
      _tree  = ordinal_type_array(_label+"::TreeArray", _m);
      
      // adjust graph structure
      A.convertGraph(_nnz, _rptr, _cidx);

      int ierr = 0;
      ordinal_type *rptr = _rptr.ptr_on_device();
      ordinal_type *cidx = _cidx.ptr_on_device();
      
      ierr = SCOTCH_graphInit(&_graph);CHKERR(ierr);
      ierr = SCOTCH_graphBuild(&_graph,             // scotch graph
                               _base,               // base value
                               _m,                  // # of vertices
                               rptr, // column index array pointer begin
                               rptr+1,             // column index array pointer end
                               NULL,                // weights on vertices (optional)
                               NULL,                // label array on vertices (optional)
                               _nnz,                // # of nonzeros
                               cidx,               // column index array
                               NULL);CHKERR(ierr);  // edge load array (optional)
      ierr = SCOTCH_graphCheck(&_graph);CHKERR(ierr);
    }
    virtual~GraphHelper_Scotch() {
      SCOTCH_graphFree(&_graph);
    }
    ordinal_type getNumBlocks() const {
      return _cblk; 
    }

    // ordinal_type* getPermutationVector() const {
    //   return _perm;
    // }
    // ordinal_type* getInversePermutationVector() const {
    //   return _peri;
    // }

    int computeOrdering() {
      int ierr = 0, level = log2(_nnz)+10;
        
      SCOTCH_Strat stradat;
      SCOTCH_Num straval = (SCOTCH_STRATLEVELMAX   | 
                            SCOTCH_STRATLEVELMIN   | 
                            SCOTCH_STRATLEAFSIMPLE | 
                            SCOTCH_STRATSEPASIMPLE);

      ierr = SCOTCH_stratInit(&stradat);CHKERR(ierr);
      ierr = SCOTCH_stratGraphOrderBuild (&stradat, straval, level, 0.2);CHKERR(ierr);
      
      ordinal_type *perm  = _perm.ptr_on_device();
      ordinal_type *peri  = _peri.ptr_on_device();
      ordinal_type *range = _range.ptr_on_device();
      ordinal_type *tree  = _tree.ptr_on_device();

      ierr = SCOTCH_graphOrder(&_graph, 
                               &stradat, 
                               perm, 
                               peri,
                               &_cblk,
                               range,
                               tree);CHKERR(ierr);
      SCOTCH_stratExit(&stradat);

      _is_ordered = true;

      return 0;
    }

    int constructTree() {
    }

    ostream& showMe(ostream &os) const {
      streamsize prec = os.precision();
      os.precision(15);
      os << scientific;
      
      os << " -- Scotch input -- " << endl
         << "    Base Value     = " << _base << endl
         << "    # of Rows      = " << _m << endl
         << "    # of NonZeros  = " << _nnz << endl;
      
      if (_is_ordered) 
        os << " -- Elimination tree -- " << endl
           << "    CBLK   = " << _cblk << endl
           << "  PERM     PERI     RANG     TREE" << endl;

      const int w = 6;
      for (ordinal_type i=0;i<_m;++i) 
        os << setw(w) << _perm[i] << "   " 
           << setw(w) << _peri[i] << "   " 
           << setw(w) << _range[i] << "   "
           << setw(w) << _tree[i] << endl; 
        
      os.unsetf(ios::scientific);
      os.precision(prec);
      
      return os;
    }
    
  };

}

#endif
