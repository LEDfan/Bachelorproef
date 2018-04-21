#include "GeoGridReaderFactory.h"
#include "GeoGridJSONReader.h"
#include "GeoGridProtoReader.h"
#include <boost/filesystem.hpp>
#include <iostream>
namespace gengeopop {

std::shared_ptr<GeoGridReader> GeoGridReaderFactory::createReader(std::string filename) const
{
        boost::filesystem::path path(filename);
        if (!boost::filesystem::exists(path)) {
                throw std::invalid_argument("File not found: " + path.string());
        }

        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONReader>(std::make_unique<std::ifstream>(path.string()));
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoReader>(std::make_unique<std::ifstream>(path.string()));
        } else {
                throw std::runtime_error("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
