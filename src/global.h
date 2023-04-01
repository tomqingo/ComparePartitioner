#pragma once

// STL libraries
#include <iostream>
#include <string>
#include <csignal>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <random>
#include <math.h>

#include <chrono>
using namespace std::chrono;

// #include "sstream.h"

// STL libraries
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <csignal>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <set>
#include <tuple>
#include <bitset>
#include <sstream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <numeric>

// Boost libraries
#include <boost/program_options.hpp>
#include <boost/icl/split_interval_map.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

using std::greater;
using std::max;
using std::min;

using std::make_pair;
using std::make_shared;
using std::map;
using std::mutex;
using std::pair;
using std::priority_queue;
using std::queue;
using std::set;
using std::shared_ptr;
using std::stack;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;
using std::weak_ptr;

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::ostream;

using boostPoint = bg::model::point<double, 2, bg::cs::cartesian>;
using boostBox = bg::model::box<boostPoint>;
using RTree = bgi::rtree<std::pair<boostBox, int>, bgi::rstar<32>>;
using RTrees = vector<bgi::rtree<std::pair<boostBox, int>, bgi::rstar<32>>>;

#define fo(i, n) for (int i = 0, _n = (n); i < _n; ++i)
// #define range(i, a, b) for (int i = (a), _n = (b); i < _n; ++i)

// Third party

//#include "flute/flute.h"
//#include "salt/salt.h"
//#include "salt/base/eval.h" // for salt::WireLengthEval
#include "patoh/patoh.h"
// extern "C"
// {
// 	DTYPE flute_wl(int d, DTYPE x[], DTYPE y[], int acc);
// 	Tree flute(int d, DTYPE x[], DTYPE y[], int acc);
// 	void freeRSMT(Tree *t);
// 	void readLUT();
// }
