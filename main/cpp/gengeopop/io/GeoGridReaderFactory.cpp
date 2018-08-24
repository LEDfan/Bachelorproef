#include "GeoGridReaderFactory.h"
#include "GeoGridJSONReader.h"
#include "GeoGridProtoReader.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <util/Exception.h>
namespace gengeopop {

std::shared_ptr<GeoGridReader> GeoGridReaderFactory::CreateReader(std::string filename) const
{
        std::filesystem::path path(filename);
        if (!std::filesystem::exists(path)) {
                throw stride::util::Exception("File not found: " + path.string());
        }

        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONReader>(std::make_unique<std::ifstream>(path.string()));
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoReader>(std::make_unique<std::ifstream>(path.string()));
        } else {
                throw stride::util::Exception("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
