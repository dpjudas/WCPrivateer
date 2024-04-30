#pragma once

#include <vector>
#include <string>
#include <cstdint>

class FileEntryReader
{
public:
	FileEntryReader(std::vector<uint8_t> buffer) : buffer(std::move(buffer)) { }

	void PushChunk(std::string tagname)
	{
		ReadTag(tagname);
		uint32_t chunksize = ReadUint32BE();
		chunks.push_back({ Tell(), chunksize });
	}

	std::string PushChunk()
	{
		std::string tag = ReadTag();
		uint32_t chunksize = ReadUint32BE();
		chunks.push_back({ Tell(), chunksize });
		return tag;
	}

	uint32_t GetChunkSize() { return chunks.back().second; }

	bool IsEndOfChunk()
	{
		return Tell() - chunks.back().first == chunks.back().second;
	}

	void PopChunk()
	{
		// Chunks always 16 bit align
		Seek(chunks.back().first + chunks.back().second + (chunks.back().second & 1));
		chunks.pop_back();
	}

	uint8_t ReadUint8()
	{
		return pos < buffer.size() ? buffer[pos++] : 0;
	}

	void ReadTag(std::string tagname)
	{
		std::string tag = ReadTag();
		if (tag != tagname)
			throw std::runtime_error("Expected " + tagname);
	}

	std::string ReadTag()
	{
		char tag[4];
		for (int i = 0; i < 4; i++)
			tag[i] = ReadUint8();
		return std::string(tag, 4);
	}

	uint32_t ReadUint32BE()
	{
		uint32_t v0 = ReadUint8();
		uint32_t v1 = ReadUint8();
		uint32_t v2 = ReadUint8();
		uint32_t v3 = ReadUint8();
		return (v0 << 24) | (v1 << 16) | (v2 << 8) | v3;
	}

	int16_t ReadInt16()
	{
		return ReadUint16();
	}

	uint16_t ReadUint16()
	{
		uint32_t v0 = ReadUint8();
		uint32_t v1 = ReadUint8();
		return (v1 << 8) | v0;
	}

	uint32_t ReadUint24()
	{
		uint32_t v0 = ReadUint8();
		uint32_t v1 = ReadUint8();
		uint32_t v2 = ReadUint8();
		return (v2 << 16) | (v1 << 8) | v0;
	}

	uint32_t ReadUint32()
	{
		uint32_t v0 = ReadUint8();
		uint32_t v1 = ReadUint8();
		uint32_t v2 = ReadUint8();
		uint32_t v3 = ReadUint8();
		return (v3 << 24) | (v2 << 16) | (v1 << 8) | v0;
	}

	const uint8_t* CurrentPosData() const { return &buffer[pos]; }

	size_t Tell() { return pos; }
	size_t Size() { return buffer.size(); }
	void Seek(size_t newpos) { pos = newpos; }

private:
	std::vector<uint8_t> buffer;
	size_t pos = 0;
	std::vector<std::pair<size_t, uint32_t>> chunks;
};
