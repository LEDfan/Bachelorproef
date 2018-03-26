#include "HouseholdCSVReader.h"
#include <util/CSV.h>

gengeopop::HouseholdCSVReader::HouseholdCSVReader(std::unique_ptr<std::istream> inputStream)
{
        stride::util::CSV reader(*(inputStream.get()));

        unsigned int id = 1;

        for (const stride::util::CSVRow& row : reader) {
                std::shared_ptr<gengeopop::Household> household = std::make_shared<gengeopop::Household>();

                // Create contactpool of the household
                std::shared_ptr<ContactPool> newCP = std::make_shared<ContactPool>(id++);
                for (std::size_t i = 0; i < 12; i++) {
                        unsigned int age;
                        try {
                                age = row.getValue<unsigned int>(i);
                        } catch (const std::bad_cast& e) {
                                // NA
                                break;
                        }

                        if (age < 18 && age >= 6) {
                                m_totalCompulsory++;
                        }

                        if (age >= 18 && age < 26) {
                                m_total1826Years++;
                        }

                        if (age >= 18 && age < 65) {
                                m_total1865Years++;
                        }

                        m_total++;

                        std::shared_ptr<stride::Person> p = std::make_shared<stride::Person>();
                        p->SetAge(age);
                        newCP->addMember(p);
                }
                household->addPool(newCP);
                m_households.push_back(household);
        }
}
