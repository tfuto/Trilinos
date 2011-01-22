#include <Teuchos_UnitTestHarness.hpp>
#include "Tpetra_DefaultPlatform.hpp"
#include "TpetraExt_BlockExtraction.hpp"
#include <Tpetra_CrsMatrix.hpp>
#include <numeric>
#include <algorithm>

namespace {

  using std::string;
  using Teuchos::RCP;
  using Teuchos::rcp;
  using Teuchos::Comm;
  using Teuchos::ScalarTraits;
  using Teuchos::OrdinalTraits;
  using Teuchos::ArrayRCP;
  using Teuchos::tuple;
  using Tpetra::CrsMatrix;
  using Tpetra::RowMatrix;
  using Tpetra::global_size_t;

  bool testMpi = true;
  // string filedir;
  // double errorTolSlack = 1e+1;

  TEUCHOS_STATIC_SETUP()
  {
    Teuchos::CommandLineProcessor &clp = Teuchos::UnitTestRepository::getCLP();
    // clp.setOption(
    //     "filedir",&filedir,"Directory of expected matrix files.");
    clp.addOutputSetupOptions(true);
    clp.setOption(
        "test-mpi", "test-serial", &testMpi,
        "Test MPI (if available) or force test of serial.  In a serial build,"
        " this option is ignord and a serial comm is always used." );
    // clp.setOption(
    //     "error-tol-slack", &errorTolSlack,
    //     "Slack off of machine epsilon used to check test results" );
  }

  RCP<const Comm<int> > getDefaultComm()
  {
    RCP<const Comm<int> > ret;
    if (testMpi) {
      ret = Tpetra::DefaultPlatform::getDefaultPlatform().getComm();
    }
    else {
      ret = rcp(new Teuchos::SerialComm<int>());
    }
    return ret;
  }

  //
  // UNIT TESTS
  // 

