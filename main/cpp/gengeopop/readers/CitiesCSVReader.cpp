#include "CitiesCSVReader.h"
#include "../../util/CSV.h"

gengeopop::CitiesCSVReader::CitiesCSVReader(std::istream& inputStream) {
    // TODO set m_cities
    // id 	province 	population 	x_coord 	y_coord 	latitude 	longitude 	name
    stride::util::CSV reader(inputStream);
    assert(reader.getColumnCount() == 8);

}
