#include "CitiesCSVReader.h"
#include "../../util/CSV.h"
#include "../../util/CSVRow.h"

gengeopop::CitiesCSVReader::CitiesCSVReader(std::istream& inputStream)
{
        // cols: id 	province 	population 	x_coord 	y_coord 	latitude 	longitude
        // name
        stride::util::CSV reader(inputStream);

        for (const stride::util::CSVRow& row : reader) {
                int  id       = stoi(row.getValue(0));
                auto location = std::make_shared<Location>(id,                               // id
                                                           stoi(row.getValue(1)),            // province
                                                           stoi(row.getValue(2)),            // relative population
                                                           Coordinate(stod(row.getValue(3)), // x_coord
                                                                      stod(row.getValue(4)), // y_coord
                                                                      stod(row.getValue(5)), // latitude
                                                                      stod(row.getValue(6))  // longtitude
                                                                      ),
                                                           row.getValue(7));

                m_locations.insert({id, location});
        }
}