  ////
  TEUCHOS_UNIT_TEST_TEMPLATE_3_DECL( BlockDiagonalExtraction, RuntimeExceptions, LO, GO, Scalar )
  {
    typedef Tpetra::Map<LO,GO> Map;
    const global_size_t INVALID = OrdinalTraits<global_size_t>::invalid();
    // get a comm
    RCP<const Comm<int> > comm = getDefaultComm();
    // set the block sizes
    // note one block of zero size, to test capability
    Teuchos::ArrayView<int> block_sizes( Teuchos::tuple<LO>(1,3,5,0,5,3,1) );
    const int maxBlockSize = *std::max_element( block_sizes.begin(), block_sizes.end() );
    // create a Map
    const size_t numLocal = std::accumulate( block_sizes.begin(), block_sizes.end(), (size_t)0 );
    RCP<const Map> map = Tpetra::createContigMap<LO,GO>(INVALID,numLocal,comm);
    RCP<const RowMatrix<Scalar,LO,GO> > mat;
    {
      RCP<CrsMatrix<Scalar,LO,GO> > mat_crs = Tpetra::createCrsMatrix<Scalar>( map );
      for (GO gid=map->getMinGlobalIndex(); gid <= map->getMaxGlobalIndex(); ++gid) {
        // add diagonal entries
        mat_crs->insertGlobalValues( gid, tuple<GO>(gid), tuple<Scalar>(1.0) );
        // add some entries outside of the diagonal block
        if (gid - maxBlockSize >= map->getMinGlobalIndex()) mat_crs->insertGlobalValues( gid, tuple<GO>(gid - maxBlockSize), tuple<Scalar>(1.0) );
        if (gid + maxBlockSize <= map->getMaxGlobalIndex()) mat_crs->insertGlobalValues( gid, tuple<GO>(gid + maxBlockSize), tuple<Scalar>(1.0) );
      }
      mat_crs->fillComplete();
      mat = mat_crs;
    }
    //
    // bad block offsets
    //
    // because the block sizes are underdetermined, solely based on the first indices (the first is always zero)
    // invalid block indices require violating the constraint that all block sizes are non-negative.
    // specifically, that the last block size is positive; this is all that extractBlockDiagonals() will check.
    // this is done by having any except the last block size be too large (the last block size isn't used to generate
    // an offset)
    {
      Teuchos::ArrayView<int> bad_bsizes( Teuchos::tuple<LO>(1,3,5,2,5,3,1) );
      Teuchos::Array<LO> bad_bfirsts( bad_bsizes.size() );
      bad_bfirsts[0] = 0;
      for (int i=1; i < (int)bad_bsizes.size(); ++i) {
        bad_bfirsts[i] = bad_bfirsts[i-1] + bad_bsizes[i-1];
      }
      Teuchos::ArrayRCP<Scalar> out_diags;
      Teuchos::ArrayRCP<LO>     out_offsets;
      TEST_THROW( Tpetra::Ext::extractBlockDiagonals( *mat, bad_bfirsts().getConst(), out_diags, out_offsets ) , std::runtime_error );
    }
    // negative block size corresponds here to non-monotonically increasing first_points
    {
      Teuchos::ArrayView<int> bad_bsizes( Teuchos::tuple<LO>(1,3,5,-1,5,3,1) );
      Teuchos::Array<LO> bad_bfirsts( bad_bsizes.size() );
      bad_bfirsts[0] = 0;
      for (int i=1; i < (int)bad_bsizes.size(); ++i) {
        bad_bfirsts[i] = bad_bfirsts[i-1] + bad_bsizes[i-1];
      }
      Teuchos::ArrayRCP<Scalar> out_diags;
      Teuchos::ArrayRCP<LO>     out_offsets;
      TEST_THROW( Tpetra::Ext::extractBlockDiagonals( *mat, bad_bfirsts().getConst(), out_diags, out_offsets ) , std::runtime_error );
    }
    // first first_point required to be zero
    {
      Teuchos::ArrayView<int> bad_bsizes( Teuchos::tuple<LO>(1,3,5,0,5,3,1) );
      Teuchos::Array<LO> bad_bfirsts( bad_bsizes.size() );
      bad_bfirsts[0] = 1;
      for (int i=1; i < (int)bad_bsizes.size(); ++i) {
        bad_bfirsts[i] = bad_bfirsts[i-1] + bad_bsizes[i-1];
      }
      Teuchos::ArrayRCP<Scalar> out_diags;
      Teuchos::ArrayRCP<LO>     out_offsets;
      TEST_THROW( Tpetra::Ext::extractBlockDiagonals( *mat, bad_bfirsts().getConst(), out_diags, out_offsets ) , std::runtime_error );
    }
    // matrix is required to be fillComplete()
    {
      Teuchos::ArrayView<int> bad_bsizes( Teuchos::tuple<LO>(1,3,5,0,5,3,1) );
      Teuchos::Array<LO> bfirsts( bad_bsizes.size() );
      bfirsts[0] = 0;
      for (int i=1; i < (int)bad_bsizes.size(); ++i) {
        bfirsts[i] = bfirsts[i-1] + bad_bsizes[i-1];
      }
      Teuchos::ArrayRCP<Scalar> out_diags;
      Teuchos::ArrayRCP<LO>     out_offsets;
      RCP<CrsMatrix<Scalar,LO,GO> > not_fill_complete = Tpetra::createCrsMatrix<Scalar>( map );
      TEST_THROW( Tpetra::Ext::extractBlockDiagonals( *not_fill_complete, bfirsts().getConst(), out_diags, out_offsets ) , std::runtime_error );
    }
  }


