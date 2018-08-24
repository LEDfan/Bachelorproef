#include "GeoGridWriterFactory.h"
#include "GeoGridJSONWriter.h"
#include "GeoGridProtoWriter.h"

#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
namespace std {
namespace filesystem = std::experimental::filesystem;
}
#endif

#include <iostream>
#include <util/Exception.h>
namespace gengeopop {

std::shared_ptr<GeoGridWriter> GeoGridWriterFactory::CreateWriter(std::string filename) const
{
        std::filesystem::path path(filename);

        if (path.extension().string() == ".json") {
                return std::make_shared<GeoGridJSONWriter>();
        } else if (path.extension().string() == ".proto") {
                return std::make_shared<GeoGridProtoWriter>();
        } else {
                throw stride::util::Exception("Unsupported file extension: " + path.extension().string());
        }
}

} // namespace gengeopop
