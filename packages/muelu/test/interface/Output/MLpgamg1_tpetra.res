========================= Aggregate option summary  =========================
min Nodes per aggregate :               1
min # of root nbrs already aggregated : 0
aggregate ordering :                    natural
=============================================================================
Level 0
 Setup Smoother (MueLu::Ifpack2Smoother{type = RELAXATION})
 relaxation: type = Symmetric Gauss-Seidel
 relaxation: sweeps = 2
 relaxation: damping factor = 1
 relaxation: zero starting solution = 1   [default]
 relaxation: backward mode = 0   [default]
 relaxation: use l1 = 0   [default]
 relaxation: l1 eta = 1.5   [default]
 relaxation: min diagonal value = 0   [default]
 relaxation: fix tiny diagonal entries = 0   [default]
 relaxation: check diagonal entries = 0   [default]
 relaxation: local smoothing indices = Teuchos::ArrayRCP<int>{ptr=0,lowerOffset=0,upperOffset=-1,size=0,node=0,strong_count=0,weak_count=0}   [default]

Level 1
 Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Build (MueLu::CoalesceDropFactory)
     Build (MueLu::AmalgamationFactory)
     [empty list]

    aggregation: drop tol = 0   [default]
    aggregation: Dirichlet threshold = 0   [default]
    aggregation: drop scheme = classical   [default]
    lightweight wrap = 0   [default]

   aggregation: mode = old   [default]
   aggregation: max agg size = -1   [default]
   aggregation: min agg size = 1   [unused]
   aggregation: max selected neighbors = 0   [unused]
   aggregation: ordering = natural   [unused]
   aggregation: enable phase 1 = 1   [default]
   aggregation: enable phase 2a = 1   [default]
   aggregation: enable phase 2b = 1   [default]
   aggregation: enable phase 3 = 1   [default]
   aggregation: preserve Dirichlet points = 0   [default]
   UseOnePtAggregationAlgorithm = 0   [default]
   UsePreserveDirichletAggregationAlgorithm = 0   [unused]
   UseUncoupledAggregationAlgorithm = 1   [default]
   UseMaxLinkAggregationAlgorithm = 1   [default]
   UseIsolatedNodeAggregationAlgorithm = 1   [default]
   UseEmergencyAggregationAlgorithm = 1   [default]
   OnePt aggregate map name =    [default]

   Build (MueLu::CoarseMapFactory)
   Striding info = {}   [default]
   Strided block id = -1   [default]
   Domain GID offsets = {0}   [default]

  [empty list]

  PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
 Minimization norm = 2   [default]
 ReUseRowBasedOmegas = 0   [default]

 Call prolongator factory for calculating restrictor (MueLu::GenericRFactory)
  Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
   PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
  Minimization norm = 2   [default]
  ReUseRowBasedOmegas = 0   [default]

 [empty list]

 Computing Ac (MueLu::RAPFactory)
 transpose: use implicit = 0   [default]
 Keep AP Pattern = 0   [default]
 Keep RAP Pattern = 0   [default]
 CheckMainDiagonal = 0   [default]
 RepairMainDiagonal = 0

 Setup Smoother (MueLu::Ifpack2Smoother{type = RELAXATION})
 relaxation: type = Symmetric Gauss-Seidel
 relaxation: sweeps = 2
 relaxation: damping factor = 1
 relaxation: zero starting solution = 1   [default]
 relaxation: backward mode = 0   [default]
 relaxation: use l1 = 0   [default]
 relaxation: l1 eta = 1.5   [default]
 relaxation: min diagonal value = 0   [default]
 relaxation: fix tiny diagonal entries = 0   [default]
 relaxation: check diagonal entries = 0   [default]
 relaxation: local smoothing indices = Teuchos::ArrayRCP<int>{ptr=0,lowerOffset=0,upperOffset=-1,size=0,node=0,strong_count=0,weak_count=0}   [default]