  ////
  TEUCHOS_UNIT_TEST_TEMPLATE_3_DECL( BlockDiagonalExtraction, SimpleExtraction, LO, GO, Scalar )
  {
    typedef Tpetra::Map<LO,GO>           Map;
    typedef Tpetra::BlockMap<LO,GO> BlockMap;
    const global_size_t INVALID = OrdinalTraits<global_size_t>::invalid();
    // get a comm
    RCP<const Comm<int> > comm = getDefaultComm();

    //
    // set the block sizes
    // note one block of zero size, to test capability
    Teuchos::ArrayView<int> block_sizes( Teuchos::tuple<LO>(1,3,5,0,5,3,1) );
    const int maxBlockSize = *std::max_element( block_sizes.begin(), block_sizes.end() );
    const size_t expected_alloc_size = std::inner_product( block_sizes.begin(), block_sizes.end(), block_sizes.begin(), 0 );
    //
    // create a point Map
    //
    const size_t numLocal = std::accumulate( block_sizes.begin(), block_sizes.end(), (size_t)0 );
    RCP<const Map> map = Tpetra::createContigMap<LO,GO>(INVALID,numLocal,comm);
    //
    // fill matrix for testing
    //
    RCP<const RowMatrix<Scalar,LO,GO> > mat;
    {
      RCP<CrsMatrix<Scalar,LO,GO> > mat_crs = Tpetra::createCrsMatrix<Scalar>( map );
      for (GO gid=map->getMinGlobalIndex(); gid <= map->getMaxGlobalIndex(); ++gid) {
        // add diagonal entries
        mat_crs->insertGlobalValues( gid, tuple<GO>(gid), tuple<Scalar>(1.0) );
        // add some entries outside of the diagonal block
        if (gid - maxBlockSize >= map->getMinGlobalIndex()) mat_crs->insertGlobalValues( gid, tuple<GO>(gid - maxBlockSize), tuple<Scalar>(1.0) );
        if (gid + maxBlockSize <= map->getMaxGlobalIndex()) mat_crs->insertGlobalValues( gid, tuple<GO>(gid + maxBlockSize), tuple<Scalar>(1.0) );
      }
      mat_crs->fillComplete();
      mat = mat_crs;
    }

    //
    // create block_firsts for first extraction
    //
    Teuchos::Array<LO> block_firsts( block_sizes.size() );
    block_firsts[0] = 0;
    for (int i=1; i < (int)block_sizes.size(); ++i) {
      block_firsts[i] = block_firsts[i-1] + block_sizes[i-1];
    }
    //
    // perform first extraction
    //
    Teuchos::ArrayRCP<Scalar> block_diagonals1;
    Teuchos::ArrayRCP<LO>     block_offsets1;
    Tpetra::Ext::extractBlockDiagonals<Scalar,LO,GO>( *mat, block_firsts(), block_diagonals1, block_offsets1 );
    //
    // independently test first extraction
    // 
    {
      TEST_EQUALITY( (size_t)expected_alloc_size, (size_t)block_diagonals1.size() );
      TEST_EQUALITY( (size_t)block_sizes.size(), (size_t)block_offsets1.size() );
      const int num_zeros_extracted    = (int)std::count( block_diagonals1.begin(), block_diagonals1.end(), ScalarTraits<Scalar>::zero() );
      const int num_nonzeros_extracted = (int)block_diagonals1.size() - num_zeros_extracted;
      TEST_EQUALITY( num_nonzeros_extracted, (int)mat->getNodeNumDiags() );
      TEST_EQUALITY_CONST( num_nonzeros_extracted < (int)mat->getNodeNumEntries(), true );
    }

    //
    // create a BlockMap for use in second extraction
    //
    Teuchos::ArrayView<GO> globalBlockIDs( Teuchos::tuple<GO>(1,2,3,4,5,6,7) );
    RCP<const BlockMap> bmap = rcp(new BlockMap(map,globalBlockIDs,block_sizes,map->getNode()));
    // 
    // perform second extraction
    //
    Teuchos::ArrayRCP<Scalar> block_diagonals2;
    Teuchos::ArrayRCP<LO>     block_offsets2;
    Tpetra::Ext::extractBlockDiagonals<Scalar,LO,GO>( *mat, *bmap, block_diagonals2, block_offsets2 );
    // 
    // independently test second extraction
    //
    {
      TEST_EQUALITY( (size_t)expected_alloc_size, (size_t)block_diagonals2.size() );
      TEST_EQUALITY( (size_t)block_sizes.size(), (size_t)block_offsets2.size() );
      const int num_zeros_extracted    = (int)std::count( block_diagonals2.begin(), block_diagonals2.end(), ScalarTraits<Scalar>::zero() );
      const int num_nonzeros_extracted = (int)block_diagonals2.size() - num_zeros_extracted;
      TEST_EQUALITY( num_nonzeros_extracted, (int)mat->getNodeNumDiags() );
      TEST_EQUALITY_CONST( num_nonzeros_extracted < (int)mat->getNodeNumEntries(), true );
    }

    //
    // compare first extraction against second
    //
    TEST_COMPARE_ARRAYS( block_diagonals1, block_diagonals2 );
    TEST_COMPARE_ARRAYS( block_offsets1,   block_offsets2 );
  }


  // 
  // INSTANTIATIONS
  //

#define UNIT_TEST_ORDINAL_SCALAR(LO, GO, SCALAR) \
      TEUCHOS_UNIT_TEST_TEMPLATE_3_INSTANT( BlockDiagonalExtraction, SimpleExtraction, LO, GO, SCALAR ) \
      TEUCHOS_UNIT_TEST_TEMPLATE_3_INSTANT( BlockDiagonalExtraction, RuntimeExceptions, LO, GO, SCALAR )

#define UNIT_TEST_GROUP_ORDINAL( LO, GO ) \
        UNIT_TEST_ORDINAL_SCALAR(LO, GO, double)

UNIT_TEST_GROUP_ORDINAL(int, int)
UNIT_TEST_GROUP_ORDINAL(int, long)

}

