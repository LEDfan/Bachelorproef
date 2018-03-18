#include "HouseholdCSVReader.h"
#include <util/CSV.h>

gengeopop::HouseholdCSVReader::HouseholdCSVReader(std::istream& inputStream)
{
        stride::util::CSV reader(inputStream);

        for (const stride::util::CSVRow& row : reader) {
                std::shared_ptr<gengeopop::Household> household = std::make_shared<gengeopop::Household>();

                // Create contactpool of the household
                std::shared_ptr<ContactPool> newCP = std::make_shared<ContactPool>();
                for (std::size_t i = 0; i < 12; i++) {
                        unsigned int age;
                        try {
                               age = row.getValue<unsigned int>(i);
                        } catch (const std::bad_cast& e) {
                                // NA
                                break;
                        }

                        std::shared_ptr<stride::Person> p   = std::make_shared<stride::Person>();
                        p->SetAge(age);
                        newCP->addMember(p);
                }
                household->addPool(newCP);
                m_households.push_back(household);
        }
}