Level 2
 Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Build (MueLu::CoalesceDropFactory)
     Build (MueLu::AmalgamationFactory)
     [empty list]

    aggregation: drop tol = 0   [default]
    aggregation: Dirichlet threshold = 0   [default]
    aggregation: drop scheme = classical   [default]
    lightweight wrap = 0   [default]

   aggregation: mode = old   [default]
   aggregation: max agg size = -1   [default]
   aggregation: min agg size = 1   [unused]
   aggregation: max selected neighbors = 0   [unused]
   aggregation: ordering = natural   [unused]
   aggregation: enable phase 1 = 1   [default]
   aggregation: enable phase 2a = 1   [default]
   aggregation: enable phase 2b = 1   [default]
   aggregation: enable phase 3 = 1   [default]
   aggregation: preserve Dirichlet points = 0   [default]
   UseOnePtAggregationAlgorithm = 0   [default]
   UsePreserveDirichletAggregationAlgorithm = 0   [unused]
   UseUncoupledAggregationAlgorithm = 1   [default]
   UseMaxLinkAggregationAlgorithm = 1   [default]
   UseIsolatedNodeAggregationAlgorithm = 1   [default]
   UseEmergencyAggregationAlgorithm = 1   [default]
   OnePt aggregate map name =    [default]

   Nullspace factory (MueLu::NullspaceFactory)
   Fine level nullspace = Nullspace

   Build (MueLu::CoarseMapFactory)
   Striding info = {}   [default]
   Strided block id = -1   [default]
   Domain GID offsets = {0}   [default]

  [empty list]

  PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
 Minimization norm = 2   [default]
 ReUseRowBasedOmegas = 0   [default]

 Call prolongator factory for calculating restrictor (MueLu::GenericRFactory)
  Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
   PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
  Minimization norm = 2   [default]
  ReUseRowBasedOmegas = 0   [default]

 [empty list]

 Computing Ac (MueLu::RAPFactory)
 transpose: use implicit = 0   [default]
 Keep AP Pattern = 0   [default]
 Keep RAP Pattern = 0   [default]
 CheckMainDiagonal = 0   [default]
 RepairMainDiagonal = 0

 Setup Smoother (MueLu::Ifpack2Smoother{type = RELAXATION})
 relaxation: type = Symmetric Gauss-Seidel
 relaxation: sweeps = 2
 relaxation: damping factor = 1
 relaxation: zero starting solution = 1   [default]
 relaxation: backward mode = 0   [default]
 relaxation: use l1 = 0   [default]
 relaxation: l1 eta = 1.5   [default]
 relaxation: min diagonal value = 0   [default]
 relaxation: fix tiny diagonal entries = 0   [default]
 relaxation: check diagonal entries = 0   [default]
 relaxation: local smoothing indices = Teuchos::ArrayRCP<int>{ptr=0,lowerOffset=0,upperOffset=-1,size=0,node=0,strong_count=0,weak_count=0}   [default]

Level 3
 Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Build (MueLu::CoalesceDropFactory)
     Build (MueLu::AmalgamationFactory)
     [empty list]

    aggregation: drop tol = 0   [default]
    aggregation: Dirichlet threshold = 0   [default]
    aggregation: drop scheme = classical   [default]
    lightweight wrap = 0   [default]

   aggregation: mode = old   [default]
   aggregation: max agg size = -1   [default]
   aggregation: min agg size = 1   [unused]
   aggregation: max selected neighbors = 0   [unused]
   aggregation: ordering = natural   [unused]
   aggregation: enable phase 1 = 1   [default]
   aggregation: enable phase 2a = 1   [default]
   aggregation: enable phase 2b = 1   [default]
   aggregation: enable phase 3 = 1   [default]
   aggregation: preserve Dirichlet points = 0   [default]
   UseOnePtAggregationAlgorithm = 0   [default]
   UsePreserveDirichletAggregationAlgorithm = 0   [unused]
   UseUncoupledAggregationAlgorithm = 1   [default]
   UseMaxLinkAggregationAlgorithm = 1   [default]
   UseIsolatedNodeAggregationAlgorithm = 1   [default]
   UseEmergencyAggregationAlgorithm = 1   [default]
   OnePt aggregate map name =    [default]

   Nullspace factory (MueLu::NullspaceFactory)
   Fine level nullspace = Nullspace

   Build (MueLu::CoarseMapFactory)
   Striding info = {}   [default]
   Strided block id = -1   [default]
   Domain GID offsets = {0}   [default]

  [empty list]

  PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
 Minimization norm = 2   [default]
 ReUseRowBasedOmegas = 0   [default]

 Call prolongator factory for calculating restrictor (MueLu::GenericRFactory)
  Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
   PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
  Minimization norm = 2   [default]
  ReUseRowBasedOmegas = 0   [default]

 [empty list]

 Computing Ac (MueLu::RAPFactory)
 transpose: use implicit = 0   [default]
 Keep AP Pattern = 0   [default]
 Keep RAP Pattern = 0   [default]
 CheckMainDiagonal = 0   [default]
 RepairMainDiagonal = 0

 Setup Smoother (MueLu::Ifpack2Smoother{type = RELAXATION})
 relaxation: type = Symmetric Gauss-Seidel
 relaxation: sweeps = 2
 relaxation: damping factor = 1
 relaxation: zero starting solution = 1   [default]
 relaxation: backward mode = 0   [default]
 relaxation: use l1 = 0   [default]
 relaxation: l1 eta = 1.5   [default]
 relaxation: min diagonal value = 0   [default]
 relaxation: fix tiny diagonal entries = 0   [default]
 relaxation: check diagonal entries = 0   [default]
 relaxation: local smoothing indices = Teuchos::ArrayRCP<int>{ptr=0,lowerOffset=0,upperOffset=-1,size=0,node=0,strong_count=0,weak_count=0}   [default]

