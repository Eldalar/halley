#pragma once
#include "config_importer.h"

namespace Halley
{
	class ConfigFile;

	class TemplateImporter : public ConfigImporter
	{
	public:
		ImportAssetType getType() const override { return ImportAssetType::Template; }

		void import(const ImportingAsset& asset, IAssetCollector& collector) override;
	};
}
