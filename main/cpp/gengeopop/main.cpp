#include "Community.h"
#include "GeoGrid.h"
#include "HighSchool.h"
#include "School.h"
#include "Workplace.h"
#include "generators/GeoGridGenerator.h"
#include "generators/SchoolGenerator.h"
#include "io/GeoGridJSONReader.h"
#include "io/GeoGridJSONWriter.h"
#include <iostream>

using namespace gengeopop;

int main()
{
        GeoGridConfig config;
        config.populationSize            = 10000;
        config.fraction_compulsoryPupils = 0.20;

        GeoGridGenerator         geoGridGenerator(config);
        std::shared_ptr<GeoGrid> geoGrid = geoGridGenerator.getGeoGrid();

        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");

        auto school = std::make_shared<School>();
        location->addContactCenter(school);
        auto schoolPool = std::make_shared<ContactPool>(2);
        school->addPool(schoolPool);

        auto community = std::make_shared<Community>();
        location->addContactCenter(community);
        auto communityPool = std::make_shared<ContactPool>(3);
        community->addPool(communityPool);

        auto secondaryCommunity = std::make_shared<Community>();
        location->addContactCenter(secondaryCommunity);
        auto secondaryCommunityPool = std::make_shared<ContactPool>(7);
        secondaryCommunity->addPool(secondaryCommunityPool);

        auto highschool = std::make_shared<HighSchool>();
        location->addContactCenter(highschool);
        auto highschoolPool = std::make_shared<ContactPool>(4);
        highschool->addPool(highschoolPool);

        auto household = std::make_shared<Household>();
        location->addContactCenter(household);
        auto householdPool = std::make_shared<ContactPool>(5);
        household->addPool(householdPool);

        auto workplace = std::make_shared<Workplace>();
        location->addContactCenter(workplace);
        auto workplacePool = std::make_shared<ContactPool>(6);
        workplace->addPool(workplacePool);

        geoGrid->addLocation(location);
        auto person = std::make_shared<stride::Person>(1, 18, 4, 2, 6, 3, 7, 0, 0, 0, 0, 0);
        communityPool->addMember(person);
        schoolPool->addMember(person);
        secondaryCommunityPool->addMember(person);
        highschoolPool->addMember(person);
        householdPool->addMember(person);
        workplacePool->addMember(person);

        std::stringstream ss;
        GeoGridJSONWriter writer;
        writer.write(geoGrid, ss);
        GeoGridJSONReader reader;
        reader.read(ss);

        stride::util::RNManager::Info rnInfo;
        rnInfo.m_seed = 100;
        stride::util::RNManager rnManager(rnInfo);

        auto schoolGenerator = std::make_shared<SchoolGenerator>(rnManager);

        geoGridGenerator.addPartialGenerator(schoolGenerator);

        geoGridGenerator.generateGeoGrid();

        return 0;
}
