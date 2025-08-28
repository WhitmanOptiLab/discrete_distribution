#include "random_selector.hpp"
#include "modifiable_heap_random_selector.hpp"
#include "no_weight_storage_modifiable_heap_random_selector.hpp"
#include "leaf_sum_tree_selector.hpp"
#include "leaf_sum_tree_split.hpp"
#include "sideways_fenwick_selector.hpp"
#include "old_sideways_fenwick_selector.hpp"
#include "sideways_fenwick_selector_bitcast.hpp"
#include "incremental_leaf_sum_tree.hpp"
#include "bucket_alias.hpp"
#include "wrsLessStorage.hpp"
#include "exponential_leafsum.hpp"
#include "incremental_exponential.hpp"
#include "exponentialAddRemove.hpp"
#include "incremental_exponential_zero_weight.hpp"
#include "incremental_exponential_noResize.hpp"
#include "alexVersionBetterMemory.hpp"
#include "dynamicProposalArray.hpp"
#include "dynamicProposalArrayAlex.hpp"



#include "XoshiroCpp.hpp"


#include <sys/time.h>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>


//std::default_random_engine generator; 
XoshiroCpp::Xoroshiro128Plus generator;
