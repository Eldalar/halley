#pragma once

/*****************************************************************\
           __
          / /
		 / /                     __  __
		/ /______    _______    / / / / ________   __       __
	   / ______  \  /_____  \  / / / / / _____  | / /      / /
	  / /      | / _______| / / / / / / /____/ / / /      / /
	 / /      / / / _____  / / / / / / _______/ / /      / /
	/ /      / / / /____/ / / / / / / |______  / |______/ /
   /_/      /_/ |________/ / / / /  \_______/  \_______  /
                          /_/ /_/                     / /
			                                         / /
		       High Level Game Framework            /_/

  ---------------------------------------------------------------

  Copyright (c) 2007-2014 - Rodrigo Braz Monteiro.
  This file is subject to the terms of halley_license.txt.

\*****************************************************************/

namespace Halley {
	template <typename T, size_t blockLen = 16384>
	class MappedPool {
		struct Entry {
			std::array<char, sizeof(T)> data;
			unsigned short nextFreeEntryIndex;
			unsigned short revision;
		};

		struct Block {
			Vector<Entry> data;

			Block(size_t blockIndex)
				: data(blockLen)
			{
				size_t base = blockIndex * blockLen;
				for (size_t i = 0; i < blockLen - 1; i++) {
					// Each entry points to the next
					data[i].nextFreeEntryIndex = static_cast<unsigned short>(i + 1 + base);
					data[i].revision = 0;
				}
				data[blockLen - 1].nextFreeEntryIndex = 0xFFFF;
				data[blockLen - 1].revision = 0;
			}
		};

	public:
		std::pair<T*, int> alloc() {
			// Next entry will be at position "entryIdx", which is just what was stored on next
			int entryIdx = next;

			// Figure which block it goes into, and make sure that exists
			int blockIdx = entryIdx / blockLen;
			if (blockIdx >= blocks.size()) {
				blocks.push_back(Block(blocks.size()));
			}
			auto& block = blocks[blockIdx];

			// Find the local entry inside that block and initialize it
			int localIdx = entryIdx % blockLen;
			auto& data = block.data[localIdx];
			int rev = data.revision;
			T* result = reinterpret_cast<T*>(&(data.data));

			// Next block is what was stored on the nextFreeEntryIndex
			std::swap(next, block.data[localIdx].nextFreeEntryIndex);

			// External index composes the revision with the index, so it's unique, but easily mappable
			int externalIdx = static_cast<signed int>(entryIdx & 0xFFFF) | (static_cast<signed int>(rev & 0x7FFF) << 16); // TODO: compute properly
			return std::pair<T*, int>(result, externalIdx);
		}

		void free(T* p) {
			// Swaps the data with the next, so this will actually be the next one to be allocated
			Entry* entry = reinterpret_cast<Entry*>(p);
			std::swap(entry->nextFreeEntryIndex, next);

			// Increase revision so the next one to allocate this gets a unique number
			++entry->revision;
		}

		void freeId(int externalIdx) {
			free(get(externalIdx));
		}

		T* get(int externalIdx) {
			unsigned short idx = static_cast<unsigned short>(externalIdx & 0xFFFF);
			unsigned short rev = static_cast<unsigned short>(externalIdx >> 16);

			int blockN = idx / blockLen;
			if (blockN < 0 || blockN >= signed(blocks.size())) {
				return nullptr;
			}

			// TODO: check if can shrink?

			auto& block = blocks[blockN];
			int localIdx = idx % blockLen;
			auto& data = block.data[localIdx];
			if (data.revision != rev) {
				return nullptr;
			}
			return reinterpret_cast<T*>(&(data.data));
		}

	private:
		Vector<Block> blocks;
		unsigned short next = 0;
	};
}