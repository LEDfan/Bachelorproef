#include "HouseholdCSVReader.h"
#include <util/CSV.h>

gengeopop::HouseholdCSVReader::HouseholdCSVReader(std::istream& inputStream)
{
        stride::util::CSV reader(inputStream);

        unsigned int id = 1;

        for (const stride::util::CSVRow& row : reader) {
                std::shared_ptr<gengeopop::Household> household = std::make_shared<gengeopop::Household>();

                // Create contactpool of the household
                std::shared_ptr<ContactPool> newCP = std::make_shared<ContactPool>(id++);
                for (int i = 0; i < 12; i++) {
                        std::string ageString = row.getValue(i);
                        if (ageString == "NA") {
                                break;
                        }
                        int                             age = stoi(ageString);
                        std::shared_ptr<stride::Person> p   = std::make_shared<stride::Person>();
                        p->setAge(age);
                        newCP->addMember(p);
                }
                household->addPool(newCP);
                m_households.push_back(household);
        }
}
