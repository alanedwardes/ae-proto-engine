#include "ManifestReader.h"
#include "Point.h"

struct RenderData
{
	virtual void LoadManifest(ManifestReader oRenderingManifest)
	{
		size = oRenderingManifest.GetPoint("size");
		texture = oRenderingManifest.GetFile("texture");
	};

	Point size;
	std::string texture;
};