Level 4
 Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Build (MueLu::CoalesceDropFactory)
     Build (MueLu::AmalgamationFactory)
     [empty list]

    aggregation: drop tol = 0   [default]
    aggregation: Dirichlet threshold = 0   [default]
    aggregation: drop scheme = classical   [default]
    lightweight wrap = 0   [default]

   aggregation: mode = old   [default]
   aggregation: max agg size = -1   [default]
   aggregation: min agg size = 1   [unused]
   aggregation: max selected neighbors = 0   [unused]
   aggregation: ordering = natural   [unused]
   aggregation: enable phase 1 = 1   [default]
   aggregation: enable phase 2a = 1   [default]
   aggregation: enable phase 2b = 1   [default]
   aggregation: enable phase 3 = 1   [default]
   aggregation: preserve Dirichlet points = 0   [default]
   UseOnePtAggregationAlgorithm = 0   [default]
   UsePreserveDirichletAggregationAlgorithm = 0   [unused]
   UseUncoupledAggregationAlgorithm = 1   [default]
   UseMaxLinkAggregationAlgorithm = 1   [default]
   UseIsolatedNodeAggregationAlgorithm = 1   [default]
   UseEmergencyAggregationAlgorithm = 1   [default]
   OnePt aggregate map name =    [default]

   Nullspace factory (MueLu::NullspaceFactory)
   Fine level nullspace = Nullspace

   Build (MueLu::CoarseMapFactory)
   Striding info = {}   [default]
   Strided block id = -1   [default]
   Domain GID offsets = {0}   [default]

  [empty list]

  PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
 Minimization norm = 2   [default]
 ReUseRowBasedOmegas = 0   [default]

 Call prolongator factory for calculating restrictor (MueLu::GenericRFactory)
  Prolongator smoothing (PG-AMG) (MueLu::PgPFactory)
   PgPFactory::ComputeRowBasedOmega (MueLu::PgPFactory)
  Minimization norm = 2   [default]
  ReUseRowBasedOmegas = 0   [default]

 [empty list]

 Computing Ac (MueLu::RAPFactory)
 transpose: use implicit = 0   [default]
 Keep AP Pattern = 0   [default]
 Keep RAP Pattern = 0   [default]
 CheckMainDiagonal = 0   [default]
 RepairMainDiagonal = 0

 Setup Smoother (MueLu::Amesos2Smoother{type = Klu})
 [empty list]


--------------------------------------------------------------------------------
---                            Multigrid Summary                             ---
--------------------------------------------------------------------------------
Number of levels    = 5
Operator complexity = 1.49

matrix rows    nnz  nnz/row procs
A 0    9999  29995     3.00  1
A 1    3333   9997     3.00  1
A 2    1111   3331     3.00  1
A 3     371   1111     2.99  1
A 4     124    370     2.98  1

Smoother (level 0) both : "Ifpack2::Relaxation": {Initialized: true, Computed: true, Type: Symmetric Gauss-Seidel, sweeps: 2, damping factor: 1, Global matrix dimensions: [9999, 9999], Global nnz: 29995}

Smoother (level 1) both : "Ifpack2::Relaxation": {Initialized: true, Computed: true, Type: Symmetric Gauss-Seidel, sweeps: 2, damping factor: 1, Global matrix dimensions: [3333, 3333], Global nnz: 9997}

Smoother (level 2) both : "Ifpack2::Relaxation": {Initialized: true, Computed: true, Type: Symmetric Gauss-Seidel, sweeps: 2, damping factor: 1, Global matrix dimensions: [1111, 1111], Global nnz: 3331}

Smoother (level 3) both : "Ifpack2::Relaxation": {Initialized: true, Computed: true, Type: Symmetric Gauss-Seidel, sweeps: 2, damping factor: 1, Global matrix dimensions: [371, 371], Global nnz: 1111}

Smoother (level 4) pre  : KLU2 solver interface
Smoother (level 4) post : no smoother

