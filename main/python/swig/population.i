%module Population
#pragma SWIG nowarn=315
// headers for generated file
%{
#include "pop/Person.h"
#include "pop/Population.h"
#include <vector>
#include <memory>
#include "util/SegmentedVector.h"


using namespace stride;
using namespace stride::util;

%}

%include <std_vector.i>
%include <std_shared_ptr.i>

%include "pop/Person.h"
%include "util/SegmentedVector.h"

%shared_ptr(stride::util::SegmentedVector<stride::Person>);
%shared_ptr(stride::Population);

%template(PopulationBaseClass) stride::util::SegmentedVector<stride::Person>;

%include "pop/Population.h"
