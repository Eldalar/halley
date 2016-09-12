#pragma once

#include <halley/text/halleystring.h>
#include <memory>
#include <halley/resources/resource.h>

namespace Halley
{
	class Deserializer;
	class Serializer;
	class ResourceLoader;
	class SpriteSheet;
	class SpriteSheetEntry;
	class Material;
	class AnimationDirection;
	class AnimationImporter;

	class AnimationFrame
	{
	public:
		AnimationFrame(int frameNumber, String imageName, SpriteSheet& sheet, const Vector<AnimationDirection>& directions);
		const SpriteSheetEntry& getSprite(int dir) const { return *sprites[dir]; }

	private:
		Vector<const SpriteSheetEntry*> sprites;
	};

	class AnimationFrameDefinition
	{
	public:
		AnimationFrameDefinition();
		AnimationFrameDefinition(int frameNumber, String imageName);
		AnimationFrame makeFrame(SpriteSheet& sheet, const Vector<AnimationDirection>& directions) const;

		void serialize(Serializer& s) const;
		void deserialize(Deserializer& s);

	private:
		String imageName;
		int frameNumber;
	};

	class AnimationSequence
	{
		friend class AnimationImporter;
		friend class Animation;

	public:
		float getFPS() const { return fps; }
		size_t numFrames() const { return frames.size(); }
		const AnimationFrame& getFrame(size_t n) const { return frames[n]; }
		String getName() const { return name; }
		bool isLooping() const { return loop; }
		bool isNoFlip() const { return noFlip; }

		void serialize(Serializer& s) const;
		void deserialize(Deserializer& s);

	private:
		Vector<AnimationFrame> frames;
		Vector<AnimationFrameDefinition> frameDefinitions;
		String name;
		float fps = 0;
		bool loop = false;
		bool noFlip = false;
	};

	class AnimationDirection
	{
		friend class AnimationImporter;
		friend class Animation;

	public:
		AnimationDirection();
		AnimationDirection(String name, String fileName, bool flip, int id);

		String getName() const { return name; }
		String getFileName() const { return fileName; }
		bool shouldFlip() const { return flip; }
		int getId() const { return id; }
		String getFrameName(int frameNumber, String baseName) const;

		void serialize(Serializer& s) const;
		void deserialize(Deserializer& s);

	private:
		String name;
		String fileName;
		int id;
		bool flip;
	};
	
	class Animation final : public Resource
	{
		friend class AnimationImporter;

	public:
		Animation();
		static std::unique_ptr<Animation> loadResource(ResourceLoader& loader);

		SpriteSheet& getSpriteSheet() const { return *spriteSheet; }
		std::shared_ptr<Material> getMaterial() const { return material; }
		const AnimationSequence& getSequence(String name) const;
		const AnimationDirection& getDirection(String name) const;
		const AnimationDirection& getDirection(int id) const;

		void serialize(Serializer& s) const;
		void deserialize(Deserializer& s);
		void loadDependencies(ResourceLoader& loader);

	private:
		String name;
		String spriteSheetName;
		String materialName;
		Vector<AnimationSequence> sequences;
		Vector<AnimationDirection> directions;

		std::shared_ptr<SpriteSheet> spriteSheet;
		std::shared_ptr<Material> material;
	};
}
