#include "CitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"

gengeopop::CitiesCSVReader::CitiesCSVReader(std::istream& inputStream) {
    // cols: id 	province 	population 	x_coord 	y_coord 	latitude 	longitude 	name
    stride::util::CSV reader(inputStream);

    for(const stride::util::CSVRow& row : reader){
        Location* newLoc = new Location(
            stoi(row.getValue(0)),
            stoi(row.getValue(1)),
            stoi(row.getValue(2)),
            Coordinate(
            stoi(row.getValue(3)),
            stoi(row.getValue(4)),
            stoi(row.getValue(5)),
            stoi(row.getValue(6))),
            row.getValue(7)
                );

        m_locations.emplace_back(newLoc);
    }

}
