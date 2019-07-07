#include "template_importer.h"
#include "halley/bytes/byte_serializer.h"
#include "halley/file_formats/config_file.h"
#include "../../yaml/halley-yamlcpp.h"
#include "halley/tools/file/filesystem.h"

using namespace Halley;

void TemplateImporter::import(const ImportingAsset& asset, IAssetCollector& collector)
{
	ConfigFile config;
	parseConfig(config, gsl::as_bytes(gsl::span<const Byte>(asset.inputFiles.at(0).data)));
	
	Metadata meta = asset.inputFiles.at(0).metadata;
	meta.set("asset_compression", "deflate");

	Path name = Path("template") / config.getRoot()["template"]["name"].asString();
	collector.output(name.getString(), AssetType::ConfigFile, Serializer::toBytes(config), meta);
}