Test plan
===

Travis CI and Jenkins are used to run the tests automatically after every push to Github.
Each Pull Request is also tested and flagged with the result.
If a commit causes a failure we will be notified by email or Telegram (Instant Messaging).

To have an overview of how much code is tested by the test code coverage tools are used. For example lcov is used to generate the code coverage using GCC.
Code coverage is generated on Travis and stored on [Codecov](https://codecov.io/gh/LEDfan/Bachelorproef).

## Unit test

Using the Google Test framework the individual units of the program will be tested.
The difference with scenario tests is that only one unit is tested at a time.

By using white-box testing the internal structure of the code can be studied, allowing us to more easily identify edge cases (e.g. character escaping in the input format, missing keys ...).

The main datastructure classes (`Location`, `GeoGrid`, `ContactCenter`, `ContactPool` and `Cluster`) are rather easy to test because they only have a few simple methods.
The tests of the `School`, `HighSchool`, `Workplace`, `Household` are trivial since they only implement some simple accessor methods.
Generating the GeoGrid is done by the `GeoGridGenerator` class. This class contains a list of classes implementing the `IPartialGenerator` interface.

Our [design](design) is using a different class for every partial generator. In comparison with one `GeoGridGenerator` this design is easier to test, because the different sub-parts can be tested in isolation.

The algorithm used in the `GeoGridGenerator` is very simple: loop over the different generators and execute them, hence it's easy to test it.


| Test name                         | Unit under test            | Algorithm used |
| -------------------------------- | -------------------------- | -------------- |
| `CSVTest::SafeCastTest`          | `stride::util::safe_cast`  | Cast some valid and invalid expressions and compare with the expected output |
| `CSVTest::TestBadCast`           | `stride::util::CSVRow::getValue` | Same method as `CSVTest::SafeCastTest` |
| `CitiesCSVReaderTest::test1`     | `gengeopop::CitiesCSVReader::FillGeoGrid` | Pass an `istringstream` to the reader and compare the generated GeoGrid with an expected GeoGrid. |
| `CommutesCSVReaderTest::test1`   | `gengeopop::CommutesCSVReader::FillGeoGrid` | Pass an `istringstream` to the reader and compare the generated GeoGrid with an expected GeoGrid. The `CommutesReader` calculates some proportions hence the test must also compare those calculated proportions. |
| `HouseholdCSVReaderTest::test1`   | `gengeopop::HouseholdCSVReader::FillGeoGrid` | Pass an `istringstream` to the reader and compare the generated list of households with the expected list. In this case the Households contains a `ContactPool` which contains some `Person`'s with a specific age. |
| `ReaderFactoryTest::TestCommutes` | `gengeopop::ReaderFactory::CreateCommutesReader` | Call factory and check if the returned reader is a CSV reader or a nullptr for respective a csv file or any other file extension. |
| `ReaderFactoryTest::TestCities` | `gengeopop::ReaderFactory::CreateCitiesReader` | Call factory and check if the returned reader is a CSV reader or a nullptr for respective a csv file or any other file extension.
| `ReaderFactoryTest::TestHouseHolds` | `gengeopop::ReaderFactory::CreateHousholdReader` | Call factory and check if the returned reader is a CSV reader or a nullptr for respective a csv file or any other file extension. |
| `ReaderFactoryTest::TestCommutesFromFile` | `gengeopop::ReaderFactory::CreateCommutesReader` , `gengeopop::ReaderFactory::OpenFile` (indirectly, private) | Call the tested method with a file path and use the provided reader to parse a file and compare the expected output. |
| `GeoGridJSONReaderTest::locationsTest` | `gengeopop::GeoGridJSONReader::ParseLocation` | Read a json file using the reader and check the GeoGrid with an expected GeoGrid, but only the locations. |
| `GeoGridJSONReaderTest::contactCentersTest` | `gengeopop::GeoGridJSONReader::ParseContactCenter` | Read a json file using the reader and check the GeoGrid with an expected GeoGrid, but only the contactcenters. |
| `GeoGridJSONReaderTest::peopleTest` | `gengeopop::GeoGridJSONReader::ParsePerson` | Read a json file using the reader and check the GeoGrid with an expected GeoGrid, but only the people. |
| `GeoGridJSONWriterTest::locationTest`| `gengeopop::GeoGridJSONWriter::parseLocation` | Create a GeoGrid, add some locations to it, write it to json and compare with an expected file.
| `GeoGridJSONWriterTest::contactCentersTest`| `gengeopop::GeoGridJSONWriter::parseLocation` | Create a GeoGrid, add a location with some contactcenters, write it to json and compare with an expected file. |
| `GeoGridJSONWriterTest::peopleTest` | `gengeopop::GeoGridJSONWriter::parseContactPool` | Create a GeoGrid, add some ContactCenters, Pools and Person's, write it to json and compare with an expected file. |
| `CommunityGeneratorTest::OneLocationTest` | `gengeopop::CommunityGenerator` | Create a communityGenerator and a GeoGrid, run the communityGenerator and compare it with an expected result. This test will generate a limited amount of communities. |
| `CommunityGeneratorTest::EqualLocationTest` | `gengeopop::CommunityGenerator` | Create a CommunityGenerator and a GeoGrid, run the communityGenerator and compare it with an expected result. This test will have locations of equal size to easily test the random distribution. |
| `CommunityGeneratorTest::ZeroLocationTest` | `gengeopop::CommunityGenerator` | Create a CommunityGenerator and expect 0 locations to be added. |
| `CommunityGeneratorTest::FiveLocationTest` | `gengeopop::CommunityGenerator` | Create a CommunityGenerator and generate 5 locations and expect the generated communities to be correctly distributed. |
| `HighSchoolGeneratorTest::OneLocationTest` | `gengeopop::HighSchoolGenerator` | Create a HighSchoolGenerator and a GeoGrid, run the communityGenerator and compare it with an expected result. This test will generate a limited amount of communities. |
| `HighSchoolGenerator::ZeroLocationTest` | `gengeopop::HighSchoolGenerator` | Create a highSchoolGenerator and expect 0 locations to be added. |
| `HighSchoolGeneratorTest::OneLocationTest` | `gengeopop::HighschoolGenerator` | Create a HighSchoolGenerator and generate 5 locations and expect the generated communities to be correctly distributed. |
| `SchoolGeneratorTest::OneLocationTest` | `gengeopop::SchoolGenerator` | Create SchoolGenerator and a GeoGrid, run the communityGenerator and compare it with an expected result. This test will generate a limited amount of communities. |
| `SchoolGenerator::ZeroLocationTest` | `gengeopop::SchoolGenerator` | Create a SchoolGenerator and expect 0 locations to be added. |
| `SchoolGeneratorTest::OneLocationTest` | `gengeopop::SchoolGenerator` | Create a SchoolGenerator and generate 5 locations and expect the generated communities to be correctly distributed. |
| `WorkplaceGeneratorTest::OneLocationTest` | `gengeopop::WorkplaceGenerator` | Create a WorkplaceGenerator and a GeoGrid, run the generator and compare it with an expected result. This test will generate a limited amount of communities. |
| `WorkplaceGenerator::ZeroLocationTest` | `gengeopop::WorkplaceGenerator` | Create a WorkplaceGenerator and expect 0 locations to be added. |
| `WorkplaceGeneratorTest::OneLocationTest` | `gengeopop::WorkplaceGenerator` | Create a WorkplaceGenerator and generate 5 locations and expect the generated communities to be correctly distributed. |


### GeoGrid JSON Writer
Functionality: the GeoGrid Reader converts a generated GeoGrid to a JSON file. This can be done after the GeoGrid or population was generated, since they use the same internal data structure.
The resulting output should contain all information in the GeoGrid so it can be converted to a GeoGrid again by the GeoGrid reader without any difference.

Test Method: manually construct a GeoGrid and compare the resulting JSON with the expected values. This should test all possible types of values, like for example a Household, School, Workplace, etc, but also with and without people in the corresponding ContactCenter.
### GeoGrid JSON Reader
Functionality: convert a JSON file containing the necessary information to a valid GeoGrid.

Test Method: construct manual JSON input files and compare the generated GeoGrid to the expected values. The criteria for building good GeoGrids for testing is analogous to the GeoGrid Writer.
### Generators
Functionality: given a GeoGrid, a Generator adds Schools, Workplaces or Communities to Locations based on a probabilistic method as described in the assignment.

Test Method: given a specific GeoGrid, check if the decorated GeoGrid by the Generator is the same as expected. As the generator uses a probabilistic method, the testing wil need to be done by using a fixed seed for the tests or by comparing the output to reasonable bounds.

### Populators
List of populators:
 - HouseholdPopulator
 - StudentPopulator
 - SchoolPopulator
 - WorkplacePopulator
 - CommunityPopulator

Functionality: these are all implemented as a partial populator and thus can apply their functionality on the passed GeoGrid. This workflow is similar to that of the Generators.

Test Method: this similarity also means the testing method can be quite similar. For each populator, we construct a set of GeoGrids which have certain characteristics to reliably test certain corresponding functionality. Since these methods are also probabilistic in nature, we have to apply a similar comparison method to the one used in the Generator tests.

## Scenario tests
The outcome of the complete Simulator is tested by a [Scenario Test](https://github.com/LEDfan/Bachelorproef/blob/master/test/cpp/gtester/BatchRuns.cpp), more information can be found in [Bounds Research](bounds.md) and [Paper](week3/main/paper.pdf).

The GeoGrid Generator must also be tested by scenario tests.
By splitting up the generator into subparts the influence of the different parts on the outcome of the GeoGenerator can be compared and verified.
The scenario tests can also be splitted into the different parts.
A similar research as for the BatchRuns can be done in order to determine the acceptable values.

The Python interface is currently tested by a [simple smoke test](https://github.com/LEDfan/Bachelorproef/blob/master/test/python/testSimple.py).
It doesn't cover all parts of the interface but any compile or link error will be discovered.

As the design is subject to change when it is implemented. We do not know perfectly what tests will be made exactly. So this test plan inventory may be expanded in the future.

### GeoGrid JSON Reader and Writer
Functionality: same as the combination of the individual components, but we have to make sure they can work together.

Test Method: create a manual GeoGrid, convert it to JSON by using the GeoGrid writer and parse this again in the GeoGrid reader. We can then compare this to the original GeoGrid, which should be identical.

### Reader Factory
Functionality: the factory must return a reader that gives the correct output given a specific input file.

Test Method: we test the reader factory by asking to create a reader for a file. We then check if the output of the reader is as expected.

### Complete pipeline / GeoGridGenerator
Functionality: the complete functionality of the GeoGen.

Test Method:
 - Give the program specific cities, commutes and household input files, and then check that GeoGrid is generated from this. This also has the extra difficulty of being probabilistic, so bounds need to be used.
 - We will also test the extreme cases. These are cases where the input is limited so we know what the output will be. For example one city with enough population so there will be at least 1 school generated in the city.
