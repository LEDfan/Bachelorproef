#include "GeoGridWriterFactory.h"
#include "GeoGridJSONWriter.h"
#include "GeoGridProtoWriter.h"
#include <boost/filesystem.hpp>
#include <Exception.h>
#include <iostream>
namespace gengeopop {

std::shared_ptr<GeoGridWriter> GeoGridWriterFactory::createWriter(std::string filename) const
{
        boost::filesystem::path path(filename);
        if (!boost::filesystem::exists(path)) {
                throw Exception("File not found: " + path.string());
        }

        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONWriter>();
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoWriter>();
        } else {
                throw Exception("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
