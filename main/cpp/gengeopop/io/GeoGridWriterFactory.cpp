#include "GeoGridWriterFactory.h"
#include "GeoGridJSONWriter.h"
#include "GeoGridProtoWriter.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <util/Exception.h>
namespace gengeopop {

std::shared_ptr<GeoGridWriter> GeoGridWriterFactory::CreateWriter(std::string filename) const
{
        boost::filesystem::path path(filename);

        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONWriter>();
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoWriter>();
        } else {
                throw stride::util::Exception("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
