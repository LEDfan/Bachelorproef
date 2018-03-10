#include <util/CSV.h>
#include "HouseholdCSVReader.h"

gengeopop::HouseholdCSVReader::HouseholdCSVReader(std::istream &inputStream) {
    stride::util::CSV reader(inputStream);

    for (const stride::util::CSVRow &row : reader) {
        gengeopop::Household household;

        // Create contactpool of the household
        ContactPool newCP = ContactPool();
        for (int i = 0; i < 12; i++) {
            std::string ageString = row.getValue(i);
            if (ageString == "NA") {
                break;
            }
            int age = stoi(ageString);
            stride::Person p = stride::Person();
            p.setAge(age);
            newCP.addMember(std::make_shared<stride::Person>(p));
        }
        household.addPool(std::make_shared<ContactPool>(newCP));
        m_households.push_back(std::make_shared<Household>(household));
    }
}
